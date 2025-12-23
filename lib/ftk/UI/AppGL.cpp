// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/App.h>

#include <ftk/UI/ClipboardSystem.h>
#include <ftk/UI/IconSystem.h>
#include <ftk/UI/Init.h>
#include <ftk/UI/Util.h>
#include <ftk/UI/Window.h>

#include <ftk/GL/Init.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Error.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>
#include <ftk/Core/String.h>
#include <ftk/Core/Time.h>
#include <ftk/Core/Timer.h>

#if defined(FTK_SDL2)
#include <SDL2/SDL.h>
#elif defined(FTK_SDL3)
#include <SDL3/SDL.h>
#endif // FTK_SDL2

#include <algorithm>
#include <iostream>

namespace ftk
{
    namespace
    {
        const std::chrono::milliseconds timeout(5);
    }

    bool MonitorInfo::operator == (const MonitorInfo& other) const
    {
        return
            name == other.name &&
            size == other.size &&
            refreshRate == other.refreshRate &&
            dpi == other.dpi &&
            bounds == other.bounds;
    }

    bool MonitorInfo::operator != (const MonitorInfo& other) const
    {
        return !(*this == other);
    }

    FTK_ENUM_IMPL(
        ColorStyle,
        "Dark",
        "Light",
        "Custom");

    std::map<ColorRole, Color4F> getCustomColorRoles()
    {
        std::map<ColorRole, Color4F> out = getDefaultColorRoles();
        out[ColorRole::Window] = Color4F(.15F, .15F, .2F);
        out[ColorRole::Button] = Color4F(.2F, .2F, .3F);
        return out;
    }

    std::vector<float> getDisplayScales()
    {
        std::vector<float> out =
        {
            1.F,
            1.25F,
            1.5F,
            1.75F,
            2.F,
            2.25F,
            2.5F,
            2.75F,
            3.F,
            3.25F,
            3.5F,
            3.75F,
            4.F
        };
        return out;
    }

    struct App::Private
    {
        struct CmdLine
        {
            std::shared_ptr<CmdLineFlagOption> exit;
            std::shared_ptr<CmdLineValueOption<float> > displayScale;
            std::shared_ptr<CmdLineValueOption<ColorStyle> > colorStyle;
        };
        CmdLine cmdLine;

        std::shared_ptr<ObservableList<MonitorInfo> > monitors;
        std::shared_ptr<FontSystem> fontSystem;
        std::shared_ptr<IconSystem> iconSystem;
        std::shared_ptr<Style> style;
        std::shared_ptr<Observable<ColorStyle> > colorStyle;
        std::shared_ptr<ObservableMap<ColorRole, Color4F> > customColorRoles;
        float defaultDisplayScale = 1.F;
        std::shared_ptr<Observable<float> > displayScale;
        std::shared_ptr<Observable<bool> > tooltipsEnabled;
        bool running = true;
        std::list<std::shared_ptr<IWindow> > windows;
        std::weak_ptr<IWindow> activeWindow;
        std::map<std::shared_ptr<IWindow>, V2I> mousePos;
        std::vector<std::string> dropFiles;
        std::list<int> tickTimes;
        std::shared_ptr<Timer> logTimer;
    };

    void App::_init(
        const std::shared_ptr<Context>& context,
        const std::vector<std::string>& argv,
        const std::string& name,
        const std::string& summary,
        const std::vector<std::shared_ptr<ICmdLineArg> >& cmdLineArgs,
        const std::vector<std::shared_ptr<ICmdLineOption> >& cmdLineOptions)
    {
        FTK_P();

        std::vector<std::shared_ptr<ICmdLineOption> > cmdLineOptionsTmp = cmdLineOptions;
        p.cmdLine.exit = CmdLineFlagOption::create(
            { "-exit" },
            "Start the user interface and then exit.",
            "Testing");
        cmdLineOptionsTmp.push_back(p.cmdLine.exit);
        p.cmdLine.displayScale = CmdLineValueOption<float>::create(
            { "-displayScale", "-ds" },
            "Set the display scale.",
            "Style");
        cmdLineOptionsTmp.push_back(p.cmdLine.displayScale);
        p.cmdLine.colorStyle = CmdLineValueOption<ColorStyle>::create(
            { "-colorStyle", "-cs" },
            "Set the color style.",
            "Style",
            std::optional<ColorStyle>(),
            quotes(getColorStyleLabels()));
        cmdLineOptionsTmp.push_back(p.cmdLine.colorStyle);

        IApp::_init(
            context,
            argv,
            name,
            summary,
            cmdLineArgs,
            cmdLineOptionsTmp);
        uiInit(context);
        gl::init(context);

        auto logSystem = _context->getSystem<LogSystem>();
        logSystem->print("ftk::App", "Create app...");

        p.monitors = ObservableList<MonitorInfo>::create();
        p.fontSystem = context->getSystem<FontSystem>();
        p.iconSystem = context->getSystem<IconSystem>();
        p.style = Style::create(context);
        p.colorStyle = Observable<ColorStyle>::create(ColorStyle::Dark);
        if (p.cmdLine.colorStyle->hasValue())
        {
            p.colorStyle->setIfChanged(p.cmdLine.colorStyle->getValue());
        }
        p.customColorRoles = ObservableMap<ColorRole, Color4F>::create(ftk::getCustomColorRoles());

#if defined(FTK_SDL2)
        float dDpi = 0.F;
        float hDpi = 0.F;
        float vDpi = 0.F;
        if (0 == SDL_GetDisplayDPI(0, &dDpi, &hDpi, &vDpi))
        {
            logSystem->print(
                "ftk::App",
                Format("Display DPI: {0}").arg(hDpi));
            p.defaultDisplayScale = std::round(hDpi / getBaseDPI());
        }
#elif defined(FTK_SDL3)
        int sdlDisplayCount = 0;
        if (SDL_DisplayID* sdlDisplays = SDL_GetDisplays(&sdlDisplayCount))
        {
            p.defaultDisplayScale = SDL_GetDisplayContentScale(sdlDisplays[0]);
            SDL_free(sdlDisplays);
        }
#endif // FTK_SDL2
        if (p.cmdLine.displayScale->hasValue())
        {
            p.defaultDisplayScale = p.cmdLine.displayScale->getValue();
        }
        if (p.defaultDisplayScale < 1.F)
        {
            p.defaultDisplayScale = 1.F;
        }
        logSystem->print(
            "ftk::App",
            Format("Display scale: {0}").arg(p.defaultDisplayScale));
        p.displayScale = Observable<float>::create(p.defaultDisplayScale);

        p.tooltipsEnabled = Observable<bool>::create(true);

        _monitorsUpdate();
        _styleUpdate();

        p.logTimer = Timer::create(context);
        p.logTimer->setRepeating(true);
        auto weak = std::weak_ptr<App>(std::dynamic_pointer_cast<App>(shared_from_this()));
        p.logTimer->start(
            std::chrono::seconds(10),
            [weak]
            {
                if (auto app = weak.lock())
                {
                    app->_log();
                }
            });
    }

