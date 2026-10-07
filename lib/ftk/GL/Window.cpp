// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/Window.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/Init.h>
#include <ftk/GL/System.h>
#include <ftk/GL/Util.h>

#include <ftk/Core/Box.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>
#include <ftk/Core/String.h>

#if defined(FTK_SDL2)
#include <SDL2/SDL.h>
#elif defined(FTK_SDL3)
#include <SDL3/SDL.h>
#endif // FTK_SDL2

#if defined(__EMSCRIPTEN__)
#include <emscripten/html5.h>
#endif // __EMSCRIPTEN__

#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

namespace ftk
{
    namespace gl
    {
#if !defined(__EMSCRIPTEN__)
        namespace
        {
            // The usable area of a display: the display less the task bar,
            // the dock, or the panel.
            bool getUsableBounds(
#if defined(FTK_SDL2)
                int display,
#elif defined(FTK_SDL3)
                SDL_DisplayID display,
#endif // FTK_SDL2
                Box2I& out)
            {
                SDL_Rect rect;
#if defined(FTK_SDL2)
                const bool ok = 0 == SDL_GetDisplayUsableBounds(display, &rect);
#elif defined(FTK_SDL3)
                const bool ok = SDL_GetDisplayUsableBounds(display, &rect);
#endif // FTK_SDL2
                if (ok && rect.w > 0 && rect.h > 0)
                {
                    out = Box2I(rect.x, rect.y, rect.w, rect.h);
                }
                return ok && rect.w > 0 && rect.h > 0;
            }

            // Keep a window, its title bar included, on the usable area of
            // the display it is on: a size saved on a larger screen, or a
            // default made for a landscape one, would otherwise leave part
            // of it out of reach.
            void fitToDisplay(SDL_Window* sdlWindow)
            {
                Box2I bounds;
#if defined(FTK_SDL2)
                const int display = SDL_GetWindowDisplayIndex(sdlWindow);
#elif defined(FTK_SDL3)
                const SDL_DisplayID display = SDL_GetDisplayForWindow(sdlWindow);
#endif // FTK_SDL2
                if (!getUsableBounds(display, bounds))
                {
                    return;
                }

                // The frame, where the platform can say; zero where it
                // cannot yet, which leaves the frame to the window manager.
                int top = 0;
                int left = 0;
                int bottom = 0;
                int right = 0;
                SDL_GetWindowBordersSize(sdlWindow, &top, &left, &bottom, &right);

                Size2I size;
                SDL_GetWindowSize(sdlWindow, &size.w, &size.h);
                const Size2I maxSize(
                    std::max(bounds.w() - left - right, 1),
                    std::max(bounds.h() - top - bottom, 1));
                if (size.w > maxSize.w || size.h > maxSize.h)
                {
                    size.w = std::min(size.w, maxSize.w);
                    size.h = std::min(size.h, maxSize.h);
                    SDL_SetWindowSize(sdlWindow, size.w, size.h);
                }

                V2I pos;
                SDL_GetWindowPosition(sdlWindow, &pos.x, &pos.y);
                const V2I fitted(
                    std::clamp(
                        pos.x,
                        bounds.min.x + left,
                        std::max(bounds.max.x + 1 - right - size.w, bounds.min.x + left)),
                    std::clamp(
                        pos.y,
                        bounds.min.y + top,
                        std::max(bounds.max.y + 1 - bottom - size.h, bounds.min.y + top)));
                if (fitted != pos)
                {
                    SDL_SetWindowPosition(sdlWindow, fitted.x, fitted.y);
                }
            }
        }
#endif // __EMSCRIPTEN__

