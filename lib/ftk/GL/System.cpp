// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GL/System.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/Init.h>
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
#include <ftk/Core/OS.h>
#include <ftk/Core/Path.h>

#if defined(__APPLE__)
#include <CoreFoundation/CoreFoundation.h>
#include <objc/message.h>
#include <objc/runtime.h>
#endif // __APPLE__

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
#if defined(__APPLE__)
            // After an application quits abnormally, macOS asks on its next
            // launch whether to reopen its windows, in a modal alert raised
            // while the launch is handled -- inside the first event poll.
            // Nothing is polled until it is answered, so a run nobody is at
            // -- "-exit", a screenshot, a test -- waits on it for good: a
            // Python run killed once hung every run of Python after it.
            //
            // SDL turns the restoring off with ApplePersistenceIgnoreState,
            // but registers it once launching has finished, after the alert.
            // Registered here, before SDL starts, it is in place in time.
            // The same defaults SDL registers itself; the Objective-C
            // runtime saves a source file of another language for the one
            // call. NSDictionary and CFDictionary are the same object.
            void registerMacDefaults()
            {
                Class userDefaultsClass = objc_getClass("NSUserDefaults");
                if (!userDefaultsClass)
                {
                    return;
                }
                auto getObject = reinterpret_cast<id(*)(id, SEL)>(objc_msgSend);
                auto setObject = reinterpret_cast<void(*)(id, SEL, id)>(objc_msgSend);
                id userDefaults = getObject(
                    reinterpret_cast<id>(userDefaultsClass),
                    sel_registerName("standardUserDefaults"));
                // The "Quietly" spelling: the plain one has AppKit log
                // "ApplePersistenceIgnoreState: Existing state will not be
                // touched" on every run, which is the application's output
                // as far as anyone reading a terminal is concerned.
                const void* keys[] = { CFSTR("ApplePersistenceIgnoreStateQuietly") };
                const void* values[] = { kCFBooleanTrue };
                CFDictionaryRef defaults = CFDictionaryCreate(
                    kCFAllocatorDefault,
                    keys,
                    values,
                    1,
                    &kCFTypeDictionaryKeyCallBacks,
                    &kCFTypeDictionaryValueCallBacks);
                setObject(
                    userDefaults,
                    sel_registerName("registerDefaults:"),
                    (id)defaults);
                CFRelease(defaults);
            }
#endif // __APPLE__

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
        }

        namespace
        {
            // Whether a context of an API can be made: tried with a window
            // nobody sees.
            bool hasAPI(API api)
            {
                bool out = false;
                SDL_GL_ResetAttributes();
                setContextAttributes(api);
                if (SDL_Window* sdlWindow = SDL_CreateWindow(
                    "",
#if defined(FTK_SDL2)
                    SDL_WINDOWPOS_UNDEFINED,
                    SDL_WINDOWPOS_UNDEFINED,
#endif // FTK_SDL2
                    16,
                    16,
                    SDL_WINDOW_OPENGL | SDL_WINDOW_HIDDEN))
                {
                    if (SDL_GLContext sdlGLContext = SDL_GL_CreateContext(sdlWindow))
                    {
                        out = true;
#if defined(FTK_SDL2)
                        SDL_GL_DeleteContext(sdlGLContext);
#elif defined(FTK_SDL3)
                        SDL_GL_DestroyContext(sdlGLContext);
#endif // FTK_SDL2
                    }
                    SDL_DestroyWindow(sdlWindow);
                }
                SDL_GL_ResetAttributes();
                return out;
            }
        }

        struct System::Private
        {
            std::weak_ptr<LogSystem> logSystem;
            std::shared_ptr<IRenderFactory> renderFactory;
            bool init = false;
            bool initGL = false;
        };
        
        System::System(const std::shared_ptr<Context>& context) :
            ISystem(context, "ftk::gl::System"),
            _p(new Private)
        {
            FTK_P();

            auto logSystem = context->getLogSystem();
            p.logSystem = logSystem;

            // Which build of the library is actually running.
            const std::string libraryInfo = getLibraryInfo(reinterpret_cast<const void*>(&System::create));
            if (!libraryInfo.empty())
            {
                logSystem->print("ftk::gl::System", libraryInfo);
            }

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
                "ftk GL Memory/Texture cache: {0}MB",
                [] { return gl::Render::getTextureCacheByteCount() / megabyte; });
            diagSystem->addSampler(
                "ftk GL Memory/Texture pool: {0}MB",
                [] { return gl::Render::getTexturePoolByteCount() / megabyte; });

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
            // The two caches the textures are held in, apart: the total on
            // its own cannot say which of them is growing.
            diagSystem->addSampler(
                "ftk GL Objects/Texture cache: {0}",
                [] { return gl::Render::getTextureCacheCount(); });
            diagSystem->addSampler(
                "ftk GL Objects/Texture pool: {0}",
                [] { return gl::Render::getTexturePoolCount(); });
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
            if (p.init)
            {
                // Only what init() started. SDL counts each subsystem's
                // users, and another library in the same process can be one:
                // tlRender starts SDL's audio, and its player destroys its
                // audio stream in its own destructor. SDL_Quit() took audio
                // down with everything else whenever this went first -- as
                // it does when Python frees a module's objects at exit, in no
                // order -- and the player's destructor then crashed in SDL.
                // SDL itself is quit by whichever user is last.
                SDL_QuitSubSystem(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
                if (0 == SDL_WasInit(0))
                {
                    SDL_Quit();
                }
            }
        }

        std::shared_ptr<System> System::create(const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<System>(new System(context));
        }

        const std::shared_ptr<IRenderFactory>& System::getRenderFactory() const
        {
            return _p->renderFactory;
        }

        void System::init()
        {
            FTK_P();
            if (p.init)
            {
                return;
            }
            p.init = true;
            auto logSystem = p.logSystem.lock();
            if (logSystem)
            {
                logSystem->print("ftk::gl::System", "Init SDL video and events...");
            }
#if defined(__APPLE__)
            registerMacDefaults();

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
#if defined(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH)
            // The click that activates a window is delivered to the widget
            // under it, rather than only raising the window. SDL ignores it
            // by default, which leaves a window that highlights what the
            // mouse is over but takes a second click to press it.
            SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");
#endif // SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH
#if defined(FTK_SDL2)
            SDL_SetHint(SDL_HINT_WINDOWS_DPI_AWARENESS, "permonitorv2");
#if defined(SDL_HINT_IME_SUPPORT_EXTENDED_TEXT)
            // Without this, input method compositions longer than the
            // event's fixed buffer are truncated.
            SDL_SetHint(SDL_HINT_IME_SUPPORT_EXTENDED_TEXT, "1");
#endif // SDL_HINT_IME_SUPPORT_EXTENDED_TEXT
            if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
#elif defined(FTK_SDL3)
            if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS))
