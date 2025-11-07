// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/DoubleEditTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/DoubleEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        DoubleEditTest::DoubleEditTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::DoubleEditTest")
        {}

        DoubleEditTest::~DoubleEditTest()
        {}

        std::shared_ptr<DoubleEditTest> DoubleEditTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<DoubleEditTest>(new DoubleEditTest(context));
        }
                
        void DoubleEditTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("DoubleEditTest");
                auto app = App::create(
                    context,
                    argv,
                    "DoubleEditTest",
                    "Double edit test.");
                auto window = Window::create(context, app, "DoubleEditTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto edit = DoubleEdit::create(context, layout);
                FTK_ASSERT(edit->getModel());
                double value = 0.0;
                edit->setCallback([&value](double v) { value = v; });
                edit->setValue(0.9);
                app->tick();
                FTK_ASSERT(0.9 == edit->getValue());
                FTK_ASSERT(0.9 == value);
                edit->setRange(0.0, 0.5);
                app->tick();
                FTK_ASSERT(RangeD(0.0, 0.5) == edit->getRange());
                FTK_ASSERT(0.5 == value);
                edit->setStep(0.2);
                FTK_ASSERT(0.2 == edit->getStep());
                edit->setLargeStep(0.3);
                FTK_ASSERT(0.3 == edit->getLargeStep());
                edit->setPrecision(3);
                edit->setPrecision(3);
                FTK_ASSERT(3 == edit->getPrecision());
                edit->setPrecision(2);
                edit->setFontRole(FontRole::Label);
                FTK_ASSERT(FontRole::Label == edit->getFontRole());
                edit->setFontRole(FontRole::Mono);
            }
        }
    }
}