        struct Window::Private
        {
            std::weak_ptr<LogSystem> logSystem;
            SDL_Window* sdlWindow = nullptr;
            SDL_GLContext sdlGLContext = nullptr;
            GLInfo glInfo;
            V2I pos;
            std::vector<std::shared_ptr<Image> > icons;
            bool fullScreen = false;
            Size2I restoreSize;
            // The geometry when neither maximized nor full screen, what it
            // was before it was last noted, and when that was.
            Box2I normalGeometry;
            Box2I normalGeometryPrev;
            std::chrono::steady_clock::time_point normalGeometryTime;
            bool floatOnTop = false;
            bool textInput = false;
            // Whether the swap waits for the display, whether it is waiting
            // now, and when the frame being drawn was started: see swap().
            bool sync = false;
            bool syncCurrent = false;
            std::chrono::steady_clock::time_point frameTime;
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

            // Before the attributes: SDL 3 refuses them until the video
            // subsystem is up, and the window would then get whatever
            // context it defaults to. OpenGL is loaded only for a window
            // that has a context: one without is drawn by another renderer.
            const bool noContext = options & static_cast<int>(WindowOptions::NoContext);
            if (noContext)
            {
                context->getSystem<System>()->init();
            }
            else
            {
                context->getSystem<System>()->initGL();
            }
            uint32_t sdlWindowFlags = SDL_WINDOW_RESIZABLE;
            if (!noContext)
            {
                SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
                const bool doubleBuffer = options & static_cast<int>(WindowOptions::DoubleBuffer);
                SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, doubleBuffer);
                setContextAttributes(getAPI());
                sdlWindowFlags |= SDL_WINDOW_OPENGL;
            }
#if defined(FTK_SDL2)
            sdlWindowFlags |= SDL_WINDOW_ALLOW_HIGHDPI;
#elif defined(FTK_SDL3)
            sdlWindowFlags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;
            if (options & static_cast<int>(WindowOptions::Vulkan))
            {
                sdlWindowFlags |= SDL_WINDOW_VULKAN;
            }
#endif // FTK_SDL2
            if (options & static_cast<int>(WindowOptions::Visible))
            {
#if defined(FTK_SDL2)
                sdlWindowFlags |= SDL_WINDOW_SHOWN;
#endif // FTK_SDL2
            }
            else
            {
                sdlWindowFlags |= SDL_WINDOW_HIDDEN;
            }
            Size2I windowSize = size;
#if defined(__EMSCRIPTEN__)
            // The canvas fills the page, and the page belongs to the
            // browser: its size wins over the size the caller asked for.
            double cssW = 0.0;
            double cssH = 0.0;
            emscripten_get_element_css_size("#canvas", &cssW, &cssH);
            if (cssW > 0.0 && cssH > 0.0)
            {
                windowSize.w = cssW;
                windowSize.h = cssH;
            }
#else // __EMSCRIPTEN__
            // No larger than the primary display's usable area, which is
            // where a window with no position of its own opens; the display
            // it does land on is checked again below.
            {
                Box2I bounds;
#if defined(FTK_SDL2)
                const bool ok = getUsableBounds(0, bounds);
#elif defined(FTK_SDL3)
                const bool ok = getUsableBounds(SDL_GetPrimaryDisplay(), bounds);
#endif // FTK_SDL2
                if (ok)
                {
                    windowSize.w = std::min(windowSize.w, bounds.w());
                    windowSize.h = std::min(windowSize.h, bounds.h());
                }
            }
#endif // __EMSCRIPTEN__
            p.sdlWindow = SDL_CreateWindow(
                title.c_str(),
#if defined(FTK_SDL2)
                SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED,
#endif // FTK_SDL2
                windowSize.w,
                windowSize.h,
                sdlWindowFlags);
            if (!p.sdlWindow)
            {
                throw std::runtime_error(Format("Cannot create window: {0}").
                    arg(SDL_GetError()));
            }
            SDL_SetWindowMinimumSize(p.sdlWindow, 320, 240);
#if defined(FTK_SDL2)
            // SDL2 starts with text input on, which routes every keystroke
            // through the input method even when nothing is being edited: a
            // shortcut on a dead key (Option+N on macOS) leaves a composition
            // pending, and it commits into whatever is focused later. Text
            // input runs only while an editor asks for it.
            SDL_StopTextInput();
#endif // FTK_SDL2

#if defined(__EMSCRIPTEN__)
            // The browser does not tell SDL when the page changes size,
            // so follow it by hand; the new size then arrives through
            // the normal SDL resize events.
            emscripten_set_resize_callback(
                EMSCRIPTEN_EVENT_TARGET_WINDOW,
                p.sdlWindow,
                EM_FALSE,
                [](int, const EmscriptenUiEvent*, void* userData) -> EM_BOOL
                {
                    double cssW = 0.0;
                    double cssH = 0.0;
                    emscripten_get_element_css_size("#canvas", &cssW, &cssH);
                    if (cssW > 0.0 && cssH > 0.0)
                    {
                        SDL_SetWindowSize(
                            static_cast<SDL_Window*>(userData),
                            cssW,
                            cssH);
                    }
                    return EM_TRUE;
                });
#endif // __EMSCRIPTEN__

