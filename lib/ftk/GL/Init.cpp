// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/Init.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/System.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Error.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/String.h>

#if defined(FTK_SDL2)
#include <SDL2/SDL.h>
#elif defined(FTK_SDL3)
#include <SDL3/SDL.h>
#endif // FTK_SDL2

#include <algorithm>
#include <cstdlib>
#include <stdexcept>

namespace ftk
{
    namespace gl
    {
        FTK_ENUM_IMPL(
            API,
            "GL_4_1",
            "GLES_3");

        namespace
        {
#if !defined(__EMSCRIPTEN__) && !defined(__APPLE__)
            API getPreferredAPI()
            {
#if defined(FTK_API_GLES_3)
                return API::GLES_3;
#else // FTK_API_GLES_3
                return API::GL_4_1;
#endif // FTK_API_GLES_3
            }
#endif // __EMSCRIPTEN__

            API& getAPIRef()
            {
                static API api = getAPIs().front();
                return api;
            }
        }

        std::vector<API> getAPIs()
        {
            std::vector<API> out;
#if defined(__EMSCRIPTEN__)
            out.push_back(API::GLES_3);
#elif defined(__APPLE__)
            out.push_back(API::GL_4_1);
#else // __EMSCRIPTEN__
            if (const char* env = std::getenv("FTK_GL_API"))
            {
                API api = API::GL_4_1;
                if (from_string(env, api))
                {
                    out.push_back(api);
                }
            }
            if (out.empty())
            {
                const API preferred = getPreferredAPI();
                out.push_back(preferred);
                out.push_back(API::GL_4_1 == preferred ? API::GLES_3 : API::GL_4_1);
            }
#endif // __EMSCRIPTEN__
            return out;
        }

        API getAPI()
        {
            return getAPIRef();
        }

        void setAPI(API value)
        {
            getAPIRef() = value;
        }

        bool isGLES()
        {
            return API::GLES_3 == getAPIRef();
        }

        void setContextAttributes(API api)
        {
            switch (api)
            {
            case API::GL_4_1:
            {
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
                const int glProfile =
#if defined(__APPLE__)
                    SDL_GL_CONTEXT_PROFILE_CORE;
#else // __APPLE__
                    SDL_GL_CONTEXT_PROFILE_COMPATIBILITY;
#endif // __APPLE__
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, glProfile);
                break;
            }
            case API::GLES_3:
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
                SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
                break;
            default: break;
            }
        }

        void init(const std::shared_ptr<Context>& context)
        {
            if (!context->getSystem<System>())
            {
                context->addSystem(System::create(context));
            }
        }

        void initGLAD()
        {
            // The functions the context has, asked of SDL by name. One
            // loader holds both APIs: what the API in use lacks is left a
            // null pointer.
            int r = 0;
            const auto load = reinterpret_cast<GLADloadfunc>(SDL_GL_GetProcAddress);
            switch (getAPI())
            {
            case API::GL_4_1: r = gladLoadGL(load); break;
            case API::GLES_3: r = gladLoadGLES2(load); break;
            default: break;
            }
            if (0 == r)
            {
                throw std::runtime_error(Format("Cannot initialize GLAD: {0}").arg(r));
            }
        }
    }
}
