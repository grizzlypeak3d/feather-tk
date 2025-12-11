// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

#include <memory>

namespace ftk
{
    class Context;

    //! OpenGL support
    namespace gl
    {
        //! Initialize the library.
        FTK_API void init(const std::shared_ptr<Context>&);

        //! Initialize GLAD.
        FTK_API void initGLAD();
    }
}