            if (!noContext)
            {
                p.sdlGLContext = SDL_GL_CreateContext(p.sdlWindow);
                if (!p.sdlGLContext)
                {
                    throw std::runtime_error(Format("Cannot create OpenGL context: {0}").
                        arg(SDL_GetError()));
                }
#if !defined(__EMSCRIPTEN__)
                // The browser paces frames itself, and asking SDL to set a
                // swap interval before the main loop exists only produces a
                // warning.
                if (options & static_cast<int>(WindowOptions::DoubleBuffer))
                {
                    SDL_GL_SetSwapInterval(1);
                    p.sync = true;
                    p.syncCurrent = true;
                }
#endif // __EMSCRIPTEN__

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

                int glVersionMajor = 0;
                if (const GLubyte* glString = glGetString(GL_VENDOR))
                {
                    p.glInfo.vendor = std::string((const char*)glString);
                }
                if (const GLubyte* glString = glGetString(GL_RENDERER))
                {
                    p.glInfo.renderer = std::string((const char*)glString);
                }
                if (const GLubyte* glString = glGetString(GL_VERSION))
                {
                    p.glInfo.version = std::string((const char*)glString);
                    glVersionMajor = getMajorVersion(p.glInfo.version);
                }
                //! \todo Shouldn't window creation fail if we didn't get the
                //! requested OpenGL version?
                if (glVersionMajor < (isGLES() ? 3 : 4))
                {
                    throw std::runtime_error(Format("Unsupported OpenGL version: {0}").
                        arg(glVersionMajor));
                }

            }

            if (auto logSystem = p.logSystem.lock())
            {
                Size2I actualSize;
                SDL_GetWindowSize(p.sdlWindow, &actualSize.w, &actualSize.h);
                Size2I frameBufferSize;
#if defined(FTK_SDL2)
                SDL_GL_GetDrawableSize(p.sdlWindow, &frameBufferSize.w, &frameBufferSize.h);
#elif defined(FTK_SDL3)
                SDL_GetWindowSizeInPixels(p.sdlWindow, &frameBufferSize.w, &frameBufferSize.h);
#endif // FTK_SDL2
                std::string message = Format(
                    "New window: {0}\n"
                    "    * Requested size: {1}\n"
                    "    * Actual size: {2}\n"
                    "    * Framebuffer size: {3}").
                    arg(this).
                    arg(size).
                    arg(actualSize).
                    arg(frameBufferSize);
                // A window without a context has nothing to say of OpenGL.
                if (!noContext)
                {
                    message += Format(
                        "\n"
                        "    * OpenGL vendor: {0}\n"
                        "    * OpenGL renderer: {1}\n"
                        "    * OpenGL version: {2}").
                        arg(p.glInfo.vendor).
                        arg(p.glInfo.renderer).
                        arg(p.glInfo.version);
                }
                logSystem->print("ftk::gl::Window", message);
            }