    App::App() :
        _p(new Private)
    {}

    App::~App()
    {
        auto logSystem = _context->getSystem<LogSystem>();
        logSystem->print("ftk::App", "Destroy app...");
    }

    std::shared_ptr<App> App::create(
        const std::shared_ptr<Context>& context,
        int argc,
        char** argv,
        const std::string& name,
        const std::string& summary,
        const std::vector<std::shared_ptr<ICmdLineArg> >& cmdLineArgs,
        const std::vector<std::shared_ptr<ICmdLineOption> >& cmdLineOptions)
    {
        auto out = std::shared_ptr<App>(new App);
        out->_init(context, convert(argc, argv), name, summary, cmdLineArgs, cmdLineOptions);
        return out;
    }

    std::shared_ptr<App> App::create(
        const std::shared_ptr<Context>& context,
        int argc,
        wchar_t* argv[],
        const std::string& name,
        const std::string& summary,
        const std::vector<std::shared_ptr<ICmdLineArg> >& cmdLineArgs,
        const std::vector<std::shared_ptr<ICmdLineOption> >& cmdLineOptions)
    {
        auto out = std::shared_ptr<App>(new App);
        out->_init(context, convert(argc, argv), name, summary, cmdLineArgs, cmdLineOptions);
        return out;
    }

    std::shared_ptr<App> App::create(
        const std::shared_ptr<Context>& context,
        const std::vector<std::string>& args,
        const std::string& name,
        const std::string& summary,
        const std::vector<std::shared_ptr<ICmdLineArg> >& cmdLineArgs,
        const std::vector<std::shared_ptr<ICmdLineOption> >& cmdLineOptions)
    {
        auto out = std::shared_ptr<App>(new App);
        out->_init(context, args, name, summary, cmdLineArgs, cmdLineOptions);
        return out;
    }

    const std::list<std::shared_ptr<IWindow> >& App::getWindows() const
    {
        return _p->windows;
    }

    std::shared_ptr<IObservableList<MonitorInfo> > App::observeMonitors() const
    {
        return _p->monitors;
    }

    const std::shared_ptr<FontSystem>& App::getFontSystem() const
    {
        return _p->fontSystem;
    }

    const std::shared_ptr<IconSystem>& App::getIconSystem() const
    {
        return _p->iconSystem;
    }

    const std::shared_ptr<Style>& App::getStyle() const
    {
        return _p->style;
    }

    ColorStyle App::getColorStyle() const
    {
        return _p->colorStyle->get();
    }

    std::shared_ptr<IObservable<ColorStyle> > App::observeColorStyle() const
    {
        return _p->colorStyle;
    }

    void App::setColorStyle(ColorStyle value)
    {
        FTK_P();
        if (p.colorStyle->setIfChanged(value))
        {
            _styleUpdate();
        }
    }

    const std::map<ColorRole, Color4F>& App::getCustomColorRoles() const
    {
        return _p->customColorRoles->get();
    }

    std::shared_ptr<IObservableMap<ColorRole, Color4F> > App::observeCustomColorRoles() const
    {
        return _p->customColorRoles;
    }

    void App::setCustomColorRoles(const std::map<ColorRole, Color4F>& value)
    {
        FTK_P();
        if (p.customColorRoles->setIfChanged(value))
        {
            if (ColorStyle::Custom == p.colorStyle->get())
            {
                _styleUpdate();
            }
        }
    }

    const std::shared_ptr<CmdLineValueOption<ColorStyle> >& App::getColorStyleCmdLineOption() const
    {
        return _p->cmdLine.colorStyle;
    }

    float App::getDefaultDisplayScale() const
    {
        return _p->defaultDisplayScale;
    }

    float App::getDisplayScale() const
    {
        return _p->displayScale->get();
    }

    std::shared_ptr<IObservable<float> > App::observeDisplayScale() const
    {
        return _p->displayScale;
    }

    void App::setDisplayScale(float value)
    {
        FTK_P();
        p.displayScale->setIfChanged(value);
        for (const auto& window : p.windows)
        {
            window->setDisplayScale(value);
        }
    }

    const std::shared_ptr<CmdLineValueOption<float> >& App::getDisplayScaleCmdLineOption() const
    {
        return _p->cmdLine.displayScale;
    }

    bool App::areTooltipsEnabled() const
    {
        return _p->tooltipsEnabled->get();
    }

    std::shared_ptr<IObservable<bool> > App::observeTooltipsEnabled() const
    {
        return _p->tooltipsEnabled;
    }

    void App::setTooltipsEnabled(bool value)
    {
        FTK_P();
        p.tooltipsEnabled->setIfChanged(value);
        for (const auto& window : p.windows)
        {
            window->setTooltipsEnabled(value);
        }
    }

    void App::exit()
    {
        _p->running = false;
    }

    void App::tick()
    {
        FTK_P();

        _context->tick();

        for (const auto& window : p.windows)
        {
            TickEvent tickEvent;
            _tickRecursive(
                window,
                window->isVisible(false),
                window->isEnabled(false),
                tickEvent);

            if (window->isVisible(false))
            {
                window->_update(p.fontSystem, p.iconSystem, p.style);
            }
        }
    }

