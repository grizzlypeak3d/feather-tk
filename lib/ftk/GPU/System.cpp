// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GPU/System.h>

#include <ftk/GPU/OffscreenBuffer.h>
#include <ftk/GPU/Render.h>
#include <ftk/GPU/Shader.h>
#include <ftk/GPU/Texture.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/DiagSystem.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>
#include <ftk/Core/String.h>

#include <SDL3/SDL.h>

#include <cstdlib>
#include <map>
#include <stdexcept>
#include <vector>

namespace ftk
{
    namespace gpu
    {
        struct System::Private
        {
            SDL_GPUDevice* device = nullptr;
            std::shared_ptr<IRenderFactory> renderFactory;
            std::map<unsigned int, SDL_GPUTexture*> textures;
            unsigned int textureID = 0;
        };

        System::System(const std::shared_ptr<Context>& context) :
            ISystem(context, "ftk::gpu::System"),
            _p(new Private)
        {
            // What the OpenGL system reports of its own, for this renderer.
            // All zero while the windows are drawn with OpenGL.
            auto diagSystem = context->getSystem<DiagSystem>();
            diagSystem->addSampler(
                "ftk GPU Memory/Buffers: {0}MB",
                [] { return OffscreenBuffer::getTotalByteCount() / megabyte; });
            diagSystem->addSampler(
                "ftk GPU Memory/Vertices: {0}MB",
                [] { return Render::getVertexByteCount() / megabyte; });
            diagSystem->addSampler(
                "ftk GPU Memory/Textures: {0}MB",
                [] { return Texture::getTotalByteCount() / megabyte; });
            diagSystem->addSampler(
                "ftk GPU Memory/Texture cache: {0}MB",
                [] { return Render::getTextureCacheByteCount() / megabyte; });
            diagSystem->addSampler(
                "ftk GPU Memory/Texture pool: {0}MB",
                [] { return Render::getTexturePoolByteCount() / megabyte; });

            diagSystem->addSampler(
                "ftk GPU Objects/Buffers: {0}",
                [] { return OffscreenBuffer::getObjectCount(); });
            diagSystem->addSampler(
                "ftk GPU Objects/Shaders: {0}",
                [] { return Render::getShaderCount(); });
            diagSystem->addSampler(
                "ftk GPU Objects/Textures: {0}",
                [] { return Texture::getObjectCount(); });
            diagSystem->addSampler(
                "ftk GPU Objects/Texture cache: {0}",
                [] { return Render::getTextureCacheCount(); });
            diagSystem->addSampler(
                "ftk GPU Objects/Texture pool: {0}",
                [] { return Render::getTexturePoolCount(); });
        }

        System::~System()
        {
            FTK_P();
            // The factory holds this system weakly, and everything made
            // with the device holds the system, so nothing of the device's
            // is left by now.
            p.renderFactory.reset();
            if (p.device)
            {
                SDL_DestroyGPUDevice(p.device);
            }
        }

        std::shared_ptr<System> System::create(const std::shared_ptr<Context>& context)
        {
            auto out = std::shared_ptr<System>(new System(context));
            out->_p->renderFactory = std::make_shared<RenderFactory>(out);
            return out;
        }

