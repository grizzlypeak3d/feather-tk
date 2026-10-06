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
#include <ftk/Core/OS.h>
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
            // Which build of the library is actually running: the OpenGL
            // system says the same of its own, and the two can come from
            // different places, the build tree and an install.
            const std::string libraryInfo = getLibraryInfo(reinterpret_cast<const void*>(&System::create));
            if (!libraryInfo.empty())
            {
                context->getLogSystem()->print("ftk::gpu::System", libraryInfo);
            }

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
                const bool debug = getEnvFlag("FTK_GPU_DEBUG");
                const SDL_PropertiesID props = SDL_CreateProperties();
                SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_MSL_BOOLEAN, true);
                SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_SHADERS_SPIRV_BOOLEAN, true);
                SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_DEBUGMODE_BOOLEAN, debug);
                // What SDL asks of a Vulkan device unless told otherwise,
                // and nothing here uses: a device without one of them, a
                // small one, would be refused for it. Without depth
                // clamping every pipeline has to clip by depth instead,
                // which they are made to.
                SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_CLIP_DISTANCE_BOOLEAN, false);
                SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_DEPTH_CLAMPING_BOOLEAN, false);
                SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_INDIRECT_DRAW_FIRST_INSTANCE_BOOLEAN, false);
                SDL_SetBooleanProperty(props, SDL_PROP_GPU_DEVICE_CREATE_FEATURE_ANISOTROPY_BOOLEAN, false);
                // Not a device that draws on the CPU, which Vulkan offers
                // where Mesa's is installed and there is no other: it is
                // slow, and the OpenGL renderer is there to draw instead,
                // perhaps with a GPU that has no driver for Vulkan.
                // FTK_GPU_SOFTWARE takes one all the same, to test on a
                // machine without a GPU.
                SDL_SetBooleanProperty(
                    props,
                    SDL_PROP_GPU_DEVICE_CREATE_VULKAN_REQUIRE_HARDWARE_ACCELERATION_BOOLEAN,
                    !getEnvFlag("FTK_GPU_SOFTWARE"));
                p.device = SDL_CreateGPUDeviceWithProperties(props);
                SDL_DestroyProperties(props);
                if (!p.device)
                {
                    throw std::runtime_error(Format("Cannot create a GPU device: {0}").arg(SDL_GetError()));
                }
                // A device that takes SPIR-V and a build with nothing to
                // make it with: glslang was not found when this was built.
                // Said now, while there is another renderer to draw with,
                // rather than by the first shader.
                if (!(SDL_GetGPUShaderFormats(p.device) & SDL_GPU_SHADERFORMAT_MSL) &&
                    !hasGLSLCompiler())
                {
                    const std::string driver = SDL_GetGPUDeviceDriver(p.device);
                    SDL_DestroyGPUDevice(p.device);
                    p.device = nullptr;
                    throw std::runtime_error(
                        Format("This build has no shader compiler for {0}: it was built without glslang").
                            arg(driver));
                }
                // The driver, and which device and system driver where
                // they are told: a machine with two cards, or a report of
                // something that only one driver does, is no use without.
                for (const auto& i : getInfo())
                {
                    _log(Format("{0}: {1}").arg(i.first).arg(i.second));
                }
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

        std::vector<std::pair<std::string, std::string> > System::getInfo()
        {
            std::vector<std::pair<std::string, std::string> > out;
            SDL_GPUDevice* device = getDevice();
            out.push_back(std::make_pair("GPU driver", SDL_GetGPUDeviceDriver(device)));
            // Told or not as the driver pleases, and in no form to rely on.
            const SDL_PropertiesID props = SDL_GetGPUDeviceProperties(device);
            const auto add = [&out, props](const std::string& label, const char* name)
            {
                const char* value = SDL_GetStringProperty(props, name, nullptr);
                if (value && value[0])
                {
                    out.push_back(std::make_pair(label, value));
                }
            };
            add("GPU device", SDL_PROP_GPU_DEVICE_NAME_STRING);
            add("GPU system driver", SDL_PROP_GPU_DEVICE_DRIVER_NAME_STRING);
            // The longer of the two versions where there is one: it says
            // the same and more.
            const char* info = SDL_GetStringProperty(props, SDL_PROP_GPU_DEVICE_DRIVER_INFO_STRING, nullptr);
            add(
                "GPU system driver version",
                info && info[0] ?
                    SDL_PROP_GPU_DEVICE_DRIVER_INFO_STRING :
                    SDL_PROP_GPU_DEVICE_DRIVER_VERSION_STRING);
            return out;
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

        bool getEnvFlag(const char* name)
        {
            const char* env = std::getenv(name);
            return env && env[0] && std::string("0") != env;
        }

        namespace
        {
            bool started = false;
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
                context->addSystem(System::create(context));
                // Another context is another run, as far as what draws
                // goes: a test makes one after another.
                started = false;
                enabled = false;
            }
        }

        bool isRequested()
        {
            const char* env = std::getenv("FTK_RENDER");
            return env && std::string("gpu") == env;
        }

        bool start(const std::shared_ptr<Context>& context)
        {
            init(context);
            if (!started)
            {
                started = true;

                // This renderer draws only where there is a device to draw
                // with. Finding that out is making one, which is done here
                // rather than by the first window, so that what cannot be
                // had is known while there is still another renderer to
                // choose.
                try
                {
                    auto system = context->getSystem<System>();
                    system->getDevice();
                    enabled = true;
                    // Beside the texture formats the device says it has:
                    // what it says nothing of, and is tried.
                    context->getSystem<LogSystem>()->print(
                        "ftk::gpu::System",
                        Format("Float texture filtering: {0}").
                            arg(hasFloatFilter(system) ?
                                "supported" :
                                "not supported, float textures are kept as half float"));
                }
                catch (const std::exception& e)
                {
                    // A warning: it was asked for and is not had, and
                    // what draws instead is whole.
                    context->getSystem<LogSystem>()->print(
                        "ftk::gpu::System",
                        Format("Drawing with OpenGL instead: {0}").arg(e.what()),
                        LogType::Warning);
                }
            }
            return enabled;
        }
    }
}
