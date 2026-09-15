// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/LabelTest.h>

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
            {
                std::vector<std::string> argv;
                argv.push_back("LabelTest");
                auto app = App::create(
                    _context,
                    argv,
                    "LabelTest",
                    "Label test.");
                auto window = Window::create(_context, app, "LabelTest");
                auto layout = VerticalLayout::create(_context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto label = Label::create(_context, layout);
                label->setText("Test");
                label->setText("Test");
                FTK_CHECK("Test" == label->getText());
                label->setTextRole(ColorRole::Red);
                label->setTextRole(ColorRole::Red);
                FTK_CHECK(ColorRole::Red == label->getTextRole());
                label->setMarginRole(SizeRole::Margin);
                label->setMarginRole(SizeRole::Margin);
                FTK_CHECK(SizeRole::Margin == label->getHMarginRole());
                FTK_CHECK(SizeRole::Margin == label->getVMarginRole());
                label->setMarginRole(SizeRole::Margin, SizeRole::None);
                label->setMarginRole(SizeRole::Margin, SizeRole::None);
                FTK_CHECK(SizeRole::Margin == label->getHMarginRole());
                FTK_CHECK(SizeRole::None == label->getVMarginRole());
                label->setHMarginRole(SizeRole::None);
                label->setHMarginRole(SizeRole::None);
                FTK_CHECK(SizeRole::None == label->getHMarginRole());
                label->setVMarginRole(SizeRole::Margin);
                label->setVMarginRole(SizeRole::Margin);
                FTK_CHECK(SizeRole::Margin == label->getVMarginRole());
                label->setFont(FontType::Mono);
                label->setFont(FontType::Mono);
                FTK_CHECK(FontType::Mono == label->getFont());

                // Eliding: the size hint stops following the text, and the
                // text is cut to the geometry when drawn.
                label->setText("A long line of text that is wider than any room it will be given");
                app->tick();
                const int textWidth = label->getSizeHint().w;
                label->setElide(true, ElideMode::Middle);
                label->setElide(true, ElideMode::Middle);
                FTK_CHECK(label->getElide());
                FTK_CHECK(ElideMode::Middle == label->getElideMode());
                FTK_CHECK(HAlign::Fill == label->getHAlign());
                app->tick();
                FTK_CHECK(label->getSizeHint().w < textWidth);
                // Laid out across the room it is given, not at its hint.
                FTK_CHECK(label->getGeometry().w() > label->getSizeHint().w);
                label->setElide(true, ElideMode::Left);
                FTK_CHECK(ElideMode::Left == label->getElideMode());
                app->tick();
                label->setElide(true, ElideMode::Right);
                app->tick();
                label->setText("大平原 \xF0\x9F\x8E\xAC wide characters");
                app->tick();
                label->setElide(false);
                FTK_CHECK(!label->getElide());
                FTK_CHECK(HAlign::Left == label->getHAlign());
                app->tick();
                FTK_CHECK(label->getSizeHint().w >= textWidth / 4);

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

