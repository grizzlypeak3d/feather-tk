// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Image.h>

namespace ftk
{
    class Context;

    namespace gl
    {
        //! \name Windows
        ///@{
        
        //! OpenGL window options.
        enum class FTK_API_TYPE WindowOptions
        {
            None         = 0,
            Visible      = 1,
            DoubleBuffer = 2,
            MakeCurrent  = 4
        };

        //! OpenGL window.
        class FTK_API_TYPE Window : public std::enable_shared_from_this<Window>
        {
            FTK_NON_COPYABLE(Window);

        protected:
            Window(
                const std::shared_ptr<Context>&,
                const std::string& title,
                const Size2I&,
                int options,
                const std::shared_ptr<Window>& share);

        public:
            FTK_API virtual ~Window();

            //! Create a new window.
            FTK_API static std::shared_ptr<Window> create(
                const std::shared_ptr<Context>&,
                const std::string& title,
                const Size2I&,
                int options =
                    static_cast<int>(WindowOptions::Visible) |
                    static_cast<int>(WindowOptions::DoubleBuffer) |
                    static_cast<int>(WindowOptions::MakeCurrent),
                const std::shared_ptr<Window>& share = nullptr);
        
            //! Get the window ID.
            FTK_API uint32_t getID() const;

            //! Get the window title.
            FTK_API std::string getTitle() const;

            //! Set the window title.
            FTK_API void setTitle(const std::string&);

            //! Set the window size.
            FTK_API void setSize(const Size2I&);

            //! Get the window minimum size.
            FTK_API Size2I getMinSize() const;

            //! Set the window minimum size.
            FTK_API void setMinSize(const Size2I&);

            //! Show the window.
            FTK_API void show();

            //! Hide the window.
            FTK_API void hide();

            //! Set the window icons
            //! 
            //! Icon images should be of type ImageType::RGBA_U8, with no
            //! flipping, memory alignment of one, and LSB memory endian.
            //!
            //! Window icons are not supported on macOS.
            FTK_API void setIcon(const std::shared_ptr<Image>&);

            //! Make this the current OpenGL context.
            FTK_API void makeCurrent();

            //! Clear the current OpenGL context.
            FTK_API void clearCurrent();

            //! Get which screen the window is on.
            FTK_API int getScreen() const;

            //! Get whether the window is in full screen mode.
            FTK_API bool isFullScreen() const;

            //! Set whether the window is in full screen mode.
            FTK_API void setFullScreen(bool);

            //! Get whether the window is floating on top.
            FTK_API bool isFloatOnTop() const;

            //! Set whether the window is floating on top.
            FTK_API void setFloatOnTop(bool);

            //! Swap the buffers.
            FTK_API void swap();

        private:
            FTK_PRIVATE();
        };
        
        ///@}
    }
}