#endif // FTK_SDL2
            {
                throw std::runtime_error(Format("Cannot initialize SDL: {0}").
                    arg(SDL_GetError()));
            }
            // Which windowing system SDL chose. On Linux the same build
            // runs on X11 or Wayland, and nothing else in a report says
            // which one it was.
            if (logSystem)
            {
                logSystem->print(
                    "ftk::gl::System",
                    Format("Video driver: {0}").arg(getVideoDriver()));
            }
#if defined(FTK_SDL2)
            SDL_LogSetOutputFunction(logOutput, this);
#elif defined(FTK_SDL3)
            SDL_SetLogOutputFunction(logOutput, this);
#endif // FTK_SDL2
        }

        void System::initGL()
        {
            FTK_P();
            if (p.initGL)
            {
                return;
            }
            p.initGL = true;
            init();
            auto logSystem = p.logSystem.lock();

            // Which graphics API this run uses: the first of those to try
            // that a context can be made for. Decided here, before there is
            // a window, since every context is then of that API and a good
            // deal is chosen by it -- the shaders, the texture formats, the
            // defaults of the buffers.
            const std::vector<API> apis = getAPIs();
            API api = apis.front();
            if (apis.size() > 1)
            {
                for (const API i : apis)
                {
                    if (hasAPI(i))
                    {
                        api = i;
                        break;
                    }
                }
            }
            setAPI(api);

            // The attributes are set before the library is loaded as well as
            // before each window: on X11 which library that is, GLX or EGL,
            // goes by the profile asked for.
            setContextAttributes(api);
            if (logSystem)
            {
                logSystem->print(
                    "ftk::gl::System",
                    Format("Graphics API: {0}").arg(getLabel(api)));
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
        }

        std::string System::getVideoDriver() const
        {
            const char* driver = SDL_GetCurrentVideoDriver();
            return driver ? driver : std::string();
        }

        std::string System::getSDLVersion() const
        {
#if defined(FTK_SDL2)
            SDL_version v;
            SDL_GetVersion(&v);
            return Format("{0}.{1}.{2}").arg(v.major).arg(v.minor).arg(v.patch);
#elif defined(FTK_SDL3)
            const int v = SDL_GetVersion();
            return Format("{0}.{1}.{2}").
                arg(SDL_VERSIONNUM_MAJOR(v)).
                arg(SDL_VERSIONNUM_MINOR(v)).
                arg(SDL_VERSIONNUM_MICRO(v));
#endif // FTK_SDL2
        }

        void System::setRenderFactory(const std::shared_ptr<IRenderFactory>& value)
        {
            _p->renderFactory = value;
        }
    }
}
