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

        private:
            FTK_PRIVATE();
        };
    }
}
