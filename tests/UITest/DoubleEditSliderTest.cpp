// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/DoubleEditSliderTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/DoubleEditSlider.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        DoubleEditSliderTest::DoubleEditSliderTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::DoubleEditSliderTest")
        {}

        DoubleEditSliderTest::~DoubleEditSliderTest()
        {}

        std::shared_ptr<DoubleEditSliderTest> DoubleEditSliderTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<DoubleEditSliderTest>(new DoubleEditSliderTest(context));
        }
                
        void DoubleEditSliderTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("DoubleEditSliderTest");
                auto app = App::create(
                    context,
                    argv,
                    "DoubleEditSliderTest",
                    "Double edit slider test.");
                auto window = Window::create(context, app, "DoubleEditSliderTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto slider = DoubleEditSlider::create(context, layout);
                FTK_ASSERT(slider->getModel());
                double value = 0.0;
                slider->setCallback([&value](double v) { value = v; });
                slider->setValue(0.9);
                app->tick();
                FTK_ASSERT(0.9 == slider->getValue());
                FTK_ASSERT(0.9 == value);
                slider->setRange(0.0, 0.5);
                app->tick();
                FTK_ASSERT(RangeD(0.0, 0.5) == slider->getRange());
                FTK_ASSERT(0.5 == value);
                slider->setStep(0.2);
                FTK_ASSERT(0.2 == slider->getStep());
                slider->setLargeStep(0.3);
                FTK_ASSERT(0.3 == slider->getLargeStep());
                slider->setPrecision(3);
                FTK_ASSERT(3 == slider->getPrecision());
                slider->setPrecision(2);
                slider->setDefaultValue(0.0);
                FTK_ASSERT(0.0 == slider->getDefaultValue());
                slider->setFontRole(FontRole::Label);
                FTK_ASSERT(FontRole::Label == slider->getFontRole());
                slider->setFontRole(FontRole::Mono);
            }
        }
    }
}

