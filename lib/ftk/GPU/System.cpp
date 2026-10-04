// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GPU/System.h>

#include <ftk/GPU/Render.h>
#include <ftk/GPU/Shader.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>

#include <SDL3/SDL.h>

#include <cstdlib>
#include <map>
#include <stdexcept>

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
        {}

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

        bool isEnabled()
        {
            static const bool out = []
            {
                const char* env = std::getenv("FTK_RENDER");
                return env && std::string(env) == "gpu";
            }();
            return out;
        }

        void init(const std::shared_ptr<Context>& context)
        {
            if (!context->getSystem<System>())
            {
                context->addSystem(System::create(context));
            }
        }
    }
}
