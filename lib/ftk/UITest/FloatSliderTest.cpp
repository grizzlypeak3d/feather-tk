// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/FloatSliderTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/FloatSlider.h>
#include <ftk/UI/IButton.h>
#include <ftk/UI/Menu.h>
#include <ftk/UI/RangePopup.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace
{
    std::shared_ptr<ftk::IButton> findButton(
        const std::shared_ptr<ftk::IWidget>& widget,
        const std::string& text)
    {
        if (auto button = std::dynamic_pointer_cast<ftk::IButton>(widget))
        {
            if (text == button->getText())
            {
                return button;
            }
        }
        for (const auto& child : widget->getChildren())
        {
            if (auto button = findButton(child, text))
            {
                return button;
            }
        }
        return nullptr;
    }
}

namespace ftk
{
    namespace ui_test
    {
        FloatSliderTest::FloatSliderTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::FloatSliderTest")
        {}

        FloatSliderTest::~FloatSliderTest()
        {}

        std::shared_ptr<FloatSliderTest> FloatSliderTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<FloatSliderTest>(new FloatSliderTest(context));
        }

        void FloatSliderTest::run()
        {
            {
                std::vector<std::string> argv;
                argv.push_back("FloatSliderTest");
                auto app = App::create(
                    _context,
                    argv,
                    "FloatSliderTest",
                    "Float slider test.");
                auto window = Window::create(_context, app, "FloatSliderTest");
                auto layout = VerticalLayout::create(_context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto slider = FloatSlider::create(_context, layout);
                FTK_CHECK(slider->getModel());
                float value = 0.F;
                slider->setCallback([&value](float v) { value = v; });
                // Programmatic changes notify like any other: the
                // callback reports every value change, including the
                // clamp from a shrinking range.
                slider->setValue(.9F);
                app->tick();
                FTK_CHECK(.9F == slider->getValue());
                FTK_CHECK(.9F == value);
                slider->setRange(0.F, .5F);
                app->tick();
                FTK_CHECK(RangeF(0.F, .5F) == slider->getRange());
                FTK_CHECK(.5F == slider->getValue());
                FTK_CHECK(.5F == value);
                slider->setStep(.2F);
                FTK_CHECK(.2F == slider->getStep());
                slider->setLargeStep(.3F);
                FTK_CHECK(.3F == slider->getLargeStep());
            }
            {
                const Size2I windowSize(1280, 960);
                std::vector<std::string> argv;
                argv.push_back("FloatSliderTest");
                auto app = App::create(
                    _context,
                    argv,
                    "FloatSliderTest",
                    "Float slider test.");
                auto window = Window::create(_context, app, "FloatSliderTest");
                auto layout = VerticalLayout::create(_context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                auto slider = FloatSlider::create(_context, layout);
                window->show();
                window->layout(windowSize);
                app->tick();

                const Box2I& g = slider->getGeometry();
                const V2I center(g.min.x + g.w() / 2, g.min.y + g.h() / 2);

                // A double click resets to the default value; the release
                // of the second click does not set the value back to the
                // click position.
                slider->setDefault(.25F);
                window->click(center, MouseButton::Left);
                window->click(center, MouseButton::Left);
                app->tick();
                FTK_CHECK(.25F == slider->getValue());

                // A right click opens the context menu without moving the
                // value.
                slider->setValue(.75F);
                window->click(center, MouseButton::Right);
                app->tick();
                FTK_CHECK(.75F == slider->getValue());
                std::shared_ptr<Menu> menu;
                for (const auto& child : window->getChildren())
                {
                    if (auto tmp = std::dynamic_pointer_cast<Menu>(child))
                    {
                        menu = tmp;
                    }
                }
                FTK_CHECK(menu);

                // A hard range is a fixed bound, so the menu only offers
                // the reset; a soft range adds Set Range.
                FTK_CHECK(1 == menu->getActions().size());
                FTK_CHECK("Reset" == menu->getActions()[0]->getText());
                menu->close();
                app->tick();
                slider->getModel()->setRangeSoft(true);
                window->click(center, MouseButton::Right);
                app->tick();
                menu.reset();
                for (const auto& child : window->getChildren())
                {
                    if (auto tmp = std::dynamic_pointer_cast<Menu>(child))
                    {
                        menu = tmp;
                    }
                }
                FTK_CHECK(menu);
                FTK_CHECK(2 == menu->getActions().size());
                FTK_CHECK("Reset" == menu->getActions()[0]->getText());
                FTK_CHECK("Set Range..." == menu->getActions()[1]->getText());

                // Reset from the menu.
                menu->getActions()[0]->doCallback();
                menu->close();
                app->tick();
                FTK_CHECK(.25F == slider->getValue());

                // Clicking the menu's range item, the way a user does,
                // closes the menu and opens the range popup.
                window->click(center, MouseButton::Right);
                app->tick();
                menu.reset();
                for (const auto& child : window->getChildren())
                {
                    if (auto tmp = std::dynamic_pointer_cast<Menu>(child))
                    {
                        menu = tmp;
                    }
                }
                FTK_CHECK(menu);
                auto item = findButton(menu, "Set Range...");
                FTK_CHECK(item);
                const Box2I& itemGeometry = item->getGeometry();
                window->click(
                    V2I(
                        itemGeometry.min.x + itemGeometry.w() / 2,
                        itemGeometry.min.y + itemGeometry.h() / 2),
                    MouseButton::Left);
                app->tick();
                std::shared_ptr<FloatRangePopup> popup;
                for (const auto& child : window->getChildren())
                {
                    if (auto tmp = std::dynamic_pointer_cast<FloatRangePopup>(child))
                    {
                        popup = tmp;
                    }
                }
                FTK_CHECK(popup);
                FTK_CHECK(popup->isOpen());
                popup->close();
                app->tick();

                // The popup can be opened again after closing.
                window->click(center, MouseButton::Right);
                app->tick();
                menu.reset();
                for (const auto& child : window->getChildren())
                {
                    if (auto tmp = std::dynamic_pointer_cast<Menu>(child))
                    {
                        menu = tmp;
                    }
                }
                FTK_CHECK(menu);
                menu->getActions()[1]->doCallback();
                menu->close();
                app->tick();
                popup.reset();
                for (const auto& child : window->getChildren())
                {
                    if (auto tmp = std::dynamic_pointer_cast<FloatRangePopup>(child))
                    {
                        popup = tmp;
                    }
                }
                FTK_CHECK(popup);
                popup->close();
                app->tick();

                // Dragging a soft ranged slider past its ends clamps at
                // the range instead of extending it; extending on drag fed
                // back into the position mapping and ran the range away.
                slider->getModel()->clearDefault();
                slider->getModel()->setRangeSoft(true);
                window->drag({ center, V2I(windowSize.w + 200, center.y) });
                app->tick();
                FTK_CHECK(RangeF(0.F, 1.F) == slider->getRange());
                FTK_CHECK(1.F == slider->getValue());
                window->drag({ center, V2I(-200, center.y) });
                app->tick();
                FTK_CHECK(RangeF(0.F, 1.F) == slider->getRange());
                FTK_CHECK(0.F == slider->getValue());
            }
        }
    }
}

