// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GPU/Present.h>

#include <ftk/GPU/Shader.h>

#include <ftk/GPU/System.h>

#include <ftk/Core/Format.h>

#include <SDL3/SDL.h>

#include <cstdlib>
#include <map>
#include <stdexcept>

namespace ftk
{
    namespace gpu
    {
        namespace
        {
            const std::string vertexSource =
                "#include <metal_stdlib>\n"
                "using namespace metal;\n"
                "\n"
                "struct VertexOut\n"
                "{\n"
                "    float4 position [[position]];\n"
                "    float2 uv [[user(locn0)]];\n"
                "};\n"
                "\n"
                "// One triangle that covers the target.\n"
                "vertex VertexOut vertexMain(uint id [[vertex_id]])\n"
                "{\n"
                "    VertexOut out;\n"
                "    float2 p = float2((id << 1) & 2, id & 2);\n"
                "    out.position = float4(p * 2.0 - 1.0, 0.0, 1.0);\n"
                "    out.uv = float2(p.x, 1.0 - p.y);\n"
                "    return out;\n"
                "}\n";

            const std::string fragmentSource =
                "#include <metal_stdlib>\n"
                "using namespace metal;\n"
                "\n"
                "struct VertexOut\n"
                "{\n"
                "    float4 position [[position]];\n"
                "    float2 uv [[user(locn0)]];\n"
                "};\n"
                "\n"
                "struct Uniforms\n"
                "{\n"
                "    int composition;\n"
                "    float sdrWhiteLevel;\n"
                "};\n"
                "\n"
                "constant int Composition_SDR = 0;\n"
                "constant int Composition_HDRExtendedLinear = 1;\n"
                "constant int Composition_HDR10 = 2;\n"
                "\n"
                "// The sRGB curve, carried on past one and mirrored below zero.\n"
                "float3 toLinear(float3 v)\n"
                "{\n"
                "    float3 a = abs(v);\n"
                "    float3 lo = a / 12.92;\n"
                "    float3 hi = pow((a + 0.055) / 1.055, float3(2.4));\n"
                "    return sign(v) * select(hi, lo, a <= float3(0.04045));\n"
                "}\n"
                "\n"
                "// SMPTE ST 2084, from nits.\n"
                "float3 toPQ(float3 nits)\n"
                "{\n"
                "    const float m1 = 0.1593017578125;\n"
                "    const float m2 = 78.84375;\n"
                "    const float c1 = 0.8359375;\n"
                "    const float c2 = 18.8515625;\n"
                "    const float c3 = 18.6875;\n"
                "    float3 y = pow(clamp(nits / 10000.0, 0.0, 1.0), float3(m1));\n"
                "    return pow((c1 + c2 * y) / (1.0 + c3 * y), float3(m2));\n"
                "}\n"
                "\n"
                "fragment float4 fragmentMain(\n"
                "    VertexOut in [[stage_in]],\n"
                "    constant Uniforms& u [[buffer(0)]],\n"
                "    texture2d<float> t0 [[texture(0)]],\n"
                "    sampler s0 [[sampler(0)]])\n"
                "{\n"
                "    float4 c = t0.sample(s0, in.uv);\n"
                "    if (Composition_HDRExtendedLinear == u.composition)\n"
                "    {\n"
                "        c.rgb = toLinear(c.rgb) * u.sdrWhiteLevel;\n"
                "    }\n"
                "    else if (Composition_HDR10 == u.composition)\n"
                "    {\n"
                "        // Rec. 709 primaries to Rec. 2020, by row.\n"
                "        // Negative values are colors outside Rec. 709, which\n"
                "        // Rec. 2020 may well hold: they are kept until then.\n"
                "        float3 l = toLinear(c.rgb);\n"
                "        float3 r2020 = max(float3(\n"
                "            dot(l, float3(0.627404, 0.329283, 0.043313)),\n"
                "            dot(l, float3(0.069097, 0.919540, 0.011362)),\n"
                "            dot(l, float3(0.016391, 0.088013, 0.895595))), 0.0);\n"
                "        c.rgb = toPQ(r2020 * u.sdrWhiteLevel * 80.0);\n"
                "    }\n"
                "    c.a = 1.0;\n"
                "    return c;\n"
                "}\n";

            const std::string vertexSourceGLSL =
                "#version 450\n"
                "\n"
                "layout(location = 0) out vec2 fTexture;\n"
                "\n"
                "// One triangle that covers the target.\n"
                "void main()\n"
                "{\n"
                "    vec2 p = vec2((gl_VertexIndex << 1) & 2, gl_VertexIndex & 2);\n"
                "    gl_Position = vec4(p * 2.0 - 1.0, 0.0, 1.0);\n"
                "    fTexture = vec2(p.x, 1.0 - p.y);\n"
                "}\n";

