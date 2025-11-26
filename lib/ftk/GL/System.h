// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ISystem.h>

namespace ftk
{
    class IRenderFactory;

    namespace gl
    {
        //! OpenGL system.
        class System : public ISystem
        {
        protected:
            System(const std::shared_ptr<Context>&);

        public:
            virtual ~System();

            //! Create a new system.
            static std::shared_ptr<System> create(const std::shared_ptr<Context>&);
        
            //! Get the render factory.
            const std::shared_ptr<IRenderFactory>& getRenderFactory() const;

            //! Set the render factory.
            void setRenderFactory(const std::shared_ptr<IRenderFactory>&);

        private:
            FTK_PRIVATE();
        };
    }
}