    namespace
    {
        MouseButton fromSDLMouseButton(int value)
        {
            MouseButton out = MouseButton::None;
            switch (value)
            {
            case SDL_BUTTON_LEFT:   out = MouseButton::Left;   break;
            case SDL_BUTTON_MIDDLE: out = MouseButton::Middle; break;
            case SDL_BUTTON_RIGHT:  out = MouseButton::Right;  break;
            case SDL_BUTTON_X1:     out = MouseButton::Extra1; break;
            case SDL_BUTTON_X2:     out = MouseButton::Extra2; break;
            }
            return out;
        }

#if defined(FTK_SDL2)
        std::string fromSDLEvent(uint32_t event)
        {
            std::string out = "Unknown";
            switch (event)
            {
            case SDL_FIRSTEVENT: out = "First"; break;
            case SDL_QUIT: out = "Quit"; break;

            case SDL_APP_TERMINATING: out = "App terminating"; break;
            case SDL_APP_LOWMEMORY: out = "Low memory"; break;
            case SDL_APP_WILLENTERBACKGROUND: out = "Will enter background"; break;
            case SDL_APP_DIDENTERBACKGROUND: out = "Did enter background"; break;
            case SDL_APP_WILLENTERFOREGROUND: out = "Will enter foreground"; break;
            case SDL_APP_DIDENTERFOREGROUND: out = "Did enter foreground"; break;

            case SDL_LOCALECHANGED: out = "Local changed"; break;
            case SDL_DISPLAYEVENT: out = "Display"; break;
            case SDL_WINDOWEVENT: out = "Window"; break;
            case SDL_SYSWMEVENT: out = "Sys WM"; break;

            case SDL_KEYDOWN: out = "Key down"; break;
            case SDL_KEYUP: out = "Key up"; break;
            case SDL_TEXTEDITING: out = "Text editing"; break;
            case SDL_TEXTINPUT: out = "Text input"; break;
            case SDL_KEYMAPCHANGED: out = "Keymap changed"; break;
            case SDL_TEXTEDITING_EXT: out = "Text editing ext"; break;

            case SDL_MOUSEMOTION: out = "Mouse motion"; break;
            case SDL_MOUSEBUTTONDOWN: out = "Mouse button down"; break;
            case SDL_MOUSEBUTTONUP: out = "Mouse button up"; break;
            case SDL_MOUSEWHEEL: out = "Mouse wheel"; break;

            case SDL_JOYAXISMOTION: out = "Joy axis motion"; break;
            case SDL_JOYBALLMOTION: out = "Joy ball motion"; break;
            case SDL_JOYHATMOTION: out = "Joy hat motion"; break;
            case SDL_JOYBUTTONDOWN: out = "Joy button down"; break;
            case SDL_JOYBUTTONUP: out = "Joy button up"; break;
            case SDL_JOYDEVICEADDED: out = "Joy device added"; break;
            case SDL_JOYDEVICEREMOVED: out = "Joy device removed"; break;
            case SDL_JOYBATTERYUPDATED: out = "Joy battery updated"; break;

            case SDL_CONTROLLERAXISMOTION: out = "Controller axis motion"; break;
            case SDL_CONTROLLERBUTTONDOWN: out = "Controller button down"; break;
            case SDL_CONTROLLERBUTTONUP: out = "Controller button up"; break;
            case SDL_CONTROLLERDEVICEADDED: out = "Controller device added"; break;
            case SDL_CONTROLLERDEVICEREMOVED: out = "Controller device removed"; break;
            case SDL_CONTROLLERDEVICEREMAPPED: out = "Controller device remapped"; break;
            case SDL_CONTROLLERTOUCHPADDOWN: out = "Controller touch pad down"; break;
            case SDL_CONTROLLERTOUCHPADMOTION: out = "Controller touch pad motion"; break;
            case SDL_CONTROLLERTOUCHPADUP: out = "Controller touch pad up"; break;
            case SDL_CONTROLLERSENSORUPDATE: out = "Controller sensor update"; break;
            case SDL_CONTROLLERUPDATECOMPLETE_RESERVED_FOR_SDL3: out = "Controller reserved for SDL3"; break;
            case SDL_CONTROLLERSTEAMHANDLEUPDATED: out = "Controller steam handle updated"; break; 

            case SDL_FINGERDOWN: out = "Finger down"; break;
            case SDL_FINGERUP: out = "Finger up"; break;
            case SDL_FINGERMOTION: out = "Finger motion"; break;

            case SDL_DOLLARGESTURE: out = "Dollar gesture"; break;
            case SDL_DOLLARRECORD: out = "Dollar record"; break;
            case SDL_MULTIGESTURE: out = "Multi gesture"; break;

            case SDL_CLIPBOARDUPDATE: out = "Clipboard update"; break;

            case SDL_DROPFILE: out = "Drop file"; break;
            case SDL_DROPTEXT: out = "Drop text"; break;
            case SDL_DROPBEGIN: out = "Drop begin"; break;
            case SDL_DROPCOMPLETE: out = "Drop complete"; break;

            case SDL_AUDIODEVICEADDED: out = "Audio device added"; break;
            case SDL_AUDIODEVICEREMOVED: out = "Audio device removed"; break;

            case SDL_SENSORUPDATE: out = "Sensor update"; break;

            case SDL_RENDER_TARGETS_RESET: out = "Render targets reset"; break;
            case SDL_RENDER_DEVICE_RESET: out = "Render device reset"; break;

            case SDL_POLLSENTINEL: out = "Poll sentinel"; break;

            case SDL_USEREVENT: out = "User"; break;

            case SDL_LASTEVENT: out = "Last"; break;

            default: break;
            }
            return out;
        }
#elif defined(FTK_SDL3)
        std::string fromSDLEvent(SDL_EventType event)
        {
            std::string out = "Unknown";
            switch (event)
            {
            default: break;
            }
            return out;
        }
#endif // FTK_SDL2

#if defined(FTK_SDL2)
        Key fromSDLKey(int32_t value)
        {
            Key out = Key::Unknown;
            switch (value)
            {
            case SDLK_RETURN: out = Key::Return; break;
            case SDLK_ESCAPE: out = Key::Escape; break;
            case SDLK_BACKSPACE: out = Key::Backspace; break;
            case SDLK_TAB: out = Key::Tab; break;
            case SDLK_SPACE: out = Key::Space; break;
            case SDLK_EXCLAIM: out = Key::Exclaim; break;
            case SDLK_QUOTEDBL: out = Key::DoubleQuote; break;
            case SDLK_HASH: out = Key::Hash; break;
            case SDLK_PERCENT: out = Key::Percent; break;
            case SDLK_DOLLAR: out = Key::Dollar; break;
            case SDLK_AMPERSAND: out = Key::Ampersand; break;
            case SDLK_QUOTE: out = Key::DoubleQuote; break;
            case SDLK_LEFTPAREN: out = Key::LeftParen; break;
            case SDLK_RIGHTPAREN: out = Key::RightParen; break;
            case SDLK_ASTERISK: out = Key::Asterisk; break;
            case SDLK_PLUS: out = Key::Plus; break;
            case SDLK_COMMA: out = Key::Comma; break;
            case SDLK_MINUS: out = Key::Minus; break;
            case SDLK_PERIOD: out = Key::Period; break;
            case SDLK_SLASH: out = Key::Slash; break;
            case SDLK_0: out = Key::_0; break;
            case SDLK_1: out = Key::_1; break;
            case SDLK_2: out = Key::_2; break;
            case SDLK_3: out = Key::_3; break;
            case SDLK_4: out = Key::_4; break;
            case SDLK_5: out = Key::_5; break;
            case SDLK_6: out = Key::_6; break;
            case SDLK_7: out = Key::_7; break;
            case SDLK_8: out = Key::_8; break;
            case SDLK_9: out = Key::_9; break;
            case SDLK_COLON: out = Key::Colon; break;
            case SDLK_SEMICOLON: out = Key::Semicolon; break;
            case SDLK_LESS: out = Key::Less; break;
            case SDLK_EQUALS: out = Key::Equals; break;
            case SDLK_GREATER: out = Key::Greater; break;
            case SDLK_QUESTION: out = Key::Question; break;
            case SDLK_AT: out = Key::At; break;
            case SDLK_LEFTBRACKET: out = Key::LeftBracket; break;
            case SDLK_BACKSLASH: out = Key::Backslash; break;
            case SDLK_RIGHTBRACKET: out = Key::RightBracket; break;
            case SDLK_CARET: out = Key::Caret; break;
            case SDLK_UNDERSCORE: out = Key::Underscore;  break;
            case SDLK_BACKQUOTE: out = Key::BackQuote; break;
            case SDLK_a: out = Key::A; break;
            case SDLK_b: out = Key::B; break;
            case SDLK_c: out = Key::C; break;
            case SDLK_d: out = Key::D; break;
            case SDLK_e: out = Key::E; break;
            case SDLK_f: out = Key::F; break;
            case SDLK_g: out = Key::G; break;
            case SDLK_h: out = Key::H; break;
            case SDLK_i: out = Key::I; break;
            case SDLK_j: out = Key::J; break;
            case SDLK_k: out = Key::K; break;
            case SDLK_l: out = Key::L; break;
            case SDLK_m: out = Key::M; break;
            case SDLK_n: out = Key::N; break;
            case SDLK_o: out = Key::O; break;
            case SDLK_p: out = Key::P; break;
            case SDLK_q: out = Key::Q; break;
            case SDLK_r: out = Key::R; break;
            case SDLK_s: out = Key::S; break;
            case SDLK_t: out = Key::T; break;
            case SDLK_u: out = Key::U; break;
            case SDLK_v: out = Key::V; break;
            case SDLK_w: out = Key::W; break;
            case SDLK_x: out = Key::X; break;
            case SDLK_y: out = Key::Y; break;
            case SDLK_z: out = Key::Z; break;
            case SDLK_CAPSLOCK: out = Key::CapsLock; break;
            case SDLK_F1: out = Key::F1; break;
            case SDLK_F2: out = Key::F2; break;
            case SDLK_F3: out = Key::F3; break;
            case SDLK_F4: out = Key::F4; break;
            case SDLK_F5: out = Key::F5; break;
            case SDLK_F6: out = Key::F6; break;
            case SDLK_F7: out = Key::F7; break;
            case SDLK_F8: out = Key::F8; break;
            case SDLK_F9: out = Key::F9; break;
            case SDLK_F10: out = Key::F10; break;
            case SDLK_F11: out = Key::F11; break;
            case SDLK_F12: out = Key::F12; break;
            case SDLK_F13: out = Key::F13; break;
            case SDLK_F14: out = Key::F14; break;
            case SDLK_F15: out = Key::F15; break;
            case SDLK_F16: out = Key::F16; break;
            case SDLK_F17: out = Key::F17; break;
            case SDLK_F18: out = Key::F18; break;
            case SDLK_F19: out = Key::F19; break;
            case SDLK_F20: out = Key::F20; break;
            case SDLK_F21: out = Key::F21; break;
            case SDLK_F22: out = Key::F22; break;
            case SDLK_F23: out = Key::F23; break;
            case SDLK_F24: out = Key::F24; break;
            case SDLK_PRINTSCREEN: out = Key::PrintScreen; break;
            case SDLK_SCROLLLOCK: out = Key::ScrollLock; break;
            case SDLK_PAUSE: out = Key::Pause; break;
            case SDLK_INSERT: out = Key::Insert; break;
            case SDLK_HOME: out = Key::Home; break;
            case SDLK_PAGEUP: out = Key::PageUp; break;
            case SDLK_DELETE: out = Key::Delete; break;
            case SDLK_END: out = Key::End; break;
            case SDLK_PAGEDOWN: out = Key::PageDown; break;
            case SDLK_RIGHT: out = Key::Right; break;
            case SDLK_LEFT: out = Key::Left; break;
            case SDLK_DOWN: out = Key::Down; break;
            case SDLK_UP: out = Key::Up; break;
            case SDLK_NUMLOCKCLEAR: out = Key::NumLock; break;
            case SDLK_KP_DIVIDE: out = Key::KeypadDivide; break;
            case SDLK_KP_MULTIPLY: out = Key::KeypadMulitply; break;
            case SDLK_KP_MINUS: out = Key::KeypadMinus; break;
            case SDLK_KP_PLUS: out = Key::KeypadPlus; break;
            case SDLK_KP_ENTER: out = Key::KeypadEnter; break;
            case SDLK_KP_1: out = Key::Keypad_1; break;
            case SDLK_KP_2: out = Key::Keypad_2; break;
            case SDLK_KP_3: out = Key::Keypad_3; break;
            case SDLK_KP_4: out = Key::Keypad_4; break;
            case SDLK_KP_5: out = Key::Keypad_5; break;
            case SDLK_KP_6: out = Key::Keypad_6; break;
            case SDLK_KP_7: out = Key::Keypad_7; break;
            case SDLK_KP_8: out = Key::Keypad_8; break;
            case SDLK_KP_9: out = Key::Keypad_9; break;
            case SDLK_KP_0: out = Key::Keypad_0; break;
            case SDLK_KP_PERIOD: out = Key::KeypadPeriod; break;
            case SDLK_KP_EQUALS: out = Key::KeypadEquals; break;
            default: break;
            }
            return out;
        }
#elif defined(FTK_SDL3)
        Key fromSDLKey(SDL_Keycode value)
        {
            Key out = Key::Unknown;
            switch (value)
            {
            case SDLK_RETURN: out = Key::Return; break;
            case SDLK_ESCAPE: out = Key::Escape; break;
            case SDLK_BACKSPACE: out = Key::Backspace; break;
            case SDLK_TAB: out = Key::Tab; break;
            case SDLK_SPACE: out = Key::Space; break;
            case SDLK_EXCLAIM: out = Key::Exclaim; break;
            case SDLK_DBLAPOSTROPHE: out = Key::DoubleQuote; break;
            case SDLK_HASH: out = Key::Hash; break;
            case SDLK_PERCENT: out = Key::Percent; break;
            case SDLK_DOLLAR: out = Key::Dollar; break;
            case SDLK_AMPERSAND: out = Key::Ampersand; break;
            case SDLK_APOSTROPHE: out = Key::DoubleQuote; break;
            case SDLK_LEFTPAREN: out = Key::LeftParen; break;
            case SDLK_RIGHTPAREN: out = Key::RightParen; break;
            case SDLK_ASTERISK: out = Key::Asterisk; break;
            case SDLK_PLUS: out = Key::Plus; break;
            case SDLK_COMMA: out = Key::Comma; break;
            case SDLK_MINUS: out = Key::Minus; break;
            case SDLK_PERIOD: out = Key::Period; break;
            case SDLK_SLASH: out = Key::Slash; break;
            case SDLK_0: out = Key::_0; break;
            case SDLK_1: out = Key::_1; break;
            case SDLK_2: out = Key::_2; break;
            case SDLK_3: out = Key::_3; break;
            case SDLK_4: out = Key::_4; break;
            case SDLK_5: out = Key::_5; break;
            case SDLK_6: out = Key::_6; break;
            case SDLK_7: out = Key::_7; break;
            case SDLK_8: out = Key::_8; break;
            case SDLK_9: out = Key::_9; break;
            case SDLK_COLON: out = Key::Colon; break;
            case SDLK_SEMICOLON: out = Key::Semicolon; break;
            case SDLK_LESS: out = Key::Less; break;
            case SDLK_EQUALS: out = Key::Equals; break;
            case SDLK_GREATER: out = Key::Greater; break;
            case SDLK_QUESTION: out = Key::Question; break;
            case SDLK_AT: out = Key::At; break;
            case SDLK_LEFTBRACKET: out = Key::LeftBracket; break;
            case SDLK_BACKSLASH: out = Key::Backslash; break;
            case SDLK_RIGHTBRACKET: out = Key::RightBracket; break;
            case SDLK_CARET: out = Key::Caret; break;
            case SDLK_UNDERSCORE: out = Key::Underscore;  break;
            case SDLK_GRAVE: out = Key::BackQuote; break;
            case SDLK_A: out = Key::A; break;
            case SDLK_B: out = Key::B; break;
            case SDLK_C: out = Key::C; break;
            case SDLK_D: out = Key::D; break;
            case SDLK_E: out = Key::E; break;
            case SDLK_F: out = Key::F; break;
            case SDLK_G: out = Key::G; break;
            case SDLK_H: out = Key::H; break;
            case SDLK_I: out = Key::I; break;
            case SDLK_J: out = Key::J; break;
            case SDLK_K: out = Key::K; break;
            case SDLK_L: out = Key::L; break;
            case SDLK_M: out = Key::M; break;
            case SDLK_N: out = Key::N; break;
            case SDLK_O: out = Key::O; break;
            case SDLK_P: out = Key::P; break;
            case SDLK_Q: out = Key::Q; break;
            case SDLK_R: out = Key::R; break;
            case SDLK_S: out = Key::S; break;
            case SDLK_T: out = Key::T; break;
            case SDLK_U: out = Key::U; break;
            case SDLK_V: out = Key::V; break;
            case SDLK_W: out = Key::W; break;
            case SDLK_X: out = Key::X; break;
            case SDLK_Y: out = Key::Y; break;
            case SDLK_Z: out = Key::Z; break;
            case SDLK_CAPSLOCK: out = Key::CapsLock; break;
            case SDLK_F1: out = Key::F1; break;
            case SDLK_F2: out = Key::F2; break;
            case SDLK_F3: out = Key::F3; break;
            case SDLK_F4: out = Key::F4; break;
            case SDLK_F5: out = Key::F5; break;
            case SDLK_F6: out = Key::F6; break;
            case SDLK_F7: out = Key::F7; break;
            case SDLK_F8: out = Key::F8; break;
            case SDLK_F9: out = Key::F9; break;
            case SDLK_F10: out = Key::F10; break;
            case SDLK_F11: out = Key::F11; break;
            case SDLK_F12: out = Key::F12; break;
            case SDLK_F13: out = Key::F13; break;
            case SDLK_F14: out = Key::F14; break;
            case SDLK_F15: out = Key::F15; break;
            case SDLK_F16: out = Key::F16; break;
            case SDLK_F17: out = Key::F17; break;
            case SDLK_F18: out = Key::F18; break;
            case SDLK_F19: out = Key::F19; break;
            case SDLK_F20: out = Key::F20; break;
            case SDLK_F21: out = Key::F21; break;
            case SDLK_F22: out = Key::F22; break;
            case SDLK_F23: out = Key::F23; break;
            case SDLK_F24: out = Key::F24; break;
            case SDLK_PRINTSCREEN: out = Key::PrintScreen; break;
            case SDLK_SCROLLLOCK: out = Key::ScrollLock; break;
            case SDLK_PAUSE: out = Key::Pause; break;
            case SDLK_INSERT: out = Key::Insert; break;
            case SDLK_HOME: out = Key::Home; break;
            case SDLK_PAGEUP: out = Key::PageUp; break;
            case SDLK_DELETE: out = Key::Delete; break;
            case SDLK_END: out = Key::End; break;
            case SDLK_PAGEDOWN: out = Key::PageDown; break;
            case SDLK_RIGHT: out = Key::Right; break;
            case SDLK_LEFT: out = Key::Left; break;
            case SDLK_DOWN: out = Key::Down; break;
            case SDLK_UP: out = Key::Up; break;
            case SDLK_NUMLOCKCLEAR: out = Key::NumLock; break;
            case SDLK_KP_DIVIDE: out = Key::KeypadDivide; break;
            case SDLK_KP_MULTIPLY: out = Key::KeypadMulitply; break;
            case SDLK_KP_MINUS: out = Key::KeypadMinus; break;
            case SDLK_KP_PLUS: out = Key::KeypadPlus; break;
            case SDLK_KP_ENTER: out = Key::KeypadEnter; break;
            case SDLK_KP_1: out = Key::Keypad_1; break;
            case SDLK_KP_2: out = Key::Keypad_2; break;
            case SDLK_KP_3: out = Key::Keypad_3; break;
            case SDLK_KP_4: out = Key::Keypad_4; break;
            case SDLK_KP_5: out = Key::Keypad_5; break;
            case SDLK_KP_6: out = Key::Keypad_6; break;
            case SDLK_KP_7: out = Key::Keypad_7; break;
            case SDLK_KP_8: out = Key::Keypad_8; break;
            case SDLK_KP_9: out = Key::Keypad_9; break;
            case SDLK_KP_0: out = Key::Keypad_0; break;
            case SDLK_KP_PERIOD: out = Key::KeypadPeriod; break;
            case SDLK_KP_EQUALS: out = Key::KeypadEquals; break;
            default: break;
            }
            return out;
        }
#endif // FTK_SDL2

