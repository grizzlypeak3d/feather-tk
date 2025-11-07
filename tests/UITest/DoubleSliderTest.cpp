// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/DoubleSliderTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/DoubleSlider.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        DoubleSliderTest::DoubleSliderTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::DoubleSliderTest")
        {}

        DoubleSliderTest::~DoubleSliderTest()
        {}

        std::shared_ptr<DoubleSliderTest> DoubleSliderTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<DoubleSliderTest>(new DoubleSliderTest(context));
        }
                
        void DoubleSliderTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("DoubleSliderTest");
                auto app = App::create(
                    context,
                    argv,
                    "DoubleSliderTest",
                    "Double slider test.");
                auto window = Window::create(context, app, "DoubleSliderTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto slider = DoubleSlider::create(context, layout);
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
            }
        }
    }
}

