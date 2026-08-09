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
            FTK_API SetAndRestore(unsigned int, bool);

            FTK_API ~SetAndRestore();

        private:
            FTK_PRIVATE();
        };

        //! Save the OpenGL state a widget is likely to change, and put it back
        //! when finished.
        //!
        //! The renderer sets some state once for the whole frame and leaves it:
        //! blending is enabled in begin() and the primitives only ever set the
        //! blend function. A widget that draws its own OpenGL and does not put
        //! things back therefore breaks the widgets drawn after it -- turning
        //! blending off draws every glyph that follows as a solid box, which is
        //! not a failure anyone traces back to the widget that caused it.
        //!
        //! Covers blending and its function, the depth test and its write mask,
        //! face culling, the scissor test, and the program point size. Reading
        //! state back is not free, so this is one call at the top of a draw
        //! rather than something to sprinkle about; SetAndRestore is the finer
        //! tool when only one capability is in question.
        class FTK_API_TYPE StateSave
        {
        public:
            FTK_API StateSave();

            FTK_API ~StateSave();

        private:
            FTK_PRIVATE();
        };

        //! Get an OpenGL error label.
        FTK_API std::string getErrorLabel(unsigned int);
        
        ///@}
    }
}