        SDL_GPUDevice* System::getDevice()
        {
            FTK_P();
            if (!p.device)
            {
                if (!SDL_WasInit(SDL_INIT_VIDEO) && !SDL_InitSubSystem(SDL_INIT_VIDEO))
                {
                    throw std::runtime_error(Format("Cannot initialize SDL: {0}").arg(SDL_GetError()));
                }
                // The shaders are written for these; see RenderShaders.cpp.
                // Validation is asked for by name: it is what says a call
                // was wrong, and it is not free.
                const bool debug = std::getenv("FTK_GPU_DEBUG") != nullptr;
                p.device = SDL_CreateGPUDevice(
                    SDL_GPU_SHADERFORMAT_MSL | SDL_GPU_SHADERFORMAT_SPIRV,
                    debug,
                    nullptr);
                if (!p.device)
                {
                    throw std::runtime_error(Format("Cannot create a GPU device: {0}").arg(SDL_GetError()));
                }
                _log(Format("GPU driver: {0}").arg(SDL_GetGPUDeviceDriver(p.device)));
                _log(Format("GLSL compiler: {0}{1}").
                    arg(hasGLSLCompiler() ? "glslang" : "none").
                    arg(hasGLSLCompiler() && validateGLSL() ?
                        ", checking every shader's GLSL as it is made" :
                        ""));

                // The formats the textures are made in, some of which
                // Vulkan leaves to the driver. Sixteen bit normalized ones
                // are kept as half float where they are missing; nothing
                // falls back from any other, so a texture that cannot be
                // made is explained here.
                struct TextureFormat
                {
                    SDL_GPUTextureFormat format;
                    std::string name;
                };
                std::vector<std::string> missing;
                for (const TextureFormat& i :
                    {
                        TextureFormat{ SDL_GPU_TEXTUREFORMAT_R8_UNORM, "R8" },
                        TextureFormat{ SDL_GPU_TEXTUREFORMAT_R8G8_UNORM, "RG8" },
                        TextureFormat{ SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM, "RGBA8" },
                        TextureFormat{ SDL_GPU_TEXTUREFORMAT_R16_UNORM, "R16" },
                        TextureFormat{ SDL_GPU_TEXTUREFORMAT_R16G16_UNORM, "RG16" },
                        TextureFormat{ SDL_GPU_TEXTUREFORMAT_R16G16B16A16_UNORM, "RGBA16" },
                        TextureFormat{ SDL_GPU_TEXTUREFORMAT_R16_FLOAT, "R16F" },
                        TextureFormat{ SDL_GPU_TEXTUREFORMAT_R16G16_FLOAT, "RG16F" },
                        TextureFormat{ SDL_GPU_TEXTUREFORMAT_R16G16B16A16_FLOAT, "RGBA16F" },
                        TextureFormat{ SDL_GPU_TEXTUREFORMAT_R32_FLOAT, "R32F" },
                        TextureFormat{ SDL_GPU_TEXTUREFORMAT_R32G32_FLOAT, "RG32F" },
                        TextureFormat{ SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT, "RGBA32F" }
                    })
                {
                    if (!SDL_GPUTextureSupportsFormat(
                        p.device,
                        i.format,
                        SDL_GPU_TEXTURETYPE_2D,
                        SDL_GPU_TEXTUREUSAGE_SAMPLER))
                    {
                        missing.push_back(i.name);
                    }
                }
                _log(
                    Format("Texture formats: {0}{1}").
                        arg(missing.empty() ? "all supported" : "not supported: " + join(missing, ", ")).
                        arg(hasUNorm16(p.device) ? "" : "; sixteen bit normalized are kept as half float"),
                    missing.empty() ? LogType::Message : LogType::Warning);
            }
            return p.device;
        }

        std::string System::getDriver()
        {
            return SDL_GetGPUDeviceDriver(getDevice());
        }

        const std::shared_ptr<IRenderFactory>& System::getRenderFactory() const
        {
            return _p->renderFactory;
        }

        void System::setRenderFactory(const std::shared_ptr<IRenderFactory>& value)
        {
            _p->renderFactory = value;
        }

        unsigned int System::addTexture(SDL_GPUTexture* value)
        {
            FTK_P();
            // Zero is no texture, as it is in OpenGL.
            ++p.textureID;
            p.textures[p.textureID] = value;
            return p.textureID;
        }

        void System::removeTexture(unsigned int value)
        {
            _p->textures.erase(value);
        }

        SDL_GPUTexture* System::getTexture(unsigned int value) const
        {
            FTK_P();
            const auto i = p.textures.find(value);
            return i != p.textures.end() ? i->second : nullptr;
        }

        namespace
        {
            bool enabled = false;
        }

        bool isEnabled()
        {
            return enabled;
        }

        void init(const std::shared_ptr<Context>& context)
        {
            if (!context->getSystem<System>())
            {
                auto system = System::create(context);
                context->addSystem(system);

                // Whether the windows are drawn with this renderer: asked
                // for by name, and only where there is a device to draw
                // with. Finding that out is making one, which is done here
                // rather than by the first window, so that what cannot be
                // had is known while there is still another renderer to
                // choose.
                const char* env = std::getenv("FTK_RENDER");
                const std::string render = env ? env : "";
                if ("gpu" == render)
                {
                    try
                    {
                        system->getDevice();
                        enabled = true;
                    }
                    catch (const std::exception& e)
                    {
                        context->getSystem<LogSystem>()->print(
                            "ftk::gpu::System",
                            Format("Drawing with OpenGL instead: {0}").arg(e.what()),
                            LogType::Error);
                    }
                }
            }
        }
    }
}
