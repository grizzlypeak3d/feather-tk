// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/System.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/Mesh.h>
#include <ftk/GL/OffscreenBuffer.h>
#include <ftk/GL/Shader.h>
#include <ftk/GL/Render.h>
#include <ftk/GL/Texture.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/DiagSystem.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>
#include <ftk/Core/String.h>

#if defined(_WIN32)
#include <windows.h>
#else // _WIN32
#include <dlfcn.h>
#include <unistd.h>
#endif // _WIN32

#include <ctime>
#include <filesystem>

#if defined(FTK_SDL2)
#include <SDL2/SDL.h>
#elif defined(FTK_SDL3)
#include <SDL3/SDL.h>
#endif // FTK_SDL2

#include <iostream>

namespace ftk
{
    namespace gl
    {
        namespace
        {
            void logOutput(void *userData, int category, SDL_LogPriority priority, const char *message)
            {
                if (userData)
                {
                    if (auto context = ((System*)userData)->getContext())
                    {
                        auto logSystem = context->getLogSystem();
                        logSystem->print("SDL", message, LogType::Message);
                    }
                }
            }

            // The file this code was loaded from: the shared library, or
            // the executable in a static build.
            std::filesystem::path libraryPath()
            {
                std::filesystem::path out;
#if defined(_WIN32)
                HMODULE module = nullptr;
                if (GetModuleHandleExW(
                    GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS |
                    GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                    reinterpret_cast<LPCWSTR>(&libraryPath),
                    &module))
                {
                    wchar_t buf[MAX_PATH];
                    if (GetModuleFileNameW(module, buf, MAX_PATH))
                    {
                        out = std::filesystem::path(buf);
                    }
                }
#else // _WIN32
                Dl_info info;
                if (dladdr(reinterpret_cast<void*>(&libraryPath), &info) &&
                    info.dli_fname)
                {
                    out = std::filesystem::u8path(info.dli_fname);
                }
#endif // _WIN32
                return out;
            }
        }

        struct System::Private
        {
            std::weak_ptr<LogSystem> logSystem;
            std::shared_ptr<IRenderFactory> renderFactory;
        };
        
        System::System(const std::shared_ptr<Context>& context) :
            ISystem(context, "ftk::gl::System"),
            _p(new Private)
        {
            FTK_P();

            // Initialize SDL.
            auto logSystem = context->getLogSystem();
            logSystem->print("ftk::gl::System", "Init SDL video and events...");
            p.logSystem = logSystem;

            // Which build of the library is actually running. A run that
            // is testing an edit made after this time is testing a stale
            // build or install.
            const std::filesystem::path path = libraryPath();
            if (!path.empty())
            {
                std::error_code ec;
                const auto fileTime = std::filesystem::last_write_time(path, ec);
                std::string modified;
                if (!ec)
                {
                    const auto systemTime =
                        std::chrono::time_point_cast<std::chrono::system_clock::duration>(
                            fileTime -
                            std::filesystem::file_time_type::clock::now() +
                            std::chrono::system_clock::now());
                    const std::time_t t = std::chrono::system_clock::to_time_t(systemTime);
                    char buf[32];
                    if (std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&t)))
                    {
                        modified = buf;
                    }
                }
                logSystem->print(
                    "ftk::gl::System",
                    Format("Library: \"{0}\", modified {1}").
                        arg(path.u8string()).
                        arg(modified));
            }
#if defined(__APPLE__)
            // On macOS 14 and later SDL no longer activates the application
            // at launch, so an application launched from a terminal starts
            // without keyboard focus: the terminal keeps it, and typing
            // goes there until the window is clicked. This asks for the old
            // behavior, but only when a terminal is attached: anything
            // launched through Launch Services -- the Finder, the dock,
            // open -- is activated by it, and SDL's activation path costs
            // a delay at startup. The environment variable
            // SDL_MAC_BACKGROUND_APP still overrides it.
            if (isatty(STDIN_FILENO) || isatty(STDERR_FILENO))
            {
                SDL_SetHint(SDL_HINT_MAC_BACKGROUND_APP, "0");
            }
#endif // __APPLE__
#if defined(FTK_SDL2)
            SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
#elif defined(FTK_SDL3)
            if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
#endif // FTK_SDL2
            {
                throw std::runtime_error(Format("Cannot initialize SDL: {0}").
                    arg(SDL_GetError()));
            }
#if defined(FTK_SDL2)
            if (SDL_GL_LoadLibrary(NULL) < 0)
#elif defined(FTK_SDL3)
            if (!SDL_GL_LoadLibrary(NULL))
#endif // FTK_SDL2
            {
                throw std::runtime_error(Format("Cannot initialize OpenGL: {0}").
                    arg(SDL_GetError()));
            }
#if defined(FTK_SDL2)
            SDL_LogSetOutputFunction(logOutput, this);
#elif defined(FTK_SDL3)
            SDL_SetLogOutputFunction(logOutput, this);
#endif // FTK_SDL2

            // Create default render factory.
            p.renderFactory = std::make_shared<RenderFactory>();

            // Diagnostics.
            auto diagSystem = context->getSystem<DiagSystem>();
            diagSystem->addSampler(
                "ftk GL Memory/Buffers: {0}MB",
                [] { return gl::OffscreenBuffer::getTotalByteCount() / megabyte; });
            diagSystem->addSampler(
                "ftk GL Memory/Meshes: {0}MB",
                [] { return gl::VBO::getTotalByteCount() / megabyte; });
            diagSystem->addSampler(
                "ftk GL Memory/Textures: {0}MB",
                [] { return gl::Texture::getTotalByteCount() / megabyte; });

            diagSystem->addSampler(
                "ftk GL Objects/Buffers: {0}",
                [] { return gl::OffscreenBuffer::getObjectCount(); });
            diagSystem->addSampler(
                "ftk GL Objects/Meshes: {0}",
                [] { return gl::VBO::getObjectCount(); });
            diagSystem->addSampler(
                "ftk GL Objects/Shaders: {0}",
                [] { return gl::Shader::getObjectCount(); });
            diagSystem->addSampler(
                "ftk GL Objects/Textures: {0}",
                [] { return gl::Texture::getObjectCount(); });
        }

        System::~System()
        {
            FTK_P();
            if (auto logSystem = p.logSystem.lock())
            {
                logSystem->print("ftk::gl::System", "Quit SDL...");
            }
#if defined(FTK_SDL2)
            SDL_LogSetOutputFunction(nullptr, nullptr);
#elif defined(FTK_SDL3)
            SDL_SetLogOutputFunction(nullptr, nullptr);
#endif // FTK_SDL2
            SDL_Quit();
        }

        std::shared_ptr<System> System::create(const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<System>(new System(context));
        }

        const std::shared_ptr<IRenderFactory>& System::getRenderFactory() const
        {
            return _p->renderFactory;
        }

        void System::setRenderFactory(const std::shared_ptr<IRenderFactory>& value)
        {
            _p->renderFactory = value;
        }
    }
}
