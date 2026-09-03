// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/FloatSliderTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/FloatSlider.h>
#include <ftk/UI/IButton.h>
#include <ftk/UI/Menu.h>
#include <ftk/UI/FloatEditSlider.h>
#include <ftk/UI/SliderPopup.h>
#include <ftk/UI/ToolButton.h>
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

                // The popup button replaces the sliders' context menu
                // (#829): the edit slider shows the small popup triangle,
                // and the popup holds the reset and the range.
                auto editSlider = FloatEditSlider::create(_context, layout);
                editSlider->setDefault(.25F);
                editSlider->setValue(.75F);
                app->tick();
                std::shared_ptr<IWidget> popupButton;
                for (const auto& child :
                    editSlider->getChildren().front()->getChildren())
                {
                    if (auto tmp = std::dynamic_pointer_cast<ToolButton>(child))
                    {
                        popupButton = tmp;
                    }
                }
                FTK_CHECK(popupButton);
                FTK_CHECK(popupButton->isVisible(false));
                const Box2I& buttonGeometry = popupButton->getGeometry();
                const V2I buttonCenter(
                    buttonGeometry.min.x + buttonGeometry.w() / 2,
                    buttonGeometry.min.y + buttonGeometry.h() / 2);
                window->click(buttonCenter, MouseButton::Left);
                app->tick();
                std::shared_ptr<FloatSliderPopup> popup;
                for (const auto& child : window->getChildren())
                {
                    if (auto tmp = std::dynamic_pointer_cast<FloatSliderPopup>(child))
                    {
                        popup = tmp;
                    }
                }
                FTK_CHECK(popup);
                FTK_CHECK(popup->isOpen());

                // Reset from the popup, the way a user does.
                auto resetButton = findButton(popup, "Reset");
                FTK_CHECK(resetButton);
                const Box2I& resetGeometry = resetButton->getGeometry();
                window->click(
                    V2I(
                        resetGeometry.min.x + resetGeometry.w() / 2,
                        resetGeometry.min.y + resetGeometry.h() / 2),
                    MouseButton::Left);
                app->tick();
                FTK_CHECK(.25F == editSlider->getValue());
                // At the default there is nothing to reset: the button
                // disabling is the feedback that the reset worked.
                FTK_CHECK(!resetButton->isEnabled(false));
                popup->close();
                app->tick();

                // The popup can be opened again after closing.
                window->click(buttonCenter, MouseButton::Left);
                app->tick();
                popup.reset();
                for (const auto& child : window->getChildren())
                {
                    if (auto tmp = std::dynamic_pointer_cast<FloatSliderPopup>(child))
                    {
                        popup = tmp;
                    }
                }
                FTK_CHECK(popup);
                // The button toggles: clicking it while its popup is open
                // closes the popup, the way a combo box does.
                window->click(buttonCenter, MouseButton::Left);
                app->tick();
                FTK_CHECK(!popup->isOpen());
                window->click(buttonCenter, MouseButton::Left);
                app->tick();
                popup.reset();
                for (const auto& child : window->getChildren())
                {
                    if (auto tmp = std::dynamic_pointer_cast<FloatSliderPopup>(child))
                    {
                        popup = tmp;
                    }
                }
                FTK_CHECK(popup);
                // The mouse put the focus down invisibly on the button that
                // opened the popup; Escape must close the popup on the first
                // press rather than silently releasing a focus nobody can
                // see.
                window->keyPress(Key::Escape);
                app->tick();
                FTK_CHECK(!popup->isOpen());

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

