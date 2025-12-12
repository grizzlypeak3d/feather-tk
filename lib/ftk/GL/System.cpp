// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/System.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/Render.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>
#include <ftk/Core/String.h>

#if defined(FTK_SDL2)
#include <SDL2/SDL.h>
#elif defined(FTK_SDL3)
#include <SDL3/SDL.h>
#endif // FTK_SDL2

#include <iostream>

namespace ftk
{
    namespace gl
    {
        namespace
        {
            void logOutput(void *userData, int category, SDL_LogPriority priority, const char *message)
            {
                if (userData)
                {
                    if (auto context = ((System*)userData)->getContext())
                    {
                        auto logSystem = context->getLogSystem();
                        logSystem->print("SDL", message, LogType::Message);
                    }
                }
            }
        }

        struct System::Private
        {
            std::weak_ptr<LogSystem> logSystem;
            std::shared_ptr<IRenderFactory> renderFactory;
        };
        
        System::System(const std::shared_ptr<Context>& context) :
            ISystem(context, "ftk::gl::System"),
            _p(new Private)
        {
            FTK_P();

            // Initialize SDL.
            auto logSystem = context->getLogSystem();
            logSystem->print("ftk::gl::System", "Init SDL video and events...");
            p.logSystem = logSystem;
#if defined(FTK_SDL2)
            SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");
#endif // FTK_SDL2
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
            {
                throw std::runtime_error(Format("Cannot initialize SDL: {0}").
                    arg(SDL_GetError()));
            }
            if (SDL_GL_LoadLibrary(NULL) < 0)
            {
                throw std::runtime_error(Format("Cannot initialize OpenGL: {0}").
                    arg(SDL_GetError()));
            }
#if defined(FTK_SDL2)
            SDL_LogSetOutputFunction(logOutput, this);
#elif defined(FTK_SDL3)
            SDL_SetLogOutputFunction(logOutput, this);
#endif // FTK_SDL2

            // Create default render factory.
            p.renderFactory = std::make_shared<RenderFactory>();
        }

        System::~System()
        {
            FTK_P();
            if (auto logSystem = p.logSystem.lock())
            {
                logSystem->print("ftk::gl::System", "Quit SDL...");
            }
#if defined(FTK_SDL2)
            SDL_LogSetOutputFunction(nullptr, nullptr);
#elif defined(FTK_SDL3)
            SDL_SetLogOutputFunction(nullptr, nullptr);
#endif // FTK_SDL2
            SDL_Quit();
        }

        std::shared_ptr<System> System::create(const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<System>(new System(context));
        }

        const std::shared_ptr<IRenderFactory>& System::getRenderFactory() const
        {
            return _p->renderFactory;
        }

        void System::setRenderFactory(const std::shared_ptr<IRenderFactory>& value)
        {
            _p->renderFactory = value;
        }
    }
}
