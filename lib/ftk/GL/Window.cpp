// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/Window.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/Init.h>
#include <ftk/GL/Util.h>

#include <ftk/Core/Box.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>
#include <ftk/Core/String.h>

#include <SDL2/SDL.h>

#include <algorithm>
#include <iostream>
#include <vector>

namespace ftk
{
    namespace gl
    {
        struct Window::Private
        {
            std::weak_ptr<LogSystem> logSystem;
            SDL_Window* sdlWindow = nullptr;
            SDL_GLContext sdlGLContext = nullptr;
            V2I pos;
            std::vector<std::shared_ptr<Image> > icons;
            bool fullScreen = false;
            Size2I restoreSize;
            bool floatOnTop = false;
        };
        
        Window::Window(
            const std::shared_ptr<Context>& context,
            const std::string& title,
            const Size2I& size,
            int options,
            const std::shared_ptr<Window>& share) :
            _p(new Private)
        {
            FTK_P();

            p.logSystem = context->getLogSystem();

            SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
            const bool doubleBuffer = options & static_cast<int>(WindowOptions::DoubleBuffer);
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, doubleBuffer);
#if defined(FTK_API_GL_4_1)
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
            const int glProfile =
#if defined(__APPLE__)
                SDL_GL_CONTEXT_PROFILE_CORE;
#else // __APPLE__
                SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
#endif // __APPLE__
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, glProfile);
#elif defined(FTK_API_GLES_2)
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif // FTK_API_GL_4_1
            uint32_t sdlWindowFlags =
                SDL_WINDOW_OPENGL |
                SDL_WINDOW_RESIZABLE |
                SDL_WINDOW_ALLOW_HIGHDPI;
            if (options & static_cast<int>(WindowOptions::Visible))
            {
                sdlWindowFlags |= SDL_WINDOW_SHOWN;
            }
            else
            {
                sdlWindowFlags |= SDL_WINDOW_HIDDEN;
            }
            p.sdlWindow = SDL_CreateWindow(
                title.c_str(),
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
                size.w,
                size.h,
                sdlWindowFlags);
            if (!p.sdlWindow)
            {
                throw std::runtime_error(Format("Cannot create window: {0}").
                    arg(SDL_GetError()));
            }
            SDL_SetWindowMinimumSize(p.sdlWindow, 320, 240);

            p.sdlGLContext = SDL_GL_CreateContext(p.sdlWindow);
            if (!p.sdlGLContext)
            {
                throw std::runtime_error(Format("Cannot create OpenGL context: {0}").
                    arg(SDL_GetError()));
            }
            if (options & static_cast<int>(WindowOptions::DoubleBuffer))
            {
                SDL_GL_SetSwapInterval(1);
            }

            initGLAD();
#if defined(FTK_API_GL_4_1_Debug)
            GLint flags = 0;
            glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
            if (flags & static_cast<GLint>(GL_CONTEXT_FLAG_DEBUG_BIT))
            {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(glDebugOutput, nullptr);
                glDebugMessageControl(
                    static_cast<GLenum>(GL_DONT_CARE),
                    static_cast<GLenum>(GL_DONT_CARE),
                    static_cast<GLenum>(GL_DONT_CARE),
                    0,
                    nullptr,
                    GL_TRUE);
            }
#endif // FTK_API_GL_4_1_Debug

            std::string glVendor;
            std::string glRenderer;
            std::string glVersion;
            int glVersionMajor = 0;
            if (const GLubyte* glString = glGetString(GL_VENDOR))
            {
                glVendor = std::string((const char*)glString);
            }
            if (const GLubyte* glString = glGetString(GL_RENDERER))
            {
                glRenderer = std::string((const char*)glString);
            }
            if (const GLubyte* glString = glGetString(GL_VERSION))
            {
                glVersion = std::string((const char*)glString);
                glVersionMajor = getMajorVersion(glVersion);
            }
            //! \todo Shouldn't window creation fail if we didn't get the
            //! requested OpenGL version?
#if defined(FTK_API_GL_4_1)
            if (glVersionMajor < 4)
#elif defined(FTK_API_GLES_2)
            if (glVersionMajor < 2)
#endif // FTK_API_GL_4_1
            {
                throw std::runtime_error(Format("Unsupported OpenGL version: {0}").
                    arg(glVersionMajor));
            }

            if (auto logSystem = p.logSystem.lock())
            {
                logSystem->print(
                    "ftk::gl::Window",
                    Format(
                        "New window {0}:\n"
                        "    Size: {1}\n"
                        "    OpenGL vendor: {2}\n"
                        "    OpenGL renderer: {3}\n"
                        "    OpenGL version: {4}").
                    arg(this).
                    arg(size).
                    arg(glVendor).
                    arg(glRenderer).
                    arg(glVersion));
            }