            if (!(options & static_cast<int>(WindowOptions::MakeCurrent)))
            {
                clearCurrent();
            }

#if !defined(__EMSCRIPTEN__)
            fitToDisplay(p.sdlWindow);
#endif // __EMSCRIPTEN__
            geometryChanged();
            p.normalGeometryPrev = p.normalGeometry;
            p.normalGeometryTime = std::chrono::steady_clock::time_point();
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
#if defined(__EMSCRIPTEN__)
            emscripten_set_resize_callback(
                EMSCRIPTEN_EVENT_TARGET_WINDOW,
                nullptr,
                EM_FALSE,
                nullptr);
#endif // __EMSCRIPTEN__
            if (p.sdlGLContext)
            {
#if defined(FTK_SDL2)
                SDL_GL_DeleteContext(p.sdlGLContext);
#elif defined(FTK_SDL3)
                SDL_GL_DestroyContext(p.sdlGLContext);
#endif // FTK_SDL2
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

    float Window::getDisplayScale() const
    {
        FTK_P();
        float out = 0.F;
#if defined(FTK_SDL3)
        if (p.sdlWindow)
        {
            out = SDL_GetWindowDisplayScale(p.sdlWindow);
        }
#endif // FTK_SDL3
        return out;
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
#if !defined(__EMSCRIPTEN__)
            fitToDisplay(_p->sdlWindow);
#endif // __EMSCRIPTEN__
            geometryChanged();
        }

        V2I Window::getPos() const
        {
            V2I out;
            SDL_GetWindowPosition(_p->sdlWindow, &out.x, &out.y);
            return out;
        }

        void Window::setPos(const V2I& value)
        {
            SDL_SetWindowPosition(_p->sdlWindow, value.x, value.y);
#if !defined(__EMSCRIPTEN__)
            fitToDisplay(_p->sdlWindow);
#endif // __EMSCRIPTEN__
            geometryChanged();
        }

        bool Window::isMaximized() const
        {
            return SDL_GetWindowFlags(_p->sdlWindow) & SDL_WINDOW_MAXIMIZED;
        }

        void Window::setMaximized(bool value)
        {
            if (value)
            {
                SDL_MaximizeWindow(_p->sdlWindow);
            }
            else if (isMaximized())
            {
                SDL_RestoreWindow(_p->sdlWindow);
            }
        }

        Box2I Window::getNormalGeometry() const
        {
            return _p->normalGeometry;
        }

        void Window::geometryChanged(bool maximized)
        {
            FTK_P();
            if (maximized)
            {
                // A platform can say the window has been resized before it
                // says it has been maximized, and the maximized size is then
                // noted as the normal one. A normal size noted a moment
                // before the window is maximized is that: put back what it
                // was.
                const auto now = std::chrono::steady_clock::now();
                if (now - p.normalGeometryTime < std::chrono::milliseconds(500) &&
                    p.normalGeometry.size() != p.normalGeometryPrev.size())
                {
                    p.normalGeometry = p.normalGeometryPrev;
                }
                return;
            }
            V2I pos;
            Size2I size;
            SDL_GetWindowPosition(p.sdlWindow, &pos.x, &pos.y);
            SDL_GetWindowSize(p.sdlWindow, &size.w, &size.h);
            const Box2I geometry(pos, size);
            const auto flags = SDL_GetWindowFlags(p.sdlWindow);
            if (p.fullScreen ||
                (flags & SDL_WINDOW_FULLSCREEN) ||
                (flags & SDL_WINDOW_MINIMIZED))
            {
                return;
            }
            if (flags & SDL_WINDOW_MAXIMIZED)
            {
                return;
            }
            if (geometry != p.normalGeometry)
            {
                p.normalGeometryPrev = p.normalGeometry;
                p.normalGeometry = geometry;
                p.normalGeometryTime = std::chrono::steady_clock::now();
            }
        }

        Size2I Window::getSize() const
    {
        FTK_P();
        Size2I out;
        if (p.sdlWindow)
        {
            SDL_GetWindowSize(p.sdlWindow, &out.w, &out.h);
        }
        return out;
    }

    Size2I Window::getFrameBufferSize() const
    {
        FTK_P();
        Size2I out;
        if (p.sdlWindow)
        {
#if defined(FTK_SDL2)
            SDL_GL_GetDrawableSize(p.sdlWindow, &out.w, &out.h);
#elif defined(FTK_SDL3)
            SDL_GetWindowSizeInPixels(p.sdlWindow, &out.w, &out.h);
#endif // FTK_SDL2
        }
        return out;
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
            if (!icon)
                return;
            const ImageInfo& info = icon->getInfo();
            if (info.type == ImageType::RGBA_U8 &&
                1 == info.layout.alignment)
            {
                auto mirrored = Image::create(info);
                for (int y = 0; y < info.size.h; ++y)
                {
                    memcpy(
                        mirrored->getData() + (info.size.h - 1 - y) * info.size.w * 4,
                        icon->getData() + y * info.size.w * 4,
                        info.size.w * 4);
                }
#if defined(FTK_SDL2)
                if (SDL_Surface* sdlSurface = SDL_CreateRGBSurfaceFrom(
                    mirrored->getData(),
                    info.size.w,
                    info.size.h,
                    32,
                    info.size.w * 4,
                    0x000000ff,
                    0x0000ff00,
                    0x00ff0000,
                    0xff000000))
#elif defined(FTK_SDL3)
                // RGBA32, not RGBA8888: the 8888 formats are packed 32-bit
                // words, so on a little-endian machine RGBA8888 reads the
                // bytes as ABGR and the icon comes out red-shifted. RGBA32
                // is the byte-order alias that matches the masks the SDL2
                // path uses.
                if (SDL_Surface* sdlSurface = SDL_CreateSurfaceFrom(
                    info.size.w,
                    info.size.h,
                    SDL_PIXELFORMAT_RGBA32,
                    mirrored->getData(),
                    info.size.w * 4))
#endif // FTK_SDL2
                {
                    SDL_SetWindowIcon(_p->sdlWindow, sdlSurface);
#if defined(FTK_SDL2)
                    SDL_FreeSurface(sdlSurface);
#elif defined(FTK_SDL3)
                    SDL_DestroySurface(sdlSurface);
#endif // FTK_SDL2
                }
            }
        }

        void Window::makeCurrent()
        {
            FTK_P();
            if (!p.sdlGLContext)
                return;
            p.frameTime = std::chrono::steady_clock::now();
#if defined(FTK_SDL2)
            if (SDL_GL_MakeCurrent(p.sdlWindow, p.sdlGLContext) < 0)
#elif defined(FTK_SDL3)
            if (!SDL_GL_MakeCurrent(p.sdlWindow, p.sdlGLContext))
#endif // FTK_SDL2
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
            if (!p.sdlGLContext)
                return;
#if defined(FTK_SDL2)
            if (SDL_GL_MakeCurrent(p.sdlWindow, nullptr) < 0)
#elif defined(FTK_SDL3)
            if (!SDL_GL_MakeCurrent(p.sdlWindow, nullptr))
#endif // FTK_SDL2
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
#if defined(FTK_SDL2)
                SDL_SetWindowFullscreen(p.sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
#elif defined(FTK_SDL3)
                SDL_SetWindowFullscreen(p.sdlWindow, 1);
#endif // FTK_SDL2
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

        void Window::raise()
        {
            SDL_RaiseWindow(_p->sdlWindow);
        }

        void Window::setFloatOnTop(bool value)
        {
            FTK_P();
            if (value == p.floatOnTop)
                return;
            p.floatOnTop = value;
#if defined(FTK_SDL2)
            SDL_SetWindowAlwaysOnTop(p.sdlWindow, value ? SDL_TRUE : SDL_FALSE);
#elif defined(FTK_SDL3)
            SDL_SetWindowAlwaysOnTop(p.sdlWindow, value ? true : false);
#endif // FTK_SDL2
        }

        bool Window::hasTextInput() const
        {
            return _p->textInput;
        }

        void Window::setTextInput(bool value)
        {
            FTK_P();
            if (value == p.textInput)
                return;
            p.textInput = value;
#if defined(FTK_SDL2)
            // Global in SDL2; there is only one text focus anyway.
            if (value)
            {
                SDL_StartTextInput();
            }
            else
            {
                SDL_StopTextInput();
            }
#elif defined(FTK_SDL3)
            if (value)
            {
                SDL_StartTextInput(p.sdlWindow);
            }
            else
            {
                SDL_StopTextInput(p.sdlWindow);
            }
#endif // FTK_SDL2
        }

        void Window::setTextInputArea(const Box2I& value)
        {
            FTK_P();
            // The box arrives in framebuffer coordinates and SDL wants
            // window coordinates, which differ on a high DPI display.
            int windowW = 0;
            int windowH = 0;
            int bufferW = 0;
            int bufferH = 0;
            SDL_GetWindowSize(p.sdlWindow, &windowW, &windowH);
#if defined(FTK_SDL2)
            SDL_GL_GetDrawableSize(p.sdlWindow, &bufferW, &bufferH);
#elif defined(FTK_SDL3)
            SDL_GetWindowSizeInPixels(p.sdlWindow, &bufferW, &bufferH);
#endif // FTK_SDL2
            const float scale =
                bufferW > 0 && windowW > 0 ?
                windowW / static_cast<float>(bufferW) :
                1.F;
            const SDL_Rect rect =
            {
                static_cast<int>(value.x() * scale),
                static_cast<int>(value.y() * scale),
                static_cast<int>(value.w() * scale),
                static_cast<int>(value.h() * scale)
            };
#if defined(FTK_SDL2)
            SDL_SetTextInputRect(&rect);
#elif defined(FTK_SDL3)
            SDL_SetTextInputArea(p.sdlWindow, &rect, 0);
#endif // FTK_SDL2
        }

        void Window::swap()
        {
            FTK_P();
            if (!p.sdlGLContext)
                return;
#if defined(__linux__)
            // A window behind another is not shown, and a driver can hold
            // its drawing back for as long as that lasts: with NVIDIA's on
            // X11, half a second a frame for a window that is covered. That
            // is the application's main loop, and so playback, held up by a
            // window that is not even visible. So a window without the
            // focus whose frame took that long stops waiting for the
            // display, until it has the focus again. Only then: a window
            // that is drawing on time is left as it is, on every other
            // driver and desktop.
            if (p.sync)
            {
                const bool focus =
                    (SDL_GetWindowFlags(p.sdlWindow) & SDL_WINDOW_INPUT_FOCUS) != 0;
                const auto frame = std::chrono::steady_clock::now() - p.frameTime;
                bool sync = p.syncCurrent;
                if (focus)
                {
                    sync = true;
                }
                else if (frame > std::chrono::milliseconds(100))
                {
                    sync = false;
                }
                if (sync != p.syncCurrent)
                {
                    p.syncCurrent = sync;
                    SDL_GL_SetSwapInterval(sync ? 1 : 0);
                    if (auto logSystem = p.logSystem.lock())
                    {
                        logSystem->print(
                            "ftk::gl::Window",
                            sync ?
                            "Swap waits for the display" :
                            "Swap does not wait for the display: the window "
                            "is behind another and its drawing was held back");
                    }
                }
            }
#endif // __linux__
            SDL_GL_SwapWindow(p.sdlWindow);
        }

        const GLInfo& Window::getGLInfo() const
        {
            return _p->glInfo;
        }

        SDL_Window* Window::getSDLWindow() const
        {
            return _p->sdlWindow;
        }
    }
}
