// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GL/Export.h>
#include <ftk/Core/Export.h>
#include <ftk/Core/Util.h>

#include <memory>
#include <string>
#include <vector>

namespace ftk
{
    class Context;

    //! OpenGL support
    namespace gl
    {
        //! The graphics APIs. One build has both, and which one a program
        //! uses is decided when it runs: OpenGL where it can be had, and
        //! OpenGL ES where it cannot, or the other way round for a build
        //! that prefers ES.
        enum class FTK_GL_API_TYPE API
        {
            GL_4_1,
            GLES_3,

            Count,
            First = GL_4_1
        };
        FTK_ENUM(FTK_GL_API, API);

        //! Get the APIs to try, the preferred one first. The build says
        //! which is preferred (ftk_API), and the FTK_GL_API environment
        //! variable names the only one to try. macOS has OpenGL alone and
        //! the web OpenGL ES alone.
        FTK_GL_API std::vector<API> getAPIs();

        //! Get the API in use. Until the first context is made that is the
        //! preferred one; gl::System decides when it starts the video, and
        //! every context after is of the same API.
        FTK_GL_API API getAPI();

        //! Set the API in use. Only before a context has been made.
        FTK_GL_API void setAPI(API);

        //! Get whether the API in use is OpenGL ES.
        FTK_GL_API bool isGLES();

        //! Ask SDL for a context of an API: its version and profile.
        FTK_GL_API void setContextAttributes(API);

        //! Initialize the library.
        FTK_GL_API void init(const std::shared_ptr<Context>&);

        //! Initialize GLAD, for the API in use.
        FTK_GL_API void initGLAD();
    }
}
