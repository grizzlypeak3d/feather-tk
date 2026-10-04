// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GPU/RenderPrivate.h>

#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>

#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace ftk
{
    namespace gpu
    {
        namespace
        {
            const size_t chunkByteCount = 1024 * 1024;

            SDL_GPUTextureFormat getFormat(BufferType value)
            {
                SDL_GPUTextureFormat out = SDL_GPU_TEXTUREFORMAT_INVALID;
                switch (value)
                {
                case BufferType::RGBA_U8: out = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM; break;
                case BufferType::RGBA_F16: out = SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT; break;
                case BufferType::RGBA_F32: out = SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT; break;
                default: break;
                }
                return out;
            }

            SDL_GPUShader* createShader(
                SDL_GPUDevice* device,
                const std::string& source,
                const char* entryPoint,
                SDL_GPUShaderStage stage,
                Uint32 samplers)
            {
                SDL_GPUShaderCreateInfo info = {};
                info.code = reinterpret_cast<const Uint8*>(source.c_str());
                info.code_size = source.size() + 1;
                info.entrypoint = entryPoint;
                info.format = SDL_GPU_SHADERFORMAT_MSL;
                info.stage = stage;
                info.num_samplers = samplers;
                info.num_uniform_buffers = 1;
                SDL_GPUShader* out = SDL_CreateGPUShader(device, &info);
                if (!out)
                {
                    throw std::runtime_error(Format("Cannot compile shader: {0}").arg(SDL_GetError()));
                }
                return out;
            }
        }

        void Render::Private::beginPass()
        {
            if (pass || !cmd || !target)
                return;
            SDL_GPUColorTargetInfo color = {};
            color.texture = target->getTexture();
            color.load_op = clearPending ? SDL_GPU_LOADOP_CLEAR : SDL_GPU_LOADOP_LOAD;
            color.store_op = SDL_GPU_STOREOP_STORE;
            color.clear_color.r = clearColor.r;
            color.clear_color.g = clearColor.g;
            color.clear_color.b = clearColor.b;
            color.clear_color.a = clearColor.a;
            pass = SDL_BeginGPURenderPass(cmd, &color, 1, nullptr);
            clearPending = false;
            applyState();
        }

        void Render::Private::endPass()
        {
            if (pass)
            {
                SDL_EndGPURenderPass(pass);
                pass = nullptr;
            }
        }

        void Render::Private::applyState()
        {
            if (!pass || !target)
                return;
            // Both from the top left, which is where the user interface
            // counts from too: nothing to turn over, as OpenGL has.
            SDL_GPUViewport v = {};
            v.x = static_cast<float>(viewport.x());
            v.y = static_cast<float>(viewport.y());
            v.w = static_cast<float>(viewport.w());
            v.h = static_cast<float>(viewport.h());
            v.min_depth = 0.F;
            v.max_depth = 1.F;
            SDL_SetGPUViewport(pass, &v);

            // Held inside the target: a scissor that leaves it is an error
            // to Metal rather than something it clips.
            const Size2I& targetSize = target->getSize();
            Box2I scissor(0, 0, targetSize.w, targetSize.h);
            if (clipRectEnabled)
            {
                scissor = intersect(scissor, clipRect);
            }
            SDL_Rect r = { 0, 0, 0, 0 };
            if (scissor.isValid())
            {
                r.x = scissor.x();
                r.y = scissor.y();
                r.w = scissor.w();
                r.h = scissor.h();
            }
            SDL_SetGPUScissor(pass, &r);
        }

        const Render::Private::Shader& Render::Private::getShader(const std::string& name)
        {
            const auto i = shaders.find(name);
            if (i != shaders.end())
            {
                return i->second;
            }
            if (!(SDL_GetGPUShaderFormats(device) & SDL_GPU_SHADERFORMAT_MSL))
            {
                throw std::runtime_error(Format(
                    "The GPU renderer has no shaders for the \"{0}\" driver yet").
                    arg(SDL_GetGPUDeviceDriver(device)));
            }
            Shader shader;
            std::string vertex = vertexSourceMSL();
            std::string fragment;
            Uint32 samplers = 0;
            if ("mesh" == name)
            {
                fragment = meshFragmentSourceMSL();
            }
            else if ("colorMesh" == name)
            {
                vertex = colorMeshVertexSourceMSL();
                fragment = colorMeshFragmentSourceMSL();
                shader.vertexType = VertexType::Color;
            }
            else if ("texture" == name)
            {
                fragment = textureFragmentSourceMSL();
                samplers = 1;
            }
            else if ("text" == name)
            {
                fragment = textFragmentSourceMSL();
                samplers = 1;
            }
            else if ("image" == name)
            {
                fragment = imageFragmentSourceMSL();
                samplers = 3;
            }
            shader.vertex = createShader(device, vertex, "vertexMain", SDL_GPU_SHADERSTAGE_VERTEX, 0);
            shader.fragment = createShader(device, fragment, "fragmentMain", SDL_GPU_SHADERSTAGE_FRAGMENT, samplers);
            shaders[name] = shader;
            return shaders[name];
        }

        SDL_GPUGraphicsPipeline* Render::Private::getPipeline(const std::string& name, Blend blend)
        {
            // What OpenGL changes between draws -- the program, the blend
            // function -- is here part of an object made ahead of them, one
            // for each combination that is drawn with.
            const auto key = std::make_tuple(name, blend, target->getType());
            const auto i = pipelines.find(key);
            if (i != pipelines.end())
            {
                return i->second;
            }
            const Shader& shader = getShader(name);

            SDL_GPUVertexBufferDescription buffer = {};
            buffer.slot = 0;
            buffer.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX;
            SDL_GPUVertexAttribute attributes[2] = {};
            attributes[0].location = 0;
            attributes[0].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
            attributes[0].offset = 0;
            attributes[1].location = 1;
            attributes[1].offset = sizeof(float) * 2;
            if (VertexType::Color == shader.vertexType)
            {
                buffer.pitch = sizeof(VertexColor);
                attributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT4;
            }
            else
            {
                buffer.pitch = sizeof(VertexUV);
                attributes[1].format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2;
            }

            SDL_GPUColorTargetDescription targetDesc = {};
            targetDesc.format = getFormat(target->getType());
            auto& b = targetDesc.blend_state;
            b.enable_blend = true;
            b.color_blend_op = SDL_GPU_BLENDOP_ADD;
            b.alpha_blend_op = SDL_GPU_BLENDOP_ADD;
            switch (blend)
            {
            case Blend::Default:
                b.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
                b.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
                b.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
                b.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
                break;
            case Blend::None:
                b.src_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
                b.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ZERO;
                b.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
                b.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO;
                break;
            case Blend::Straight:
                b.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA;
                b.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
                b.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
                b.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
                break;
            case Blend::Premultiplied:
                b.src_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
                b.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
                b.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
                b.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
                break;
            }

            SDL_GPUGraphicsPipelineCreateInfo info = {};
            info.vertex_shader = shader.vertex;
            info.fragment_shader = shader.fragment;
            info.vertex_input_state.vertex_buffer_descriptions = &buffer;
            info.vertex_input_state.num_vertex_buffers = 1;
            info.vertex_input_state.vertex_attributes = attributes;
            info.vertex_input_state.num_vertex_attributes = 2;
            info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
            info.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
            // Back faces are culled, as the OpenGL renderer culls them:
            // which way a mesh winds decides whether it is drawn at all.
            info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_BACK;
            info.rasterizer_state.front_face = SDL_GPU_FRONTFACE_COUNTER_CLOCKWISE;
            info.target_info.color_target_descriptions = &targetDesc;
            info.target_info.num_color_targets = 1;
            SDL_GPUGraphicsPipeline* out = SDL_CreateGPUGraphicsPipeline(device, &info);
            if (!out)
            {
                throw std::runtime_error(Format("Cannot create a pipeline: {0}").arg(SDL_GetError()));
            }
            pipelines[key] = out;
            return out;
        }

        void Render::Private::draw(
            const std::string& shader,
            Blend blend,
            const void* vertices,
            size_t vertexCount,
            size_t vertexByteCount,
            const M44F& transform,
            const void* uniforms,
            size_t uniformsByteCount,
            const SDL_GPUTextureSamplerBinding* textures,
            size_t textureCount)
        {
            if (!cmd || !target || 0 == vertexCount)
                return;
            // An empty clip clips everything.
            if (clipRectEnabled && !clipRect.isValid())
                return;
            beginPass();
            if (!pass)
                return;

            // Somewhere to put the vertices.
            const size_t byteCount = vertexCount * vertexByteCount;
            while (chunk < chunks.size() &&
                chunks[chunk].used + byteCount > chunks[chunk].data.size())
            {
                ++chunk;
            }
            if (chunk >= chunks.size())
            {
                Chunk c;
                c.data.resize(std::max(chunkByteCount, byteCount));
                SDL_GPUBufferCreateInfo bufferInfo = {};
                bufferInfo.usage = SDL_GPU_BUFFERUSAGE_VERTEX;
                bufferInfo.size = static_cast<Uint32>(c.data.size());
                c.buffer = SDL_CreateGPUBuffer(device, &bufferInfo);
                SDL_GPUTransferBufferCreateInfo transferInfo = {};
                transferInfo.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD;
                transferInfo.size = static_cast<Uint32>(c.data.size());
                c.transfer = SDL_CreateGPUTransferBuffer(device, &transferInfo);
                if (!c.buffer || !c.transfer)
                {
                    throw std::runtime_error(Format("Cannot create a vertex buffer: {0}").arg(SDL_GetError()));
                }
                chunks.push_back(c);
                chunk = chunks.size() - 1;
            }
            Chunk& c = chunks[chunk];
            const size_t offset = c.used;
            std::memcpy(c.data.data() + offset, vertices, byteCount);
            c.used += byteCount;

            SDL_BindGPUGraphicsPipeline(pass, getPipeline(shader, blend));
            SDL_GPUBufferBinding binding = {};
            binding.buffer = c.buffer;
            binding.offset = static_cast<Uint32>(offset);
            SDL_BindGPUVertexBuffers(pass, 0, &binding, 1);

            // Column by column, which is how the shader reads a matrix and
            // not how this one is kept.
            float mvp[16];
            for (int row = 0; row < 4; ++row)
            {
                for (int column = 0; column < 4; ++column)
                {
                    mvp[column * 4 + row] = transform.get(row, column);
                }
            }
            SDL_PushGPUVertexUniformData(cmd, 0, mvp, sizeof(mvp));
            SDL_PushGPUFragmentUniformData(cmd, 0, uniforms, static_cast<Uint32>(uniformsByteCount));
            if (textureCount > 0)
            {
                SDL_BindGPUFragmentSamplers(pass, 0, textures, static_cast<Uint32>(textureCount));
            }
            SDL_DrawGPUPrimitives(pass, static_cast<Uint32>(vertexCount), 1, 0, 0);
        }

        void Render::Private::drawUV(
            const std::string& shader,
            Blend blend,
            const TriMesh2F& mesh,
            const M44F& transform,
            const void* uniforms,
            size_t uniformsByteCount,
            const SDL_GPUTextureSamplerBinding* textures,
            size_t textureCount)
        {
            const size_t vSize = mesh.v.size();
            const size_t tSize = mesh.t.size();
            verticesUV.resize(mesh.triangles.size() * 3);
            VertexUV* out = verticesUV.data();
            for (const auto& triangle : mesh.triangles)
            {
                for (size_t k = 0; k < 3; ++k, ++out)
                {
                    const size_t v = triangle.v[k].v;
                    const size_t t = triangle.v[k].t;
                    out->x = v && v <= vSize ? mesh.v[v - 1].x : 0.F;
                    out->y = v && v <= vSize ? mesh.v[v - 1].y : 0.F;
                    out->u = t && t <= tSize ? mesh.t[t - 1].x : 0.F;
                    out->v = t && t <= tSize ? mesh.t[t - 1].y : 0.F;
                }
            }
            diag.triangles += mesh.triangles.size();
            draw(
                shader,
                blend,
                verticesUV.data(),
                verticesUV.size(),
                sizeof(VertexUV),
                transform,
                uniforms,
                uniformsByteCount,
                textures,
                textureCount);
        }

        void Render::_init(
            const std::shared_ptr<System>& system,
            const std::shared_ptr<LogSystem>& logSystem,
            const std::shared_ptr<FontSystem>& fontSystem)
        {
            IRender::_init(logSystem, fontSystem);
            FTK_P();
            p.system = system;
            p.device = system->getDevice();

            SDL_GPUSamplerCreateInfo samplerInfo = {};
            samplerInfo.min_filter = SDL_GPU_FILTER_LINEAR;
            samplerInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
            samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
            samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
            samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
            p.sampler = SDL_CreateGPUSampler(p.device, &samplerInfo);
        }

        Render::Render() :
            _p(new Private)
        {}

        Render::~Render()
        {
            FTK_P();
            if (p.device)
            {
                p.endPass();
                if (p.cmd)
                {
                    SDL_CancelGPUCommandBuffer(p.cmd);
                }
                for (const auto& i : p.pipelines)
                {
                    SDL_ReleaseGPUGraphicsPipeline(p.device, i.second);
                }
                for (const auto& i : p.shaders)
                {
                    SDL_ReleaseGPUShader(p.device, i.second.vertex);
                    SDL_ReleaseGPUShader(p.device, i.second.fragment);
                }
                for (const auto& i : p.chunks)
                {
                    SDL_ReleaseGPUBuffer(p.device, i.buffer);
                    SDL_ReleaseGPUTransferBuffer(p.device, i.transfer);
                }
                if (p.sampler)
                {
                    SDL_ReleaseGPUSampler(p.device, p.sampler);
                }
            }
        }

        std::shared_ptr<Render> Render::create(
            const std::shared_ptr<System>& system,
            const std::shared_ptr<LogSystem>& logSystem,
            const std::shared_ptr<FontSystem>& fontSystem)
        {
            auto out = std::shared_ptr<Render>(new Render);
            out->_init(system, logSystem, fontSystem);
            return out;
        }

        void Render::setTarget(const std::shared_ptr<OffscreenBuffer>& value)
        {
            _p->target = value;
        }

        void Render::begin(
            const Size2I& size,
            const RenderOptions& options)
        {
            FTK_P();

            p.startTime = std::chrono::steady_clock::now();
            p.diag = RenderDiag();

            p.size = size;
            p.options = options;
            p.texturePool.setMax(options.texturePoolByteCount);
            p.textureCache.setMax(options.textureCacheByteCount);

            const int glyphAtlasSize = options.glyphAtlasSize > 0 ? options.glyphAtlasSize : 4096;
            if (!p.glyphTexture || glyphAtlasSize != p.glyphAtlasSize)
            {
                p.glyphAtlasSize = glyphAtlasSize;
                TextureOptions textureOptions;
                p.glyphTexture = Texture::create(
                    p.system,
                    ImageInfo(glyphAtlasSize, glyphAtlasSize, ImageType::L_U8),
                    textureOptions);
                p.glyphPack = BoxPack::create(Size2I(glyphAtlasSize, glyphAtlasSize), 1);
                p.glyphIDs.clear();
            }

            p.cmd = SDL_AcquireGPUCommandBuffer(p.device);
            if (!p.cmd)
            {
                throw std::runtime_error(Format("Cannot acquire a command buffer: {0}").arg(SDL_GetError()));
            }
            p.pass = nullptr;
            p.chunk = 0;
            for (auto& i : p.chunks)
            {
                i.used = 0;
            }
            p.clearPending = options.clear;
            p.clearColor = options.clearColor;
            p.clipRectEnabled = false;

            setViewport(Box2I(0, 0, size.w, size.h));
            setTransform(ortho(
                0.F,
                static_cast<float>(size.w),
                static_cast<float>(size.h),
                0.F,
                -1.F,
                1.F));
        }

        void Render::end()
        {
            FTK_P();
            if (p.cmd)
            {
                // A frame that cleared and drew nothing still clears.
                if (p.clearPending)
                {
                    p.beginPass();
                }
                p.endPass();

                // The vertices, in a command buffer submitted ahead of the
                // one that draws them.
                bool upload = false;
                for (const auto& i : p.chunks)
                {
                    upload |= i.used > 0;
                }
                if (upload)
                {
                    SDL_GPUCommandBuffer* uploadCmd = SDL_AcquireGPUCommandBuffer(p.device);
                    SDL_GPUCopyPass* copyPass = SDL_BeginGPUCopyPass(uploadCmd);
                    for (auto& i : p.chunks)
                    {
                        if (i.used > 0)
                        {
                            void* data = SDL_MapGPUTransferBuffer(p.device, i.transfer, true);
                            std::memcpy(data, i.data.data(), i.used);
                            SDL_UnmapGPUTransferBuffer(p.device, i.transfer);
                            SDL_GPUTransferBufferLocation source = {};
                            source.transfer_buffer = i.transfer;
                            SDL_GPUBufferRegion destination = {};
                            destination.buffer = i.buffer;
                            destination.size = static_cast<Uint32>(i.used);
                            // Not cycled: the draws already name this
                            // buffer, and a cycle would hand them the old
                            // one.
                            SDL_UploadToGPUBuffer(copyPass, &source, &destination, false);
                        }
                    }
                    SDL_EndGPUCopyPass(copyPass);
                    SDL_SubmitGPUCommandBuffer(uploadCmd);
                }

                SDL_SubmitGPUCommandBuffer(p.cmd);
                p.cmd = nullptr;
            }

            const auto now = std::chrono::steady_clock::now();
            const auto diff = std::chrono::duration_cast<std::chrono::microseconds>(
                now - p.startTime);
            p.frameTimes[p.frameTimePos] = diff.count();
            p.frameTimePos = (p.frameTimePos + 1) % p.frameTimes.size();
            p.frameTimeCount = std::min(p.frameTimeCount + 1, p.frameTimes.size());
            int64_t total = 0;
            int64_t peak = 0;
            for (size_t i = 0; i < p.frameTimeCount; ++i)
            {
                total += p.frameTimes[i];
                peak = std::max(peak, p.frameTimes[i]);
            }
            p.diag.time = total / static_cast<int64_t>(p.frameTimeCount);
            p.diag.timePeak = peak;
        }

        Size2I Render::getRenderSize() const
        {
            return _p->size;
        }

        void Render::setRenderSize(const Size2I& value)
        {
            _p->size = value;
        }

        RenderOptions Render::getRenderOptions() const
        {
            return _p->options;
        }

        Box2I Render::getViewport() const
        {
            return _p->viewport;
        }

        void Render::setViewport(const Box2I& value)
        {
            FTK_P();
            p.viewport = value;
            p.applyState();
        }

        void Render::clearViewport(const Color4F& value)
        {
            FTK_P();
            // A clear is how a pass starts, so the one under way ends.
            p.endPass();
            p.clearPending = true;
            p.clearColor = value;
        }

        bool Render::getClipRectEnabled() const
        {
            return _p->clipRectEnabled;
        }

        void Render::setClipRectEnabled(bool value)
        {
            FTK_P();
            p.clipRectEnabled = value;
            p.applyState();
        }

        Box2I Render::getClipRect() const
        {
            return _p->clipRect;
        }

        void Render::setClipRect(const Box2I& value)
        {
            FTK_P();
            p.clipRect = value;
            p.applyState();
        }

        M44F Render::getTransform() const
        {
            return _p->transform;
        }

        void Render::setTransform(const M44F& value)
        {
            _p->transform = value;
        }

        RenderDiag Render::getDiag() const
        {
            return _p->diag;
        }

        RenderFactory::RenderFactory(const std::shared_ptr<System>& system) :
            _system(system)
        {}

        RenderFactory::~RenderFactory()
        {}

        std::shared_ptr<IRender> RenderFactory::createRender(
            const std::shared_ptr<LogSystem>& logSystem,
            const std::shared_ptr<FontSystem>& fontSystem)
        {
            return Render::create(_system.lock(), logSystem, fontSystem);
        }
    }
}
