// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/IntEditSliderTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/IntEditSlider.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        IntEditSliderTest::IntEditSliderTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::IntEditSliderTest")
        {}

        IntEditSliderTest::~IntEditSliderTest()
        {}

        std::shared_ptr<IntEditSliderTest> IntEditSliderTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<IntEditSliderTest>(new IntEditSliderTest(context));
        }
                
        void IntEditSliderTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("IntEditSliderTest");
                auto app = App::create(
                    context,
                    argv,
                    "IntEditSliderTest",
                    "Integer edit slider test.");
                auto window = Window::create(context, app, "IntEditSliderTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto slider = IntEditSlider::create(context, layout);
                FTK_ASSERT(slider->getModel());
                int value = 0;
                slider->setCallback([&value](int v) { value = v; });
                slider->setValue(11);
                app->tick();
                FTK_ASSERT(11 == slider->getValue());
                slider->setRange(0, 10);
                app->tick();
                FTK_ASSERT(RangeI(0, 10) == slider->getRange());
                FTK_ASSERT(10 == slider->getValue());
                slider->setStep(2);
                FTK_ASSERT(2 == slider->getStep());
                slider->setLargeStep(3);
                FTK_ASSERT(3 == slider->getLargeStep());
                slider->setDefaultValue(0);
                FTK_ASSERT(0 == slider->getDefaultValue());
                slider->setFontRole(FontRole::Label);
                FTK_ASSERT(FontRole::Label == slider->getFontRole());
                slider->setFontRole(FontRole::Mono);
            }
        }
    }
}

