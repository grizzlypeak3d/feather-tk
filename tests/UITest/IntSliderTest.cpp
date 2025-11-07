// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/IntSliderTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/IntSlider.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        IntSliderTest::IntSliderTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::IntSliderTest")
        {}

        IntSliderTest::~IntSliderTest()
        {}

        std::shared_ptr<IntSliderTest> IntSliderTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<IntSliderTest>(new IntSliderTest(context));
        }
                
        void IntSliderTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("IntSliderTest");
                auto app = App::create(
                    context,
                    argv,
                    "IntSliderTest",
                    "Integer slider test.");
                auto window = Window::create(context, app, "IntSliderTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto slider = IntSlider::create(context, layout);
                FTK_ASSERT(slider->getModel());
                int value = 0;
                slider->setCallback([&value](int v) { value = v; });
                slider->setValue(11);
                app->tick();
                FTK_ASSERT(11 == slider->getValue());
                FTK_ASSERT(11 == value);
                slider->setRange(0, 10);
                app->tick();
                FTK_ASSERT(RangeI(0, 10) == slider->getRange());
                FTK_ASSERT(10 == value);
                slider->setStep(2);
                FTK_ASSERT(2 == slider->getStep());
                slider->setLargeStep(3);
                FTK_ASSERT(3 == slider->getLargeStep());
            }
        }
    }
}

