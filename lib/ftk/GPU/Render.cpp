// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GPU/RenderPrivate.h>

#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>

#include <algorithm>
#include <atomic>
#include <cstdlib>
#include <cstring>
#include <map>
#include <stdexcept>

namespace ftk
{
    namespace gpu
    {
        namespace
        {
            const size_t chunkByteCount = 1024 * 1024;

            std::atomic<size_t> shaderCount = 0;
            std::atomic<size_t> vertexByteCount = 0;
            std::atomic<size_t> textureCacheByteCount = 0;
            std::atomic<size_t> textureCacheCount = 0;
            std::atomic<size_t> texturePoolByteCount = 0;
            std::atomic<size_t> texturePoolCount = 0;

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
            Shader shader;
            ShaderSource vertex = vertexSource();
            ShaderSource fragment;
            size_t samplers = 0;
            if ("mesh" == name)
            {
                fragment = meshFragmentSource();
            }
            else if ("colorMesh" == name)
            {
                vertex = colorMeshVertexSource();
                fragment = colorMeshFragmentSource();
                shader.vertexType = VertexType::Color;
            }
            else if ("texture" == name)
            {
                fragment = textureFragmentSource();
                samplers = 1;
            }
            else if ("text" == name)
            {
                fragment = textFragmentSource();
                samplers = 1;
            }
            else if ("image" == name)
            {
                fragment = imageFragmentSource();
                samplers = 3;
            }
            else if ("textureScale" == name)
            {
                fragment = textureScaleFragmentSource();
                samplers = 2;
            }
            else if ("imageScaleX" == name)
            {
                fragment = imageScaleXFragmentSource();
                samplers = 4;
            }
            else if ("imageScaleY" == name)
            {
                fragment = imageScaleYFragmentSource();
                samplers = 2;
            }
            else if (const auto j = customShaders.find(name); j != customShaders.end())
            {
                fragment = j->second.fragmentSource;
                samplers = j->second.samplers;
            }
            else
            {
                throw std::runtime_error(Format("No shader named \"{0}\"").arg(name));
            }
            shader.vertex = createShader(device, vertex, ShaderStage::Vertex, 0, 1);
            try
            {
                shader.fragment = createShader(device, fragment, ShaderStage::Fragment, samplers, 1);
            }
            catch (const std::exception&)
            {
                SDL_ReleaseGPUShader(device, shader.vertex);
                throw;
            }
            shaderCount += 2;
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
            case Blend::PremultipliedAddAlpha:
                b.src_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
                b.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA;
                b.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
                b.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE;
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
            // The other is depth clamping, which the device is not asked
            // for; see System::getDevice(). Everything is drawn at depth
            // zero, so neither does anything.
            info.rasterizer_state.enable_depth_clip = true;
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
            size_t vertexSize,
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
            const size_t byteCount = vertexCount * vertexSize;
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
                vertexByteCount += c.data.size();
                chunks.push_back(c);
                chunk = chunks.size() - 1;
            }
            Chunk& c = chunks[chunk];
            const size_t offset = c.used;
            std::memcpy(c.data.data() + offset, vertices, byteCount);
            c.used += byteCount;

