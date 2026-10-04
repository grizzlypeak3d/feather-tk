// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GL/Export.h>
#include <ftk/Core/Box.h>
#include <ftk/Core/Image.h>

struct SDL_Window;

namespace ftk
{
    class Context;

    namespace gl
    {
        //! \name Windows
        ///@{
        
        //! OpenGL information.
        struct FTK_GL_API_TYPE GLInfo
        {
            std::string vendor;
            std::string renderer;
            std::string version;
        };

        //! OpenGL window options.
        enum class FTK_GL_API_TYPE WindowOptions
        {
            None         = 0,
            Visible      = 1,
            DoubleBuffer = 2,
            MakeCurrent  = 4,
            //! No OpenGL context: the window is drawn to with something
            //! else, and making it current or swapping it does nothing.
            NoContext    = 8
        };

        //! OpenGL window.
        class FTK_GL_API_TYPE Window : public std::enable_shared_from_this<Window>
        {
            FTK_NON_COPYABLE(Window);

        protected:
            FTK_GL_API Window(
                const std::shared_ptr<Context>&,
                const std::string& title,
                const Size2I&,
                int options,
                const std::shared_ptr<Window>& share);

        public:
            FTK_GL_API virtual ~Window();

            //! Create a new window.
            FTK_GL_API static std::shared_ptr<Window> create(
                const std::shared_ptr<Context>&,
                const std::string& title,
                const Size2I&,
                int options =
                    static_cast<int>(WindowOptions::Visible) |
                    static_cast<int>(WindowOptions::DoubleBuffer) |
                    static_cast<int>(WindowOptions::MakeCurrent),
                const std::shared_ptr<Window>& share = nullptr);
        
            //! Get the window ID.
            FTK_GL_API uint32_t getID() const;

            //! Get the display scale of the display the window is on, or
            //! zero where the platform has no answer. Per window rather
            //! than per display: on macOS the display-level content scale
            //! reports one for external monitors that are in fact scaled,
            //! and only the window knows its own backing.
            FTK_GL_API float getDisplayScale() const;

            //! Get the window title.
            FTK_GL_API std::string getTitle() const;

            //! Set the window title.
            FTK_GL_API void setTitle(const std::string&);

            //! Set the window size.
            FTK_GL_API void setSize(const Size2I&);

            //! Get the window size. Valid for a hidden window too, which
            //! is what a screenshot run has.
            FTK_GL_API Size2I getSize() const;

            //! Get the frame buffer size in pixels.
            FTK_GL_API Size2I getFrameBufferSize() const;

            //! Get the window position, in screen coordinates. Where the
            //! platform keeps the position to itself, as Wayland does, it is
            //! zero.
            FTK_GL_API V2I getPos() const;

            //! Set the window position. The window is kept on the screen;
            //! where the platform places the windows itself this does
            //! nothing.
            FTK_GL_API void setPos(const V2I&);

            //! Get whether the window is maximized.
            FTK_GL_API bool isMaximized() const;

            //! Set whether the window is maximized. A hidden window is
            //! maximized when it is shown.
            FTK_GL_API void setMaximized(bool);

            //! Get the position and size the window has when it is neither
            //! maximized nor full screen: what it goes back to, and what to
            //! remember it by. Remembered by its maximized size, a window
            //! comes back that size without being maximized.
            FTK_GL_API Box2I getNormalGeometry() const;

            //! Note the window's position and size, after the platform has
            //! moved, resized, maximized or restored it; "maximized" for
            //! when it has said the window was maximized.
            FTK_GL_API void geometryChanged(bool maximized = false);

            //! Get the window minimum size.
            FTK_GL_API Size2I getMinSize() const;

            //! Set the window minimum size.
            FTK_GL_API void setMinSize(const Size2I&);

            //! Show the window.
            FTK_GL_API void show();

            //! Hide the window.
            FTK_GL_API void hide();

            //! Set the window icons
            //! 
            //! Icon images should be of type ImageType::RGBA_U8, with no
            //! mirroring, memory alignment of one, and LSB memory endian.
            //!
            //! Window icons are not supported on macOS.
            FTK_GL_API void setIcon(const std::shared_ptr<Image>&);

            //! Make this the current OpenGL context.
            FTK_GL_API void makeCurrent();

            //! Clear the current OpenGL context.
            FTK_GL_API void clearCurrent();

            //! Get which screen the window is on.
            FTK_GL_API int getScreen() const;

            //! Get whether the window is in full screen mode.
            FTK_GL_API bool isFullScreen() const;

            //! Set whether the window is in full screen mode.
            FTK_GL_API void setFullScreen(bool);

            //! Get whether the window is floating on top.
            FTK_GL_API bool isFloatOnTop() const;

            //! Set whether the window is floating on top.
            FTK_GL_API void setFloatOnTop(bool);

            //! Get whether the window has text input.
            FTK_GL_API bool hasTextInput() const;

            //! Set whether the window has text input.
            FTK_GL_API void setTextInput(bool);

            //! Set the text input area: where an input method places
            //! its candidate window, in framebuffer coordinates.
            FTK_GL_API void setTextInputArea(const Box2I&);

            //! Raise the window above the others and give it the input
            //! focus.
            FTK_GL_API void raise();

            //! Swap the buffers.
            FTK_GL_API void swap();

            //! Get the OpenGL information.
            FTK_GL_API const GLInfo& getGLInfo() const;

            //! Get the platform window.
            FTK_GL_API SDL_Window* getSDLWindow() const;

        private:
            FTK_PRIVATE();
        };
        
        ///@}
    }
}

