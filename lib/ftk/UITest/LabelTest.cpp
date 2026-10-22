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
                // Asking for the whole text's width, and eliding only when
                // given less.
                label->setElideFullWidth(true);
                label->setElideFullWidth(true);
                FTK_CHECK(label->getElideFullWidth());
                app->tick();
                FTK_CHECK(textWidth == label->getSizeHint().w);
                label->setElideFullWidth(false);
                app->tick();
                FTK_CHECK(label->getSizeHint().w < textWidth);
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

                // Wrapping: narrower and taller, and back again.
                label->setText("one two three four five six seven eight nine ten eleven twelve");
                app->tick();
                const Size2I unwrapped = label->getSizeHint();
                label->setWrap(20);
                label->setWrap(20);
                FTK_CHECK(20 == label->getWrap());
                app->tick();
                FTK_CHECK(label->getSizeHint().w < unwrapped.w);
                FTK_CHECK(label->getSizeHint().h > unwrapped.h);
                // A word longer than the width is left whole, and a line
                // break in the text is kept.
                label->setText("supercalifragilisticexpialidocious\nb");
                app->tick();

                // Wrapping to fit: across the room the label is given, which
                // is wider here than twenty characters, so fewer lines; the
                // size hint stays as narrow as the wrap width.
                label->setText("one two three four five six seven eight nine ten eleven twelve");
                app->tick();
                const Size2I wrapped = label->getSizeHint();
                label->setWrapToFit(true);
                label->setWrapToFit(true);
                FTK_CHECK(label->getWrapToFit());
                FTK_CHECK(HAlign::Fill == label->getHAlign());
                app->tick();
                app->tick();
                FTK_CHECK(label->getGeometry().w() > wrapped.w);
                FTK_CHECK(label->getSizeHint().w <= wrapped.w);
                FTK_CHECK(label->getSizeHint().h < wrapped.h);
                // And narrower than the wrap width when there is less room:
                // a width no window here is as wide as still fits.
                std::string longText;
                for (int i = 0; i < 20; ++i)
                {
                    longText += "one two three four five six seven eight nine ten ";
                }
                label->setText(longText);
                label->setWrap(1000);
                app->tick();
                app->tick();
                FTK_CHECK(label->getSizeHint().h > unwrapped.h);
                label->setText("one two three four five six seven eight nine ten eleven twelve");
                label->setWrap(20);
                app->tick();
                app->tick();
                label->setWrapToFit(false);
                FTK_CHECK(!label->getWrapToFit());
                FTK_CHECK(HAlign::Left == label->getHAlign());
                app->tick();
                FTK_CHECK(label->getSizeHint() == wrapped);

                label->setWrap(-1);
                FTK_CHECK(0 == label->getWrap());
                label->setText("one two three four five six seven eight nine ten eleven twelve");
                app->tick();
                FTK_CHECK(label->getSizeHint() == unwrapped);

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

