// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/System.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/Render.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>
#include <ftk/Core/String.h>

#include <SDL2/SDL.h>

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
                    if (auto context = ((System*)userData)->getContext().lock())
                    {
                        auto logSystem = context->getLogSystem();
                        logSystem->print("SDL", message, LogType::Message);
                    }
                }
            }
        }

        struct System::Private
        {
            std::shared_ptr<IRenderFactory> renderFactory;
        };
        
        System::System(const std::shared_ptr<Context>& context) :
            ISystem(context, "ftk::gl::System"),
            _p(new Private)
        {
            FTK_P();

            // Initialize SDL.
            SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");
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
            //SDL_LogSetOutputFunction(logOutput, this);

            // Create default render factory.
            p.renderFactory = std::make_shared<RenderFactory>();
        }

        System::~System()
        {
            //SDL_LogSetOutputFunction(nullptr, nullptr);
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
