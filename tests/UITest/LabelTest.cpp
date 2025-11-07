// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/LabelTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        LabelTest::LabelTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::LabelTest")
        {}

        LabelTest::~LabelTest()
        {}

        std::shared_ptr<LabelTest> LabelTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<LabelTest>(new LabelTest(context));
        }
                
        void LabelTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("LabelTest");
                auto app = App::create(
                    context,
                    argv,
                    "LabelTest",
                    "Label test.");
                auto window = Window::create(context, app, "LabelTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto label = Label::create(context, layout);
                label->setText("Test");
                label->setText("Test");
                FTK_ASSERT("Test" == label->getText());
                label->setTextRole(ColorRole::Red);
                label->setTextRole(ColorRole::Red);
                FTK_ASSERT(ColorRole::Red == label->getTextRole());
                label->setMarginRole(SizeRole::Margin);
                label->setMarginRole(SizeRole::Margin);
                FTK_ASSERT(SizeRole::Margin == label->getHMarginRole());
                FTK_ASSERT(SizeRole::Margin == label->getVMarginRole());
                label->setMarginRole(SizeRole::Margin, SizeRole::None);
                label->setMarginRole(SizeRole::Margin, SizeRole::None);
                FTK_ASSERT(SizeRole::Margin == label->getHMarginRole());
                FTK_ASSERT(SizeRole::None == label->getVMarginRole());
                label->setHMarginRole(SizeRole::None);
                label->setHMarginRole(SizeRole::None);
                FTK_ASSERT(SizeRole::None == label->getHMarginRole());
                label->setVMarginRole(SizeRole::Margin);
                label->setVMarginRole(SizeRole::Margin);
                FTK_ASSERT(SizeRole::Margin == label->getVMarginRole());
                label->setFontRole(FontRole::Mono);
                label->setFontRole(FontRole::Mono);
                FTK_ASSERT(FontRole::Mono == label->getFontRole());

                label->setEnabled(false);
                app->tick();
                label->setEnabled(true);
                app->tick();

                label->hide();
                app->tick();
                label->show();
                app->tick();
            }
        }
    }
}

