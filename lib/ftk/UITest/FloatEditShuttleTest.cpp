// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/FloatEditShuttleTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/FloatEditShuttle.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ShuttleWidget.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>

namespace ftk
{
    namespace ui_test
    {
        namespace
        {
            //! A window that can be laid out and dragged in, so that the
            //! shuttle can be driven the way a person drives it.
            class TestWindow : public Window
            {
            protected:
                TestWindow() = default;

            public:
                static std::shared_ptr<TestWindow> create(
                    const std::shared_ptr<Context>& context,
                    const std::shared_ptr<App>& app,
                    const std::string& title,
                    const Size2I& size)
                {
                    auto out = std::shared_ptr<TestWindow>(new TestWindow);
                    out->_init(context, app, title, size);
                    return out;
                }

                //! Lay the window out, without depending on the window system
                //! to deliver a resize.
                void resize(const Size2I& size)
                {
                    _setSize(size, size);
                }

                void drag(const V2I& from, const V2I& to, int modifiers = 0)
                {
                    _cursorEnter(true);
                    _cursorPos(from);
                    _mouseButton(MouseButton::Left, true, modifiers);
                    _cursorPos(to);
                    _mouseButton(MouseButton::Left, false, modifiers);
                }
            };

            //! The shuttle inside the widget, which is what has to be dragged.
            std::shared_ptr<ShuttleWidget> findShuttle(
                const std::shared_ptr<IWidget>& widget)
            {
                if (auto out = std::dynamic_pointer_cast<ShuttleWidget>(widget))
                    return out;
                for (const auto& child : widget->getChildren())
                {
                    if (auto out = findShuttle(child))
                        return out;
                }
                return nullptr;
            }
        }

        FloatEditShuttleTest::FloatEditShuttleTest(
            const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::FloatEditShuttleTest")
        {}

        FloatEditShuttleTest::~FloatEditShuttleTest()
        {}

        std::shared_ptr<FloatEditShuttleTest> FloatEditShuttleTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<FloatEditShuttleTest>(
                new FloatEditShuttleTest(context));
        }

        void FloatEditShuttleTest::run()
        {
            std::vector<std::string> argv;
            argv.push_back("FloatEditShuttleTest");
            auto app = App::create(
                _context,
                argv,
                "FloatEditShuttleTest",
                "Float edit shuttle test.");
            const Size2I size(1280, 960);
            auto window = TestWindow::create(
                _context, app, "FloatEditShuttleTest", size);
            auto layout = VerticalLayout::create(_context, window);
            layout->setMarginRole(SizeRole::MarginLarge);
            window->show();
            app->tick();
            window->resize(size);
            app->tick();

            auto shuttle = FloatEditShuttle::create(_context, layout);
            FTK_CHECK(shuttle->getModel());
            shuttle->setRange(-1000.F, 1000.F);
            shuttle->setStep(1.F);
            FTK_CHECK(1.F == shuttle->getStep());
            shuttle->setValue(0.F);
            shuttle->setDefault(0.F);
            FTK_CHECK(0.F == shuttle->getDefault());

            float value = 0.F;
            int count = 0;
            shuttle->setCallback(
                [&value, &count](float v) { value = v; ++count; });
            bool released = false;
            shuttle->setPressedCallback(
                [&released](float, bool pressed) { released = !pressed; });
            app->tick();
            window->resize(size);
            app->tick();

            // Dragged the way a person drags it. The value has to be reported
            // while the drag is happening: the shuttle writes to the model
            // without going through the edit, and a listener that only hears
            // about it on release is a listener watching a stale number.
            auto widget = findShuttle(shuttle);
            FTK_CHECK(widget);
            const Box2I& g = widget->getGeometry();
            FTK_CHECK(g.isValid());
            const V2I from(g.min.x + g.w() / 2, g.min.y + g.h() / 2);
            window->drag(from, V2I(from.x + g.h() * 2, from.y));
            app->tick();
            FTK_CHECK(count > 0);
            FTK_CHECK(value > 0.F);
            FTK_CHECK(value == shuttle->getValue());
            FTK_CHECK(released);

            // And the other way.
            const float forward = value;
            window->drag(from, V2I(from.x - g.h() * 2, from.y));
            app->tick();
            FTK_CHECK(shuttle->getValue() < forward);

            // What a notch is worth depends on what is held: Control is
            // finer, Shift is the large step. The same drag three ways has to
            // give three different distances, or the modifiers are being
            // read and thrown away.
            shuttle->setStep(1.F);
            shuttle->setLargeStep(10.F);
            const V2I to(from.x + g.h() * 2, from.y);

            shuttle->setValue(0.F);
            window->drag(from, to);
            app->tick();
            const float plain = shuttle->getValue();

            shuttle->setValue(0.F);
            window->drag(from, to, static_cast<int>(KeyModifier::Control));
            app->tick();
            const float fine = shuttle->getValue();

            shuttle->setValue(0.F);
            window->drag(from, to, static_cast<int>(KeyModifier::Shift));
            app->tick();
            const float coarse = shuttle->getValue();

            FTK_CHECK(plain > 0.F);
            FTK_CHECK(fine > 0.F);
            FTK_CHECK(fine < plain);
            FTK_CHECK(coarse > plain);
        }
    }
}
