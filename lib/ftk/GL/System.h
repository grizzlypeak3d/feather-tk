// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GL/Export.h>
#include <ftk/Core/ISystem.h>

namespace ftk
{
    class IRenderFactory;

    namespace gl
    {
        //! OpenGL system.
        class FTK_GL_API_TYPE System : public ISystem
        {
        protected:
            System(const std::shared_ptr<Context>&);

        public:
            FTK_GL_API virtual ~System();

            //! Create a new system.
            FTK_GL_API static std::shared_ptr<System> create(const std::shared_ptr<Context>&);
        
            //! Get the render factory.
            FTK_GL_API const std::shared_ptr<IRenderFactory>& getRenderFactory() const;

            //! Set the render factory.
            FTK_GL_API void setRenderFactory(const std::shared_ptr<IRenderFactory>&);

            //! Start SDL's video subsystem. Not done on creation, so that
            //! an application can read its command line first and print its
            //! help without a display; the application calls this once it
            //! knows it is going to run, a window calls it before it is
            //! made, and after the first time it does nothing.
            FTK_GL_API void init();

            //! Choose the graphics API and load OpenGL, starting the video
            //! subsystem first if nothing has. Apart from init(), since
            //! another renderer may be the one that draws: the application
            //! calls this where OpenGL does, a window calls it before it
            //! makes a context, and after the first time it does nothing.
            FTK_GL_API void initGL();

            //! Get the name of the video driver SDL chose (e.g., "x11",
            //! "wayland", "cocoa", "windows").
            FTK_GL_API std::string getVideoDriver() const;

            //! Get the version of SDL in use.
            FTK_GL_API std::string getSDLVersion() const;

        private:
            FTK_PRIVATE();
        };
    }
}
