// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GPU/Shader.h>

#include <ftk/Core/Format.h>

#include <SDL3/SDL.h>

#if defined(FTK_GLSLANG)
#include <glslang/Public/ResourceLimits.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/SPIRV/GlslangToSpv.h>
#endif // FTK_GLSLANG

#include <cstdlib>
#include <mutex>
#include <stdexcept>

namespace ftk
{
    namespace gpu
    {
        bool hasGLSLCompiler()
        {
#if defined(FTK_GLSLANG)
            return true;
#else // FTK_GLSLANG
            return false;
#endif // FTK_GLSLANG
        }

        std::vector<uint32_t> compileGLSL(const std::string& source, ShaderStage stage)
        {
            std::vector<uint32_t> out;
#if defined(FTK_GLSLANG)
            // Once for the process, and never finalized: the compiler's
            // tables are needed for as long as shaders are made.
            static std::once_flag once;
            std::call_once(once, [] { glslang::InitializeProcess(); });

            const EShLanguage language = ShaderStage::Vertex == stage ?
                EShLangVertex :
                EShLangFragment;
            glslang::TShader shader(language);
            const char* strings[] = { source.c_str() };
            shader.setStrings(strings, 1);
            shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 100);
            shader.setEnvClient(glslang::EShClientVulkan, glslang::EShTargetVulkan_1_0);
            shader.setEnvTarget(glslang::EShTargetSpv, glslang::EShTargetSpv_1_0);
            const EShMessages messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules);
            if (!shader.parse(GetDefaultResources(), 450, false, messages))
            {
                throw std::runtime_error(Format("Cannot compile shader: {0}").arg(shader.getInfoLog()));
            }
            glslang::TProgram program;
            program.addShader(&shader);
            if (!program.link(messages))
            {
                throw std::runtime_error(Format("Cannot link shader: {0}").arg(program.getInfoLog()));
            }
            glslang::GlslangToSpv(*program.getIntermediate(language), out);
#else // FTK_GLSLANG
            throw std::runtime_error("This build has no GLSL compiler: it was built without glslang");
#endif // FTK_GLSLANG
            return out;
        }

        bool validateGLSL()
        {
            static const bool out = std::getenv("FTK_GPU_VALIDATE") != nullptr;
            return out;
        }

        SDL_GPUShader* createShader(
            SDL_GPUDevice* device,
            const ShaderSource& source,
            ShaderStage stage,
            size_t samplers,
            size_t uniformBuffers)
        {
            SDL_GPUShaderCreateInfo info = {};
            info.stage = ShaderStage::Vertex == stage ?
                SDL_GPU_SHADERSTAGE_VERTEX :
                SDL_GPU_SHADERSTAGE_FRAGMENT;
            info.num_samplers = static_cast<Uint32>(samplers);
            info.num_uniform_buffers = static_cast<Uint32>(uniformBuffers);

            // The GLSL is compiled where it is not what the device takes,
            // when that is asked for: an error in it is then found on a
            // machine that can only run the other.
            std::vector<uint32_t> spirv;
            const SDL_GPUShaderFormat formats = SDL_GetGPUShaderFormats(device);
            const bool msl = formats & SDL_GPU_SHADERFORMAT_MSL;
            if (!msl || (validateGLSL() && hasGLSLCompiler()))
            {
                if (source.glsl.empty())
                {
                    throw std::runtime_error("The shader has no GLSL source");
                }
                spirv = compileGLSL(source.glsl, stage);
            }

            if (msl)
            {
                if (source.msl.empty())
                {
                    throw std::runtime_error("The shader has no Metal source");
                }
                info.format = SDL_GPU_SHADERFORMAT_MSL;
                info.code = reinterpret_cast<const Uint8*>(source.msl.c_str());
                info.code_size = source.msl.size() + 1;
                info.entrypoint = ShaderStage::Vertex == stage ? "vertexMain" : "fragmentMain";
            }
            else if (formats & SDL_GPU_SHADERFORMAT_SPIRV)
            {
                info.format = SDL_GPU_SHADERFORMAT_SPIRV;
                info.code = reinterpret_cast<const Uint8*>(spirv.data());
                info.code_size = spirv.size() * sizeof(uint32_t);
                info.entrypoint = "main";
            }
            else
            {
                throw std::runtime_error(Format(
                    "The GPU renderer has no shaders for the \"{0}\" driver").
                    arg(SDL_GetGPUDeviceDriver(device)));
            }
            SDL_GPUShader* out = SDL_CreateGPUShader(device, &info);
            if (!out)
            {
                throw std::runtime_error(Format("Cannot create shader: {0}").arg(SDL_GetError()));
            }
            return out;
        }
    }
}
