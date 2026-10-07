// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/ButtonTest.h>

#include <ftk/UI/CheckBox.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/ToolButton.h>
#include <ftk/UI/Tooltip.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Time.h>

namespace ftk
{
    namespace ui_test
    {
        ButtonTest::ButtonTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ButtonTest")
        {}

        ButtonTest::~ButtonTest()
        {}

        std::shared_ptr<ButtonTest> ButtonTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ButtonTest>(new ButtonTest(context));
        }

        void ButtonTest::run()
        {
            {
                std::vector<std::string> argv;
                argv.push_back("ButtonTest");
                auto app = App::create(
                    _context,
                    argv,
                    "ButtonTest",
                    "Button test.");
                auto window = Window::create(_context, app, "ButtonTest");
                auto layout = VerticalLayout::create(_context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                PushButton::create(_context, "Push", layout);

                std::shared_ptr<IButton> button = PushButton::create(_context, "Push", layout);
                button->setObjectName("PushButton");
                _print(button->getObjectName());
                _print(button->getObjectPath());
                _test(app, window, layout, button);
                button->setParent(nullptr);
                button.reset();

                button = ToolButton::create(_context, "Tool", layout);
                _test(app, window, layout, button);
                button->setParent(nullptr);
                button.reset();

                button = CheckBox::create(_context, "CheckBox", layout);
                _test(app, window, layout, button);
                std::string tooltip = "This is a tooltip";
                button->setTooltip(tooltip);
                FTK_CHECK(tooltip == button->getTooltip());
                button->setParent(nullptr);
                button.reset();
            }
        }

        void ButtonTest::_test(
            const std::shared_ptr<App>& app,
            const std::shared_ptr<IWindow>& window,
            const std::shared_ptr<VerticalLayout>& layout,
            const std::shared_ptr<IButton>& button)
        {
            FTK_CHECK(button->getParent());
            FTK_CHECK(button->getParentT<Window>());
            FTK_CHECK(button->getWindow());

            std::string text = "Playback";
            button->setText(text);
            button->setText(text);
            FTK_CHECK(text == button->getText());

            FontType font = FontType::Mono;
            button->setFont(font);
            button->setFont(font);
            FTK_CHECK(font == button->getFont());

            button->setCheckable(true);
            button->setCheckable(true);
            FTK_CHECK(button->isCheckable());
            button->setChecked(true);
            button->setChecked(true);
            FTK_CHECK(button->isChecked());
            button->setCheckable(false);
            FTK_CHECK(!button->isChecked());
            button->setCheckable(true);

            std::string icon = "PlaybackForward";
            button->setIcon(icon);
            button->setIcon(icon);
            FTK_CHECK(icon == button->getIcon());
            icon = "PlaybackStop";
            button->setCheckedIcon(icon);
            button->setCheckedIcon(icon);
            FTK_CHECK(icon == button->getCheckedIcon());

            ColorRole colorRole = ColorRole::Red;
            button->setBackgroundRole(colorRole);
            button->setBackgroundRole(colorRole);
            FTK_CHECK(colorRole == button->getBackgroundRole());
            colorRole = ColorRole::Green;
            button->setButtonRole(colorRole);
            button->setButtonRole(colorRole);
            FTK_CHECK(colorRole == button->getButtonRole());
            colorRole = ColorRole::Blue;
            button->setCheckedRole(colorRole);
            button->setCheckedRole(colorRole);
            FTK_CHECK(colorRole == button->getCheckedRole());

            button->hide();
            button->hide();
            app->tick();
            button->show();
            app->tick();

            button->setEnabled(false);
            button->setEnabled(false);
            app->tick();
            button->setEnabled(true);
            app->tick();

            bool hovered = false;
            button->setHoveredCallback([&hovered](bool value) { hovered = value; });
            bool pressed = false;
            button->setPressedCallback([&pressed] { pressed = true; });
            int clicks = 0;
            button->setRepeatClick(true);
            FTK_CHECK(button->hasRepeatClick());
            button->setClickedCallback([&clicks] { ++clicks; });
            bool checked = false;
            button->setCheckedCallback([&checked](bool value) { checked = value; });

            // A press that a touch gesture takes over is cancelled rather
            // than released: the button under the first finger does not
            // click.
            button->setRepeatClick(false);
            app->tick();
            const V2I c = center(button->getGeometry());
            window->click(c);
            FTK_CHECK(1 == clicks);
            window->drag({ c, c }, 0, false);
            window->gesture(c, V2F());
            FTK_CHECK(1 == clicks);
            window->click(c);
            FTK_CHECK(2 == clicks);
            clicks = 0;

            // A button acts on the release, so a press held by press()
            // has not clicked yet; the moves in between go to it as a
            // drag, and release() is what clicks.
            window->press(c);
            FTK_CHECK(0 == clicks);
            window->hover(V2I(c.x + 1, c.y));
            FTK_CHECK(0 == clicks);
            window->release(V2I(c.x + 1, c.y));
            FTK_CHECK(1 == clicks);
            clicks = 0;

            // A touch gesture leaves nothing hovered, until the mouse moves:
            // the cursor is where a finger was.
            window->hover(c);
            app->tick();
            FTK_CHECK(hovered);
            window->gesture(c, V2F());
            app->tick();
            FTK_CHECK(!hovered);
            window->hover(c);
            app->tick();
            FTK_CHECK(hovered);

            app->setDisplayScale(2.F);
            app->tick();
            app->setDisplayScale(1.F);
            app->tick();
        }
    }
}

