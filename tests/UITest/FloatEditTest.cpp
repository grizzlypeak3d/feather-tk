// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/FloatEditTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/FloatEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        FloatEditTest::FloatEditTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::FloatEditTest")
        {}

        FloatEditTest::~FloatEditTest()
        {}

        std::shared_ptr<FloatEditTest> FloatEditTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<FloatEditTest>(new FloatEditTest(context));
        }

        void FloatEditTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("FloatEditTest");
                auto app = App::create(
                    context,
                    argv,
                    "FloatEditTest",
                    "Float edit test.");
                auto window = Window::create(context, app, "FloatEditTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto edit = FloatEdit::create(context, layout);
                FTK_ASSERT(edit->getModel());
                float value = 0.F;
                edit->setCallback([&value](float v) { value = v; });
                edit->setValue(.9F);
                app->tick();
                FTK_ASSERT(.9F == edit->getValue());
                edit->setRange(0.F, .5F);
                app->tick();
                FTK_ASSERT(RangeF(0.F, .5F) == edit->getRange());
                FTK_ASSERT(.5F == edit->getValue());
                edit->setStep(.2F);
                FTK_ASSERT(.2F == edit->getStep());
                edit->setLargeStep(.3F);
                FTK_ASSERT(.3F == edit->getLargeStep());
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