            const std::string fragmentSourceGLSL =
                "#version 450\n"
                "\n"
                "layout(location = 0) in vec2 fTexture;\n"
                "layout(location = 0) out vec4 outColor;\n"
                "\n"
                "layout(set = 2, binding = 0) uniform sampler2D s0;\n"
                "\n"
                "layout(set = 3, binding = 0) uniform Uniforms\n"
                "{\n"
                "    int composition;\n"
                "    float sdrWhiteLevel;\n"
                "} u;\n"
                "\n"
                "const int Composition_SDR = 0;\n"
                "const int Composition_HDRExtendedLinear = 1;\n"
                "const int Composition_HDR10 = 2;\n"
                "\n"
                "// The sRGB curve, carried on past one and mirrored below zero.\n"
                "vec3 toLinear(vec3 v)\n"
                "{\n"
                "    vec3 a = abs(v);\n"
                "    vec3 lo = a / 12.92;\n"
                "    vec3 hi = pow((a + 0.055) / 1.055, vec3(2.4));\n"
                "    return sign(v) * mix(hi, lo, lessThanEqual(a, vec3(0.04045)));\n"
                "}\n"
                "\n"
                "// SMPTE ST 2084, from nits.\n"
                "vec3 toPQ(vec3 nits)\n"
                "{\n"
                "    const float m1 = 0.1593017578125;\n"
                "    const float m2 = 78.84375;\n"
                "    const float c1 = 0.8359375;\n"
                "    const float c2 = 18.8515625;\n"
                "    const float c3 = 18.6875;\n"
                "    vec3 y = pow(clamp(nits / 10000.0, 0.0, 1.0), vec3(m1));\n"
                "    return pow((c1 + c2 * y) / (1.0 + c3 * y), vec3(m2));\n"
                "}\n"
                "\n"
                "void main()\n"
                "{\n"
                "    vec4 c = texture(s0, fTexture);\n"
                "    if (Composition_HDRExtendedLinear == u.composition)\n"
                "    {\n"
                "        c.rgb = toLinear(c.rgb) * u.sdrWhiteLevel;\n"
                "    }\n"
                "    else if (Composition_HDR10 == u.composition)\n"
                "    {\n"
                "        // Rec. 709 primaries to Rec. 2020, by row.\n"
                "        // Negative values are colors outside Rec. 709, which\n"
                "        // Rec. 2020 may well hold: they are kept until then.\n"
                "        vec3 l = toLinear(c.rgb);\n"
                "        vec3 r2020 = max(vec3(\n"
                "            dot(l, vec3(0.627404, 0.329283, 0.043313)),\n"
                "            dot(l, vec3(0.069097, 0.919540, 0.011362)),\n"
                "            dot(l, vec3(0.016391, 0.088013, 0.895595))), 0.0);\n"
                "        c.rgb = toPQ(r2020 * u.sdrWhiteLevel * 80.0);\n"
                "    }\n"
                "    c.a = 1.0;\n"
                "    outColor = c;\n"
                "}\n";

            struct Uniforms
            {
                int32_t composition = 0;
                float sdrWhiteLevel = 1.F;
                int32_t pad[2] = { 0, 0 };
            };

            SDL_GPUSwapchainComposition getSDL(Composition value)
            {
                SDL_GPUSwapchainComposition out = SDL_GPU_SWAPCHAINCOMPOSITION_SDR;
                switch (value)
                {
                case Composition::HDRExtendedLinear: out = SDL_GPU_SWAPCHAINCOMPOSITION_HDR_EXTENDED_LINEAR; break;
                case Composition::HDR10: out = SDL_GPU_SWAPCHAINCOMPOSITION_HDR10_ST2084; break;
                default: break;
                }
                return out;
            }
        }

        std::string getLabel(Composition value)
        {
            std::string out = "SDR";
            switch (value)
            {
            case Composition::HDRExtendedLinear: out = "HDR extended linear"; break;
            case Composition::HDR10: out = "HDR10"; break;
            default: break;
            }
            return out;
        }

        Composition getCompositionRequest()
        {
            Composition out = Composition::SDR;
            if (const char* env = std::getenv("FTK_GPU_SWAPCHAIN"))
            {
                const std::string s(env);
                if ("hdr" == s)
                {
                    out = Composition::HDRExtendedLinear;
                }
                else if ("hdr10" == s)
                {
                    out = Composition::HDR10;
                }
            }
            return out;
        }

        bool hasCompositionRequest()
        {
            return std::getenv("FTK_GPU_SWAPCHAIN") != nullptr;
        }

        Composition getComposition(SDL_Window* window)
        {
            const bool hdr = SDL_GetBooleanProperty(
                SDL_GetWindowProperties(window),
                SDL_PROP_WINDOW_HDR_ENABLED_BOOLEAN,
                false);
            return hdr ? Composition::HDRExtendedLinear : Composition::SDR;
        }