        int fromSDLKeyModifier(uint16_t value)
        {
            int out = 0;
#if defined(FTK_SDL2)
            if (value & KMOD_SHIFT)
            {
                out |= static_cast<int>(KeyModifier::Shift);
            }
            if (value & KMOD_CTRL)
            {
                out |= static_cast<int>(KeyModifier::Control);
            }
            if (value & KMOD_ALT)
            {
                out |= static_cast<int>(KeyModifier::Alt);
            }
            if (value & KMOD_GUI)
            {
                out |= static_cast<int>(KeyModifier::Super);
            }
#elif defined(FTK_SDL3)
            if (value & SDL_KMOD_SHIFT)
            {
                out |= static_cast<int>(KeyModifier::Shift);
            }
            if (value & SDL_KMOD_CTRL)
            {
                out |= static_cast<int>(KeyModifier::Control);
            }
            if (value & SDL_KMOD_ALT)
            {
                out |= static_cast<int>(KeyModifier::Alt);
            }
            if (value & SDL_KMOD_GUI)
            {
                out |= static_cast<int>(KeyModifier::Super);
            }
#endif // FTK_SDL2
            return out;
        }
    }

    void App::run()
    {
        FTK_P();
        auto t0 = std::chrono::steady_clock::now();

        // Make sure one of the windows is visible.
        bool visible = false;
        for (const auto& window : p.windows)
        {
            visible |= window->isVisible(false);
        }
        if (!visible && !p.windows.empty())
        {
            p.windows.front()->show();
        }

        while (p.running && !p.windows.empty())
        {
            auto logSystem = _context->getSystem<LogSystem>();
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                //std::cout << "Event: " << fromSDLEvent(event.type) << std::endl;
                switch (event.type)
                {
#if defined(FTK_SDL2)
                case SDL_DISPLAYEVENT:
#elif defined(FTK_SDL3)
                case SDL_EVENT_DISPLAY_ORIENTATION:
                case SDL_EVENT_DISPLAY_ADDED:
                case SDL_EVENT_DISPLAY_REMOVED:
                case SDL_EVENT_DISPLAY_MOVED:
                case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED:
                case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
                case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
#endif // FTK_SDL2
                    logSystem->print("ftk::App", "SDL Display Event");
                    _monitorsUpdate();
                    break;

#if defined(FTK_SDL2)
                case SDL_WINDOWEVENT:
                {
                    switch (event.window.event)
                    {
                    case SDL_WINDOWEVENT_SHOWN:
                        //std::cout << "Window event shown" << std::endl;
                        if (auto window = _getWindow(event.window.windowID))
                        {
                            window->setVisible(true);
                        }
                        break;
                    case SDL_WINDOWEVENT_HIDDEN:
                        //std::cout << "Window event hidden" << std::endl;
                        if (auto window = _getWindow(event.window.windowID))
                        {
                            window->setVisible(false);
                        }
                        break;
                    case SDL_WINDOWEVENT_EXPOSED:
                        //std::cout << "Window event exposed" << std::endl;
                        if (auto window = _getWindow(event.window.windowID))
                        {
                            window->_cursorEnter(true);
                            p.activeWindow = window;
                        }
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        //std::cout << "Window event size changed" << std::endl;
                        if (auto window = _getWindow(event.window.windowID))
                        {
                            if (SDL_Window* sdlWindow = SDL_GetWindowFromID(event.window.windowID))
                            {
                                Size2I windowSize;
                                Size2I frameBufferSize;
                                SDL_GetWindowSize(sdlWindow, &windowSize.w, &windowSize.h);
                                SDL_GL_GetDrawableSize(sdlWindow, &frameBufferSize.w, &frameBufferSize.h);
                                window->_sizeUpdate(windowSize, frameBufferSize);
                            }
                        }
                        break;
                    case SDL_WINDOWEVENT_ENTER:
                        //std::cout << "Window event enter" << std::endl;
                        if (auto window = _getWindow(event.window.windowID))
                        {
                            window->_cursorEnter(true);
                            p.activeWindow = window;
                        }
                        break;
                    case SDL_WINDOWEVENT_LEAVE:
                        //std::cout << "Window event leave" << std::endl;
                        if (auto window = _getWindow(event.window.windowID))
                        {
                            window->_cursorEnter(false);
                            p.activeWindow.reset();
                        }
                        break;
                    case SDL_WINDOWEVENT_CLOSE:
                        //std::cout << "Window event close" << std::endl;
                        if (auto window = _getWindow(event.window.windowID))
                        {
                            window->close();
                        }
                        break;
                    default: break;
                    }
                    break;
                }
#elif defined(FTK_SDL3)
                case SDL_EVENT_WINDOW_SHOWN:
                    if (auto window = _getWindow(event.window.windowID))
                    {
                        window->setVisible(true);
                    }
                    break;
                case SDL_EVENT_WINDOW_HIDDEN:
                    if (auto window = _getWindow(event.window.windowID))
                    {
                        window->setVisible(false);
                    }
                    break;
                case SDL_EVENT_WINDOW_EXPOSED:
                case SDL_EVENT_WINDOW_RESIZED:
                    if (auto window = _getWindow(event.window.windowID))
                    {
                        if (SDL_Window* sdlWindow = SDL_GetWindowFromID(event.window.windowID))
                        {
                            Size2I windowSize;
                            Size2I frameBufferSize;
                            SDL_GetWindowSize(sdlWindow, &windowSize.w, &windowSize.h);
                            SDL_GetWindowSizeInPixels(sdlWindow, &frameBufferSize.w, &frameBufferSize.h);
                            window->_sizeUpdate(windowSize, frameBufferSize);
                        }
                    }
                    break;
                case SDL_EVENT_WINDOW_MOUSE_ENTER:
                    if (auto window = _getWindow(event.window.windowID))
                    {
                        window->_cursorEnter(true);
                        p.activeWindow = window;
                    }
                    break;
                case SDL_EVENT_WINDOW_MOUSE_LEAVE:
                    if (auto window = _getWindow(event.window.windowID))
                    {
                        window->_cursorEnter(false);
                        p.activeWindow.reset();
                    }
                    break;
                case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                    if (auto window = _getWindow(event.window.windowID))
                    {
                        window->close();
                    }
                    break;
#endif // FTK_SDL2

#if defined(FTK_SDL2)
                case SDL_MOUSEMOTION:
#elif defined(FTK_SDL3)
                case SDL_EVENT_MOUSE_MOTION:
#endif // FTK_SDL2
                    if (auto window = p.activeWindow.lock())
                    {
                        const float contentScale = window->getContentScale();
                        const V2I pos(
                            event.motion.x * contentScale,
                            event.motion.y * contentScale);
                        window->_cursorPos(pos);
                        p.mousePos[window] = pos;
                    }
                    break;

#if defined(FTK_SDL2)
                case SDL_MOUSEBUTTONDOWN:
#elif defined(FTK_SDL3)
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
#endif // FTK_SDL2
                    if (auto window = p.activeWindow.lock())
                    {
                        window->_mouseButton(
                            fromSDLMouseButton(event.button.button),
                            true,
                            fromSDLKeyModifier(static_cast<uint16_t>(SDL_GetModState())));
                    }
                    break;

#if defined(FTK_SDL2)
                case SDL_MOUSEBUTTONUP:
#elif defined(FTK_SDL3)
                case SDL_EVENT_MOUSE_BUTTON_UP:
#endif // FTK_SDL2
                    if (auto window = p.activeWindow.lock())
                    {
                        window->_mouseButton(
                            fromSDLMouseButton(event.button.button),
                            false,
                            fromSDLKeyModifier(static_cast<uint16_t>(SDL_GetModState())));
                    }
                    break;

#if defined(FTK_SDL2)
                case SDL_MOUSEWHEEL:
                    if (auto window = p.activeWindow.lock())
                    {
                        const float contentScale = window->getContentScale();
                        window->_scroll(V2F(
                            event.wheel.preciseX * contentScale,
                            event.wheel.preciseY * contentScale),
                            fromSDLKeyModifier(static_cast<uint16_t>(SDL_GetModState())));
                    }
                    break;
#elif defined(FTK_SDL3)
                case SDL_EVENT_MOUSE_WHEEL:
                    if (auto window = p.activeWindow.lock())
                    {
                        const float contentScale = window->getContentScale();
                        window->_scroll(V2F(
                            event.wheel.x * contentScale,
                            event.wheel.y * contentScale),
                            fromSDLKeyModifier(static_cast<uint16_t>(SDL_GetModState())));
                    }
                    break;
#endif // FTK_SDL2


#if defined(FTK_SDL2)
                case SDL_KEYDOWN:
                    if (auto window = p.activeWindow.lock())
                    {
                        window->_key(
                            fromSDLKey(event.key.keysym.sym),
                            true,
                            fromSDLKeyModifier(event.key.keysym.mod));
                    }
                    break;
#elif defined(FTK_SDL3)
                case SDL_EVENT_KEY_DOWN:
                    if (auto window = p.activeWindow.lock())
                    {
                        window->_key(
                            fromSDLKey(event.key.key),
                            true,
                            fromSDLKeyModifier(event.key.mod));
                    }
                    break;
#endif // FTK_SDL3

#if defined(FTK_SDL2)
                case SDL_KEYUP:
                    if (auto window = p.activeWindow.lock())
                    {
                        window->_key(
                            fromSDLKey(event.key.keysym.sym),
                            false,
                            fromSDLKeyModifier(event.key.keysym.mod));
                    }
                    break;
#elif defined(FTK_SDL3)
                case SDL_EVENT_KEY_UP:
                    if (auto window = p.activeWindow.lock())
                    {
                        window->_key(
                            fromSDLKey(event.key.key),
                            false,
                            fromSDLKeyModifier(event.key.mod));
                    }
                    break;
#endif // FTK_SDL2

#if defined(FTK_SDL2)
                case SDL_TEXTINPUT:
                    if (auto window = p.activeWindow.lock())
                    {
                        window->_text(event.text.text);
                    }
                    break;
#elif defined(FTK_SDL3)
                case SDL_EVENT_TEXT_INPUT:
                    if (auto window = p.activeWindow.lock())
                    {
                        window->_text(event.text.text);
                    }
                    break;
#endif // FTK_SDL2

#if defined(FTK_SDL2)
                case SDL_CLIPBOARDUPDATE:
                {
                    const std::string text = SDL_GetClipboardText();
                    _context->getSystem<ClipboardSystem>()->setText(text);
                    break;
                }
#elif defined(FTK_SDL3)
                case SDL_EVENT_CLIPBOARD_UPDATE:
                {
                    const std::string text = SDL_GetClipboardText();
                    _context->getSystem<ClipboardSystem>()->setText(text);
                    break;
                }
#endif // FTK_SDL2

#if defined(FTK_SDL2)
                case SDL_DROPFILE:
                    logSystem->print("ftk::App", Format("SDL_DROPFILE: {0}").arg(event.drop.file));
                    p.dropFiles.push_back(event.drop.file);
                    break;
#elif defined(FTK_SDL3)
                case SDL_EVENT_DROP_FILE:
                    logSystem->print("ftk::App", Format("SDL_EVENT_DROP_FILE: {0}").arg(event.drop.data));
                    p.dropFiles.push_back(event.drop.data);
                    break;
#endif // FTK_SDL2

#if defined(FTK_SDL2)
                case SDL_DROPBEGIN:
#elif defined(FTK_SDL3)
                case SDL_EVENT_DROP_BEGIN:
#endif // FTK_SDL2
                    logSystem->print("ftk::App", "SDL_DROPBEGIN");
                    p.dropFiles.clear();
                    break;

#if defined(FTK_SDL2)
                case SDL_DROPCOMPLETE:
#elif defined(FTK_SDL3)
                case SDL_EVENT_DROP_COMPLETE:
#endif // FTK_SDL2
                {
                    logSystem->print("ftk::App", "SDL_DROPCOMPLETE");
                    bool found = false;
                    for (const auto& window : p.windows)
                    {
                        if (window->getID() == event.drop.windowID)
                        {
                            found = true;
                            V2I pos;
                            auto i = p.mousePos.find(window);
                            if (i != p.mousePos.end())
                            {
                                pos = i->second;
                            }
                            DragDropEvent event(pos, pos, std::make_shared<DragDropTextData>(p.dropFiles));
                            window->dropEvent(event);
                            break;
                        }
                    }
                    if (!found)
                    {
                        if (auto window = p.activeWindow.lock())
                        {
                            V2I pos;
                            auto i = p.mousePos.find(window);
                            if (i != p.mousePos.end())
                            {
                                pos = i->second;
                            }
                            DragDropEvent event(pos, pos, std::make_shared<DragDropTextData>(p.dropFiles));
                            window->dropEvent(event);
                        }
                        else if (!p.windows.empty())
                        {
                            V2I pos;
                            auto i = p.mousePos.find(p.windows.front());
                            if (i != p.mousePos.end())
                            {
                                pos = i->second;
                            }
                            DragDropEvent event(pos, pos, std::make_shared<DragDropTextData>(p.dropFiles));
                            p.windows.front()->dropEvent(event);
                        }
                    }
                    break;
                }

#if defined(FTK_SDL2)
                case SDL_QUIT:
#elif defined(FTK_SDL3)
                case SDL_EVENT_QUIT:
#endif // FTK_SDL2
                    exit();
                    break;

                default: break;
                }
            }

            tick();

            auto t1 = std::chrono::steady_clock::now();
            sleep(timeout, t0, t1);
            t1 = std::chrono::steady_clock::now();
            const auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0);
            p.tickTimes.push_back(diff.count());
            while (p.tickTimes.size() > 10)
            {
                p.tickTimes.pop_front();
            }
            t0 = t1;

