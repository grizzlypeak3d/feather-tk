// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/IntEditTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/IntEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        IntEditTest::IntEditTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::IntEditTest")
        {}

        IntEditTest::~IntEditTest()
        {}

        std::shared_ptr<IntEditTest> IntEditTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<IntEditTest>(new IntEditTest(context));
        }
                
        void IntEditTest::run()
        {
            {
                std::vector<std::string> argv;
                argv.push_back("IntEditTest");
                auto app = App::create(
                    _context,
                    argv,
                    "IntEditTest",
                    "Integer edit test.");
                auto window = Window::create(_context, app, "IntEditTest");
                auto layout = VerticalLayout::create(_context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto edit = IntEdit::create(_context, layout);
                FTK_ASSERT(edit->getModel());
                int value = 0;
                edit->setCallback([&value](int v) { value = v; });
                edit->setValue(11);
                app->tick();
                FTK_ASSERT(11 == edit->getValue());
                edit->setRange(0, 10);
                app->tick();
                FTK_ASSERT(RangeI(0, 10) == edit->getRange());
                FTK_ASSERT(10 == edit->getValue());
                edit->setStep(2);
                FTK_ASSERT(2 == edit->getStep());
                edit->setLargeStep(3);
                FTK_ASSERT(3 == edit->getLargeStep());
                edit->setFont(FontType::Regular);
                FTK_ASSERT(FontType::Regular == edit->getFont());
                edit->setFont(FontType::Mono);
            }
        }
    }
}

