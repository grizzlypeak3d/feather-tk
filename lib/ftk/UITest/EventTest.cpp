// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/EventTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Event.h>
#include <ftk/UI/IWidget.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        namespace
        {
            //! A window the test sends pen and mouse input to, as the
            //! application's event loop does from SDL's events.
            class PenWindow : public Window
            {
            protected:
                PenWindow() = default;

            public:
                static std::shared_ptr<PenWindow> create(
                    const std::shared_ptr<Context>& context,
                    const std::shared_ptr<App>& app)
                {
                    auto out = std::shared_ptr<PenWindow>(new PenWindow);
                    out->_init(context, app, "EventTest", Size2I(100, 100));
                    return out;
                }

                void resize(const Size2I& size) { _setSize(size, size); }
                void pen(bool down, float pressure, bool eraser) { _pen(down, pressure, eraser); }
                void cursorPos(const V2I& pos) { _cursorPos(pos); }
                void button(bool press) { _mouseButton(MouseButton::Left, press, 0); }
            };

            //! Keeps the pen state of the presses and moves it is given.
            class PenWidget : public IWidget
            {
            protected:
                PenWidget() = default;

            public:
                static std::shared_ptr<PenWidget> create(
                    const std::shared_ptr<Context>& context,
                    const std::shared_ptr<IWidget>& parent)
                {
                    auto out = std::shared_ptr<PenWidget>(new PenWidget);
                    out->_init(context, "PenWidget", parent);
                    return out;
                }

                void mousePressEvent(MouseClickEvent& event) override
                {
                    event.accept = true;
                    press = event;
                }

                void mouseMoveEvent(MouseMoveEvent& event) override
                {
                    move = event;
                }

                MouseClickEvent press;
                MouseMoveEvent move = MouseMoveEvent(V2I(), V2I());
            };
        }

        EventTest::EventTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::EventTest")
        {}

        EventTest::~EventTest()
        {}

        std::shared_ptr<EventTest> EventTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<EventTest>(new EventTest(context));
        }
                
        void EventTest::run()
        {
            _enums();
            _pen();
        }

        void EventTest::_pen()
        {
            // The pen's state comes before the mouse events it belongs to
            // and is stamped on them: how hard it presses, and whether it
            // is the eraser end. A mouse is no pen, presses with one, and
            // never erases.
            std::vector<std::string> argv;
            argv.push_back("EventTest");
            auto app = App::create(_context, argv, "EventTest", "Event test.");
            auto window = PenWindow::create(_context, app);
            auto widget = PenWidget::create(_context, window);
            window->show();
            window->resize(Size2I(100, 100));
            app->tick();

            window->cursorPos(V2I(10, 10));
            window->pen(true, .5F, true);
            window->button(true);
            FTK_CHECK(widget->press.pen);
            FTK_CHECK(.5F == widget->press.pressure);
            FTK_CHECK(widget->press.eraser);
            window->pen(true, .75F, true);
            window->cursorPos(V2I(20, 20));
            FTK_CHECK(widget->move.pen);
            FTK_CHECK(.75F == widget->move.pressure);
            FTK_CHECK(widget->move.eraser);
            window->pen(false, 1.F, false);
            window->button(false);

            window->pen(true, .25F, false);
            window->button(true);
            FTK_CHECK(widget->press.pen);
            FTK_CHECK(!widget->press.eraser);
            window->pen(false, 1.F, false);
            window->button(false);

            window->button(true);
            FTK_CHECK(!widget->press.pen);
            FTK_CHECK(1.F == widget->press.pressure);
            FTK_CHECK(!widget->press.eraser);
            window->button(false);
        }
        
        void EventTest::_enums()
        {
            FTK_TEST_ENUM(Key);
            _print(getKeyModifierLabel(static_cast<int>(KeyModifier::None)));
            _print(getKeyModifierLabel(static_cast<int>(KeyModifier::Shift)));
            _print(getKeyModifierLabel(static_cast<int>(KeyModifier::Control)));
            _print(getKeyModifierLabel(static_cast<int>(KeyModifier::Alt)));
            _print(getKeyModifierLabel(static_cast<int>(KeyModifier::Super)));
            _print(getShortcutLabel(Key::O, static_cast<int>(KeyModifier::Control)));
        }
    }
}