            SDL_BindGPUGraphicsPipeline(pass, getPipeline(shader, blendEnabled ? blend : Blend::None));
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
            if (uniforms && uniformsByteCount > 0)
            {
                SDL_PushGPUFragmentUniformData(cmd, 0, uniforms, static_cast<Uint32>(uniformsByteCount));
            }
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
            samplerInfo.min_filter = SDL_GPU_FILTER_NEAREST;
            samplerInfo.mag_filter = SDL_GPU_FILTER_NEAREST;
            p.samplerNearest = SDL_CreateGPUSampler(p.device, &samplerInfo);
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
                    shaderCount -= 2;
                }
                for (const auto& i : p.chunks)
                {
                    SDL_ReleaseGPUBuffer(p.device, i.buffer);
                    SDL_ReleaseGPUTransferBuffer(p.device, i.transfer);
                    vertexByteCount -= i.data.size();
                }
                textureCacheByteCount -= p.cacheTotals.cacheByteCount;
                textureCacheCount -= p.cacheTotals.cacheCount;
                texturePoolByteCount -= p.cacheTotals.poolByteCount;
                texturePoolCount -= p.cacheTotals.poolCount;
                if (p.sampler)
                {
                    SDL_ReleaseGPUSampler(p.device, p.sampler);
                }
                if (p.samplerNearest)
                {
                    SDL_ReleaseGPUSampler(p.device, p.samplerNearest);
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

        IGPURender::~IGPURender()
        {}

        std::shared_ptr<Render> getRender(const std::shared_ptr<IRender>& value)
        {
            auto gpuRender = std::dynamic_pointer_cast<IGPURender>(value);
            return gpuRender ? gpuRender->getGPURender() : nullptr;
        }

        bool hasFloatFilter(const std::shared_ptr<System>& system)
        {
            static const bool no = std::getenv("FTK_GPU_NO_FLOAT_FILTER") != nullptr;
            if (no)
            {
                return false;
            }
            static std::map<SDL_GPUDevice*, bool> devices;
            SDL_GPUDevice* device = system->getDevice();
            const auto i = devices.find(device);
            if (i != devices.end())
            {
                return i->second;
            }
            bool out = false;
            try
            {
                // Two texels, zero and one, drawn into one pixel: half of
                // each where the device filters, and one of them where it
                // does not.
                auto image = Image::create(2, 1, ImageType::L_F32);
                const float values[] = { 0.F, 1.F };
                std::memcpy(image->getData(), values, sizeof(values));
                auto buffer = OffscreenBuffer::create(system, Size2I(1, 1), BufferType::RGBA_U8);
                auto render = Render::create(system, nullptr, nullptr);
                render->setTarget(buffer);
                ImageOptions options;
                options.imageFilters.minify = ImageFilter::Linear;
                options.imageFilters.magnify = ImageFilter::Linear;
                options.cache = false;
                render->begin(Size2I(1, 1));
                render->drawImage(image, Box2F(0.F, 0.F, 1.F, 1.F), Color4F(1.F, 1.F, 1.F), options);
                render->end();
                const float value = buffer->getPixel(V2I(0, 0)).r;
                out = value > .4F && value < .6F;
            }
            catch (const std::exception&)
            {}
            devices[device] = out;
            return out;
        }

        std::shared_ptr<Render> Render::getGPURender()
        {
            return std::dynamic_pointer_cast<Render>(shared_from_this());
        }

        const std::shared_ptr<System>& Render::getSystem() const
        {
            return _p->system;
        }

        void Render::pushTarget(
            const std::shared_ptr<OffscreenBuffer>& value,
            bool clear,
            const Color4F& color)
        {
            FTK_P();
            // One pass draws into one target, so the pass under way ends
            // here and another is started when the target comes back.
            p.endPass();
            Private::TargetState state;
            state.target = p.target;
            state.viewport = p.viewport;
            state.clipRectEnabled = p.clipRectEnabled;
            state.clearPending = p.clearPending;
            state.clearColor = p.clearColor;
            p.targets.push_back(state);
            p.target = value;
            p.viewport = Box2I(V2I(), value ? value->getSize() : Size2I());
            p.clipRectEnabled = false;
            p.clearPending = clear;
            p.clearColor = color;
        }

        void Render::popTarget()
        {
            FTK_P();
            if (p.targets.empty())
                return;
            // A buffer that was to be cleared and was never drawn into is
            // still cleared.
            if (p.clearPending)
            {
                p.beginPass();
            }
            p.endPass();
            const Private::TargetState& state = p.targets.back();
            p.target = state.target;
            p.viewport = state.viewport;
            p.clipRectEnabled = state.clipRectEnabled;
            p.clearPending = state.clearPending;
            p.clearColor = state.clearColor;
            p.targets.pop_back();
        }

        const std::shared_ptr<OffscreenBuffer>& Render::getTarget() const
        {
            return _p->target;
        }

        void Render::setShader(
            const std::string& name,
            const ShaderSource& fragmentSource,
            size_t samplers)
        {
            FTK_P();
            removeShader(name);
            Private::CustomShader shader;
            shader.fragmentSource = fragmentSource;
            shader.samplers = samplers;
            p.customShaders[name] = shader;
        }

        bool Render::hasShader(const std::string& name) const
        {
            FTK_P();
            return p.customShaders.find(name) != p.customShaders.end();
        }

        void Render::removeShader(const std::string& name)
        {
            FTK_P();
            p.customShaders.erase(name);
            auto i = p.pipelines.begin();
            while (i != p.pipelines.end())
            {
                if (std::get<0>(i->first) == name)
                {
                    SDL_ReleaseGPUGraphicsPipeline(p.device, i->second);
                    i = p.pipelines.erase(i);
                }
                else
                {
                    ++i;
                }
            }
            if (const auto j = p.shaders.find(name); j != p.shaders.end())
            {
                SDL_ReleaseGPUShader(p.device, j->second.vertex);
                SDL_ReleaseGPUShader(p.device, j->second.fragment);
                shaderCount -= 2;
                p.shaders.erase(j);
            }
        }

        void Render::drawShader(
            const std::string& name,
            Blend blend,
            const TriMesh2F& mesh,
            const M44F& transform,
            const void* uniforms,
            size_t uniformsByteCount,
            const std::vector<TextureBinding>& textures)
        {
            FTK_P();
            if (mesh.triangles.empty())
                return;
            std::vector<SDL_GPUTextureSamplerBinding> bindings(textures.size());
            for (size_t i = 0; i < textures.size(); ++i)
            {
                bindings[i].texture = textures[i].texture;
                bindings[i].sampler = textures[i].sampler;
            }
            p.drawUV(
                name,
                blend,
                mesh,
                transform,
                uniforms,
                uniformsByteCount,
                bindings.data(),
                bindings.size());
        }

        SDL_GPUSampler* Render::getSampler(ImageFilter value) const
        {
            FTK_P();
            return ImageFilter::Nearest == value ? p.samplerNearest : p.sampler;
        }

        void Render::setBlendEnabled(bool value)
        {
            _p->blendEnabled = value;
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
            p.blendEnabled = true;

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
            while (!p.targets.empty())
            {
                popTarget();
            }
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

                // What is drawn reads what was sent in command buffers
                // submitted before this one: the vertices just above, and
                // the textures. That the one is done before the other
                // starts is the driver's to see to. FTK_GPU_SERIALIZE waits
                // here until everything sent is done, which is how to find
                // out whether a driver does: drawing that is wrong without
                // it and right with it is that.
                static const bool serialize = std::getenv("FTK_GPU_SERIALIZE") != nullptr;
                if (serialize)
                {
                    SDL_WaitForGPUIdle(p.device);
                }

                SDL_SubmitGPUCommandBuffer(p.cmd);
                p.cmd = nullptr;
            }

            {
                const auto apply = [](std::atomic<size_t>& total, size_t& previous, size_t current)
                {
                    total += current - previous;
                    previous = current;
                };
                apply(textureCacheByteCount, p.cacheTotals.cacheByteCount, p.textureCache.getSize());
                apply(textureCacheCount, p.cacheTotals.cacheCount, p.textureCache.getCount());
                apply(texturePoolByteCount, p.cacheTotals.poolByteCount, p.texturePool.getSize());
                apply(texturePoolCount, p.cacheTotals.poolCount, p.texturePool.getCount());
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

        size_t Render::getShaderCount()
        {
            return shaderCount;
        }

        size_t Render::getVertexByteCount()
        {
            return vertexByteCount;
        }

        size_t Render::getTextureCacheByteCount()
        {
            return textureCacheByteCount;
        }

        size_t Render::getTextureCacheCount()
        {
            return textureCacheCount;
        }

        size_t Render::getTexturePoolByteCount()
        {
            return texturePoolByteCount;
        }

        size_t Render::getTexturePoolCount()
        {
            return texturePoolCount;
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