        Composition setComposition(
            const std::shared_ptr<System>& system,
            SDL_Window* window,
            Composition value)
        {
            SDL_GPUDevice* device = system->getDevice();
            Composition out = Composition::SDR;
            if (value != Composition::SDR &&
                SDL_WindowSupportsGPUSwapchainComposition(device, window, getSDL(value)))
            {
                out = value;
            }
            if (!SDL_SetGPUSwapchainParameters(device, window, getSDL(out), SDL_GPU_PRESENTMODE_VSYNC))
            {
                out = Composition::SDR;
            }
            return out;
        }

        struct Present::Private
        {
            std::shared_ptr<System> system;
            SDL_GPUShader* vertex = nullptr;
            SDL_GPUShader* fragment = nullptr;
            SDL_GPUSampler* sampler = nullptr;
            std::map<int, SDL_GPUGraphicsPipeline*> pipelines;
        };

        Present::Present() :
            _p(new Private)
        {}

        Present::~Present()
        {
            FTK_P();
            if (p.system)
            {
                SDL_GPUDevice* device = p.system->getDevice();
                for (const auto& i : p.pipelines)
                {
                    SDL_ReleaseGPUGraphicsPipeline(device, i.second);
                }
                if (p.vertex)
                {
                    SDL_ReleaseGPUShader(device, p.vertex);
                }
                if (p.fragment)
                {
                    SDL_ReleaseGPUShader(device, p.fragment);
                }
                if (p.sampler)
                {
                    SDL_ReleaseGPUSampler(device, p.sampler);
                }
            }
        }

        std::shared_ptr<Present> Present::create(const std::shared_ptr<System>& system)
        {
            auto out = std::shared_ptr<Present>(new Present);
            out->_p->system = system;
            return out;
        }

        void Present::draw(
            SDL_GPUCommandBuffer* cmd,
            SDL_GPUTexture* source,
            SDL_GPUTexture* destination,
            int destinationFormat,
            Composition composition,
            float sdrWhiteLevel)
        {
            FTK_P();
            SDL_GPUDevice* device = p.system->getDevice();
            if (!p.vertex)
            {
                p.vertex = createShader(
                    device,
                    { vertexSource, vertexSourceGLSL },
                    ShaderStage::Vertex,
                    0,
                    0);
                p.fragment = createShader(
                    device,
                    { fragmentSource, fragmentSourceGLSL },
                    ShaderStage::Fragment,
                    1,
                    1);
                SDL_GPUSamplerCreateInfo samplerInfo = {};
                samplerInfo.min_filter = SDL_GPU_FILTER_LINEAR;
                samplerInfo.mag_filter = SDL_GPU_FILTER_LINEAR;
                samplerInfo.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
                samplerInfo.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
                samplerInfo.address_mode_w = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE;
                p.sampler = SDL_CreateGPUSampler(device, &samplerInfo);
            }
            SDL_GPUGraphicsPipeline* pipeline = nullptr;
            if (const auto i = p.pipelines.find(destinationFormat); i != p.pipelines.end())
            {
                pipeline = i->second;
            }
            else
            {
                SDL_GPUColorTargetDescription target = {};
                target.format = static_cast<SDL_GPUTextureFormat>(destinationFormat);
                SDL_GPUGraphicsPipelineCreateInfo info = {};
                info.vertex_shader = p.vertex;
                info.fragment_shader = p.fragment;
                info.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLELIST;
                info.rasterizer_state.fill_mode = SDL_GPU_FILLMODE_FILL;
                info.rasterizer_state.cull_mode = SDL_GPU_CULLMODE_NONE;
                info.target_info.color_target_descriptions = &target;
                info.target_info.num_color_targets = 1;
                pipeline = SDL_CreateGPUGraphicsPipeline(device, &info);
                if (!pipeline)
                {
                    throw std::runtime_error(Format("Cannot create a pipeline: {0}").arg(SDL_GetError()));
                }
                p.pipelines[destinationFormat] = pipeline;
            }

            SDL_GPUColorTargetInfo color = {};
            color.texture = destination;
            color.load_op = SDL_GPU_LOADOP_DONT_CARE;
            color.store_op = SDL_GPU_STOREOP_STORE;
            SDL_GPURenderPass* pass = SDL_BeginGPURenderPass(cmd, &color, 1, nullptr);
            SDL_BindGPUGraphicsPipeline(pass, pipeline);
            Uniforms uniforms;
            uniforms.composition = static_cast<int32_t>(composition);
            uniforms.sdrWhiteLevel = sdrWhiteLevel;
            SDL_PushGPUFragmentUniformData(cmd, 0, &uniforms, sizeof(uniforms));
            SDL_GPUTextureSamplerBinding binding = {};
            binding.texture = source;
            binding.sampler = p.sampler;
            SDL_BindGPUFragmentSamplers(pass, 0, &binding, 1);
            SDL_DrawGPUPrimitives(pass, 3, 1, 0, 0);
            SDL_EndGPURenderPass(pass);
        }
    }
}
