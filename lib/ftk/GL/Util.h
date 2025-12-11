// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/RenderOptions.h>

#include <memory>
#include <string>

namespace ftk
{
    namespace gl
    {
        //! \name Utility
        ///@{

        //! Get the major version number from an OpenGL version string.
        FTK_API int getMajorVersion(const std::string&);

        //! Get the glReadPixels format.
        FTK_API unsigned int getReadPixelsFormat(ImageType);

        //! Get the glReadPixels type.
        FTK_API unsigned int getReadPixelsType(ImageType);

        //! Set the alpha blending.
        FTK_API void setAlphaBlend(AlphaBlend);

        //! Set whether an OpenGL capability is enabled and restore it to the
        //! previous value when finished.
        class FTK_API_TYPE SetAndRestore
        {
        public:
            SetAndRestore(unsigned int, bool);

            ~SetAndRestore();

        private:
            FTK_PRIVATE();
        };

        //! Get an OpenGL error label.
        FTK_API std::string getErrorLabel(unsigned int);
        
        ///@}
    }
}
