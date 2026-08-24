// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GL/Export.h>
#include <ftk/Core/Image.h>

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
            MakeCurrent  = 4
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

            //! Get the window title.
            FTK_GL_API std::string getTitle() const;

            //! Set the window title.
            FTK_GL_API void setTitle(const std::string&);

            //! Set the window size.
            FTK_GL_API void setSize(const Size2I&);

            //! Get the window minimum size.
            FTK_GL_API Size2I getMinSize() const;

            //! Set the window minimum size.
            FTK_GL_API void setMinSize(const Size2I&);

            //! Get whether the window is shown.
            FTK_GL_API bool isVisible() const;

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

            //! Raise the window above the others and give it the input
            //! focus.
            FTK_GL_API void raise();

            //! Swap the buffers.
            FTK_GL_API void swap();

            //! Get the OpenGL information.
            FTK_GL_API const GLInfo& getGLInfo() const;

        private:
            FTK_PRIVATE();
        };
        
        ///@}
    }
}