            if (p.cmdLine.exit->found())
            {
                break;
            }
        }
    }

    void App::_addWindow(const std::shared_ptr<IWindow>& window)
    {
        FTK_P();
        const auto i = std::find(p.windows.begin(), p.windows.end(), window);
        if (i == p.windows.end())
        {
            window->setDisplayScale(p.displayScale->get());
            window->setTooltipsEnabled(p.tooltipsEnabled->get());
            p.windows.push_back(window);
        }
    }

    void App::_removeWindow(const std::shared_ptr<IWindow>& window)
    {
        FTK_P();
        const auto i = std::find(p.windows.begin(), p.windows.end(), window);
        if (i != p.windows.end())
        {
            p.windows.erase(i);
        }
        const auto j = p.mousePos.find(window);
        if (j != p.mousePos.end())
        {
            p.mousePos.erase(j);
        }
    }

    std::shared_ptr<IWindow> App::_getWindow(uint32_t id) const
    {
        FTK_P();
        const auto i = std::find_if(
            p.windows.begin(),
            p.windows.end(),
            [id](const std::shared_ptr<IWindow>& window)
            {
                return window->getID() == id;
            });
        return i != p.windows.end() ? *i : nullptr;
    }

    void App::_tickRecursive(
        const std::shared_ptr<IWidget>& widget,
        bool visible,
        bool enabled,
        const TickEvent& event)
    {
        FTK_P();
        const bool parentsVisible = visible && widget->isVisible(false);
        const bool parentsEnabled = enabled && widget->isEnabled(false);
        for (const auto& child : widget->getChildren())
        {
            _tickRecursive(
                child,
                parentsVisible,
                parentsEnabled,
                event);
        }
        widget->tickEvent(visible, enabled, event);
    }

    void App::_monitorsUpdate()
    {
        FTK_P();
        std::vector<MonitorInfo> monitors;

#if defined(FTK_SDL2)
        const int displayCount = SDL_GetNumVideoDisplays();
        for (int i = 0; i < displayCount; ++i)
        {
            MonitorInfo monitor;
            monitor.name = SDL_GetDisplayName(i);
            SDL_DisplayMode sdlDisplayMode;
            SDL_GetCurrentDisplayMode(i, &sdlDisplayMode);
            monitor.size.w = sdlDisplayMode.w;
            monitor.size.h = sdlDisplayMode.h;
            monitor.refreshRate = sdlDisplayMode.refresh_rate;
            float dDpi = 0.F;
            SDL_GetDisplayDPI(i, &dDpi, &monitor.dpi.x, &monitor.dpi.y);
            SDL_Rect sdlRect;
            SDL_GetDisplayBounds(i, &sdlRect);
            monitor.bounds = Box2I(sdlRect.x, sdlRect.y, sdlRect.w, sdlRect.h);
            monitors.push_back(monitor);
        }
        p.monitors->setIfChanged(monitors);
#elif defined(FTK_SDL3)
        int sdlDisplayCount = 0;
        if (SDL_DisplayID* sdlDisplays = SDL_GetDisplays(&sdlDisplayCount))
        {
            for (int i = 0; i < sdlDisplayCount; ++i)
            {
                MonitorInfo monitor;
                if (const char* sdlName = SDL_GetDisplayName(i))
                {
                    monitor.name = sdlName;
                }
                if (const SDL_DisplayMode* sdlDisplayMode = SDL_GetCurrentDisplayMode(i))
                {
                    monitor.size.w = sdlDisplayMode->w;
                    monitor.size.h = sdlDisplayMode->h;
                    monitor.refreshRate = sdlDisplayMode->refresh_rate;
                    //! \todo DPI
                    SDL_Rect sdlRect;
                    SDL_GetDisplayBounds(i, &sdlRect);
                    monitor.bounds = Box2I(sdlRect.x, sdlRect.y, sdlRect.w, sdlRect.h);
                    monitors.push_back(monitor);
                }
            }
            SDL_free(sdlDisplays);
        }
#endif // FTK_SDL2

        std::vector<std::string> lines;
        for (int i = 0; i < monitors.size(); ++i)
        {
            lines.push_back(Format("Monitor: {0}").arg(monitors[i].name));
            lines.push_back(Format("    Size: {0}").arg(monitors[i].size));
            lines.push_back(Format("    Referesh rate: {0}").arg(monitors[i].refreshRate));
            lines.push_back(Format("    DPI: {0}").arg(monitors[i].dpi.x));
            lines.push_back(Format("    Bounds: {0}").arg(monitors[i].bounds));
        }
        if (!lines.empty())
        {
            auto logSystem = _context->getSystem<LogSystem>();
            logSystem->print(
                "ftk::App",
                join(lines, '\n'));
        }
    }

    void App::_styleUpdate()
    {
        FTK_P();
        std::map<ColorRole, Color4F> colorRoles;
        switch (p.colorStyle->get())
        {
        case ColorStyle::Dark: colorRoles = getDefaultColorRoles(); break;
        case ColorStyle::Light: colorRoles = getLightColorRoles(); break;
        case ColorStyle::Custom: colorRoles = p.customColorRoles->get(); break;
        default: break;
        }
        p.style->setColorRoles(colorRoles);
    }

    void App::_log()
    {
        FTK_P();
        double tickAverage = 0.0;
        if (!p.tickTimes.empty())
        {
            for (auto t : p.tickTimes)
            {
                tickAverage += t;
            }
            tickAverage /= static_cast<double>(p.tickTimes.size());
        }
        auto logSystem = _context->getSystem<LogSystem>();
        logSystem->print(
            "ftk::App",
            Format("Average tick time: {0}ms").arg(tickAverage));
    }
}
