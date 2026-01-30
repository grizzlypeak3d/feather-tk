// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/FloatSliderTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/FloatSlider.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

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
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("FloatSliderTest");
                auto app = App::create(
                    context,
                    argv,
                    "FloatSliderTest",
                    "Float slider test.");
                auto window = Window::create(context, app, "FloatSliderTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto slider = FloatSlider::create(context, layout);
                FTK_ASSERT(slider->getModel());
                float value = 0.F;
                slider->setCallback([&value](float v) { value = v; });
                slider->setValue(.9F);
                app->tick();
                FTK_ASSERT(.9F == slider->getValue());
                slider->setRange(0.F, .5F);
                app->tick();
                FTK_ASSERT(RangeF(0.F, .5F) == slider->getRange());
                FTK_ASSERT(.5F == slider->getValue());
                slider->setStep(.2F);
                FTK_ASSERT(.2F == slider->getStep());
                slider->setLargeStep(.3F);
                FTK_ASSERT(.3F == slider->getLargeStep());
            }
        }
    }
}

