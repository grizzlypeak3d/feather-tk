// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/LineEditTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        LineEditTest::LineEditTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::LineEditTest")
        {}

        LineEditTest::~LineEditTest()
        {}

        std::shared_ptr<LineEditTest> LineEditTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<LineEditTest>(new LineEditTest(context));
        }
                
        void LineEditTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("LineEditTest");
                auto app = App::create(
                    context,
                    argv,
                    "LineEditTest",
                    "Line edit test.");
                auto window = Window::create(context, app, "LineEditTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto edit = LineEdit::create(context, layout);
                std::string text;
                edit->setCallback(
                    [&text](const std::string& value)
                    {
                        text = value;
                    });
                std::string textChanged;
                edit->setTextChangedCallback(
                    [&textChanged](const std::string& value)
                    {
                        textChanged = value;
                    });
                edit->setText("Test");
                edit->setText("Test");
                FTK_ASSERT("Test" == edit->getText());
                edit->clearText();
                FTK_ASSERT(edit->getText().empty());
                edit->setFormat("00.00");
                edit->setFormat("00.00");
                FTK_ASSERT("00.00" == edit->getFormat());
                edit->setFormat("");
                edit->setFontRole(FontRole::Mono);
                edit->setFontRole(FontRole::Mono);
                FTK_ASSERT(FontRole::Mono == edit->getFontRole());
                edit->setFontRole(FontRole::Label);

                edit->takeKeyFocus();
                app->tick();
                edit->hide();
                app->tick();
                edit->show();
                app->tick();
                FTK_ASSERT(!edit->hasKeyFocus());

                edit->takeKeyFocus();
                app->tick();
                edit->setEnabled(false);
                app->tick();
                edit->setEnabled(true);
                app->tick();
                FTK_ASSERT(!edit->hasKeyFocus());
            }
        }
    }
}