            if (!(options & static_cast<int>(WindowOptions::MakeCurrent)))
            {
                clearCurrent();
            }
        }
        
        Window::~Window()
        {
            FTK_P();
            if (auto logSystem = p.logSystem.lock())
            {
                logSystem->print(
                    "ftk::gl::Window",
                    Format("Destroy window {0}...").arg(this));
            }
            if (p.sdlGLContext)
            {
                SDL_GL_DeleteContext(p.sdlGLContext);
            }
            if (p.sdlWindow)
            {
                SDL_DestroyWindow(p.sdlWindow);
            }
        }

        std::shared_ptr<Window> Window::create(
            const std::shared_ptr<Context>& context,
            const std::string& title,
            const Size2I& size,
            int options,
            const std::shared_ptr<Window>& share)
        {
            return std::shared_ptr<Window>(new Window(context, title, size, options, share));
        }

        uint32_t Window::getID() const
        {
            return SDL_GetWindowID(_p->sdlWindow);
        }

        std::string Window::getTitle() const
        {
            return SDL_GetWindowTitle(_p->sdlWindow);
        }

        void Window::setTitle(const std::string& value)
        {
            SDL_SetWindowTitle(_p->sdlWindow, value.c_str());
        }

        void Window::setSize(const Size2I& value)
        {
            setFullScreen(false);
            SDL_SetWindowSize(_p->sdlWindow, value.w, value.h);
        }

        Size2I Window::getMinSize() const
        {
            Size2I out;
            SDL_GetWindowMinimumSize(_p->sdlWindow, &out.w, &out.h);
            return out;
        }

        void Window::setMinSize(const Size2I& value)
        {
            SDL_SetWindowMinimumSize(_p->sdlWindow, value.w, value.h);
        }

        void Window::show()
        {
            SDL_ShowWindow(_p->sdlWindow);
        }

        void Window::hide()
        {
            setFullScreen(false);
            SDL_HideWindow(_p->sdlWindow);
        }

        void Window::setIcon(const std::shared_ptr<Image>& icon)
        {
            const ImageInfo& info = icon->getInfo();
            if (info.type == ImageType::RGBA_U8 &&
                1 == info.layout.alignment)
            {
                auto flipped = Image::create(info);
                for (int y = 0; y < info.size.h; ++y)
                {
                    memcpy(
                        flipped->getData() + (info.size.h - 1 - y) * info.size.w * 4,
                        icon->getData() + y * info.size.w * 4,
                        info.size.w * 4);
                }
                if (SDL_Surface* sdlSurface = SDL_CreateRGBSurfaceFrom(
                    flipped->getData(),
                    info.size.w,
                    info.size.h,
                    32,
                    info.size.w * 4,
                    0x000000ff,
                    0x0000ff00,
                    0x00ff0000,
                    0xff000000))
                {
                    SDL_SetWindowIcon(_p->sdlWindow, sdlSurface);
                    SDL_FreeSurface(sdlSurface);
                }
            }
        }

        void Window::makeCurrent()
        {
            FTK_P();
            if (SDL_GL_MakeCurrent(p.sdlWindow, p.sdlGLContext) < 0)
            {
                if (auto logSystem = p.logSystem.lock())
                {
                    logSystem->print(
                        "ftk::gl::Window",
                        Format("Cannot make context current: {0}").arg(SDL_GetError()),
                        LogType::Error);
                }
            }
        }

        void Window::clearCurrent()
        {
            FTK_P();
            if (SDL_GL_MakeCurrent(p.sdlWindow, nullptr) < 0)
            {
                if (auto logSystem = p.logSystem.lock())
                {
                    logSystem->print(
                        "ftk::gl::Window",
                        Format("Cannot make context done: {0}").arg(SDL_GetError()),
                        LogType::Error);
                }
            }
        }

        int Window::getScreen() const
        {
            return 0;
        }

        bool Window::isFullScreen() const
        {
            return _p->fullScreen;
        }

        void Window::setFullScreen(bool value)
        {
            FTK_P();
            if (value == p.fullScreen)
                return;
            p.fullScreen = value;
            if (p.fullScreen)
            {
                SDL_GetWindowPosition(p.sdlWindow, &p.pos.x, &p.pos.y);
                SDL_GetWindowSize(p.sdlWindow, &p.restoreSize.w, &p.restoreSize.h);
                SDL_SetWindowFullscreen(p.sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
            }
            else
            {
                SDL_SetWindowFullscreen(p.sdlWindow, 0);
                SDL_SetWindowPosition(p.sdlWindow, p.pos.x, p.pos.y);
                SDL_SetWindowSize(p.sdlWindow, p.restoreSize.w, p.restoreSize.h);
            }
        }

        bool Window::isFloatOnTop() const
        {
            return _p->floatOnTop;
        }

        void Window::setFloatOnTop(bool value)
        {
            FTK_P();
            if (value == p.floatOnTop)
                return;
            p.floatOnTop = value;
        }

        void Window::swap()
        {
            SDL_GL_SwapWindow(_p->sdlWindow);
        }
    }
}
