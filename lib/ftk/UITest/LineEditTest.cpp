// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/LineEditTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/LineEditModel.h>
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
            {
                std::vector<std::string> argv;
                argv.push_back("LineEditTest");
                auto app = App::create(
                    _context,
                    argv,
                    "LineEditTest",
                    "Line edit test.");
                auto window = Window::create(_context, app, "LineEditTest");
                auto layout = VerticalLayout::create(_context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto edit = LineEdit::create(_context, layout);
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
                edit->setFont(FontType::Mono);
                edit->setFont(FontType::Mono);
                FTK_ASSERT(FontType::Mono == edit->getFont());
                edit->setFont(FontType::Regular);

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

                // Undo/redo.
                {
                    auto undoEdit = LineEdit::create(_context, layout);
                    auto model = undoEdit->getModel();

                    bool hasUndo = false;
                    bool hasRedo = false;
                    auto hasUndoObserver = Observer<bool>::create(
                        model->observeHasUndo(),
                        [&hasUndo](bool value) { hasUndo = value; });
                    auto hasRedoObserver = Observer<bool>::create(
                        model->observeHasRedo(),
                        [&hasRedo](bool value) { hasRedo = value; });
                    FTK_ASSERT(!hasUndo);
                    FTK_ASSERT(!hasRedo);

                    // Type three characters, then undo back to empty and
                    // redo back to "abc".
                    model->input("a");
                    model->input("b");
                    model->input("c");
                    FTK_ASSERT("abc" == model->getText());
                    FTK_ASSERT(3 == model->getCursor());
                    FTK_ASSERT(hasUndo);
                    FTK_ASSERT(!hasRedo);
                    model->undo();
                    FTK_ASSERT("ab" == model->getText());
                    FTK_ASSERT(2 == model->getCursor());
                    model->undo();
                    model->undo();
                    FTK_ASSERT(model->getText().empty());
                    FTK_ASSERT(!hasUndo);
                    FTK_ASSERT(hasRedo);
                    model->redo();
                    model->redo();
                    model->redo();
                    FTK_ASSERT("abc" == model->getText());
                    FTK_ASSERT(hasUndo);
                    FTK_ASSERT(!hasRedo);

                    // Undo of a selection-replace restores the selection.
                    model->setText("hello");
                    FTK_ASSERT(!hasUndo);
                    model->setSelection(LineEditSelection(0, 5));
                    model->input("X");
                    FTK_ASSERT("X" == model->getText());
                    model->undo();
                    FTK_ASSERT("hello" == model->getText());
                    FTK_ASSERT(LineEditSelection(0, 5) == model->getSelection());

                    // A new edit after an undo truncates the redo branch.
                    model->setText("ab");
                    model->setCursor(2);
                    model->input("c");
                    model->undo();
                    FTK_ASSERT("ab" == model->getText());
                    FTK_ASSERT(hasRedo);
                    model->input("d");
                    FTK_ASSERT("abd" == model->getText());
                    FTK_ASSERT(!hasRedo);

                    // Setting the text clears the undo history.
                    model->input("e");
                    FTK_ASSERT(hasUndo);
                    model->setText("zzz");
                    FTK_ASSERT(!hasUndo);
                    FTK_ASSERT(!hasRedo);
                }
            }
        }
    }
}

