// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/AppTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Window.h>

#include <ftk/UI/PushButton.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

#if defined(FTK_SDL2)
#include <SDL2/SDL.h>
#elif defined(FTK_SDL3)
#include <SDL3/SDL.h>
#endif // FTK_SDL2

namespace ftk
{
    namespace ui_test
    {
        AppTest::AppTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::AppTest")
        {}

        AppTest::~AppTest()
        {}

        std::shared_ptr<AppTest> AppTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<AppTest>(new AppTest(context));
        }
                
        void AppTest::run()
        {
            _mouseRouting();

            std::vector<std::string> argv = { "app", "-exit" };
            try
            {
                auto app = App::create(
                    _context,
                    argv,
                    "ftk::ui_test::App",
                    "Test UI application");

                auto window = Window::create(_context, app, "ftk::ui_test::App");
                auto layout = VerticalLayout::create(_context, window);
                auto button = PushButton::create(_context, "Button", layout);
                FTK_CHECK(app->getWindows().front() == window);

                // A scale the application set is not corrected by the window;
                // an automatic one is.
                app->setDisplayScale(1.5F);
                app->setDisplayScaleFromWindow(2.F);
                FTK_CHECK(1.5F == app->getDisplayScale());
                app->setDisplayScale(0.F);
                FTK_CHECK(2.F == app->getDisplayScale());
                app->setDisplayScaleFromWindow(3.F);
                FTK_CHECK(3.F == app->getDisplayScale());
                window->show();
                window->setSize(Size2I(1280, 960));

                _print(Format("Window size: {0}").arg(window->getSize()));
                _print(Format("Buffer size: {0}").arg(window->getBufferSize()));
                _print(Format("Display scale: {0}").arg(window->getDisplayScale()));

                app->run();
                app->exit();
                window->close();
            }
            catch (const std::exception&)
            {}
        }
        void AppTest::_mouseRouting()
        {
#if !defined(__EMSCRIPTEN__) && (defined(FTK_SDL2) || defined(FTK_SDL3))
            // A mouse event goes to the window SDL tagged it with. The
            // events below are the order moving from one window to another
            // can arrive in -- the enter for the new window before the leave
            // for the old one -- which cleared the window the pointer had
            // just entered, and its click went nowhere.
            // With -exit, run() is one pass of the event loop, which is
            // what reads the events pushed below; tick() does not.
            std::vector<std::string> argv = { "app", "-exit" };
            auto app = App::create(_context, argv, "ftk::ui_test::App", "Test UI application");
            std::vector<std::shared_ptr<Window> > windows;
            std::vector<int> clicks = { 0, 0 };
            for (size_t i = 0; i < 2; ++i)
            {
                auto window = Window::create(_context, app, Format("ftk::ui_test::App {0}").arg(i));
                auto layout = VerticalLayout::create(_context, window);
                auto button = PushButton::create(_context, "Button", layout);
                button->setHStretch(Stretch::Expanding);
                button->setVStretch(Stretch::Expanding);
                button->setClickedCallback([&clicks, i] { ++clicks[i]; });
                window->show();
                window->layout(Size2I(640, 480));
                windows.push_back(window);
            }
            app->tick();

            const auto push = [](SDL_Event& event)
            {
                SDL_PushEvent(&event);
            };
            const uint32_t a = windows[0]->getID();
            const uint32_t b = windows[1]->getID();
            SDL_Event e;
#if defined(FTK_SDL2)
            SDL_zero(e);
            e.type = SDL_WINDOWEVENT;
            e.window.event = SDL_WINDOWEVENT_ENTER;
            e.window.windowID = b;
            push(e);
            SDL_zero(e);
            e.type = SDL_WINDOWEVENT;
            e.window.event = SDL_WINDOWEVENT_LEAVE;
            e.window.windowID = a;
            push(e);
            SDL_zero(e);
            e.type = SDL_MOUSEMOTION;
            e.motion.windowID = b;
            e.motion.x = 20;
            e.motion.y = 20;
            push(e);
            SDL_zero(e);
            e.type = SDL_MOUSEBUTTONDOWN;
            e.button.windowID = b;
            e.button.button = SDL_BUTTON_LEFT;
            e.button.state = SDL_PRESSED;
            push(e);
            e.type = SDL_MOUSEBUTTONUP;
            e.button.state = SDL_RELEASED;
            push(e);
#elif defined(FTK_SDL3)
            SDL_zero(e);
            e.type = SDL_EVENT_WINDOW_MOUSE_ENTER;
            e.window.windowID = b;
            push(e);
            SDL_zero(e);
            e.type = SDL_EVENT_WINDOW_MOUSE_LEAVE;
            e.window.windowID = a;
            push(e);
            SDL_zero(e);
            e.type = SDL_EVENT_MOUSE_MOTION;
            e.motion.windowID = b;
            e.motion.x = 20.F;
            e.motion.y = 20.F;
            push(e);
            SDL_zero(e);
            e.type = SDL_EVENT_MOUSE_BUTTON_DOWN;
            e.button.windowID = b;
            e.button.button = SDL_BUTTON_LEFT;
            e.button.down = true;
            push(e);
            e.type = SDL_EVENT_MOUSE_BUTTON_UP;
            e.button.down = false;
            push(e);
#endif // FTK_SDL2
            app->run();
            _print(Format("Clicks: {0} {1}").arg(clicks[0]).arg(clicks[1]));
            FTK_CHECK(0 == clicks[0]);
            FTK_CHECK(1 == clicks[1]);

            for (const auto& window : windows)
            {
                window->close();
            }
            app->tick();
#endif // __EMSCRIPTEN__
        }
    }
}
