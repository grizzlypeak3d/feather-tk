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
                FTK_CHECK("Test" == edit->getText());
                edit->clearText();
                FTK_CHECK(edit->getText().empty());
                edit->setFormat("00.00");
                edit->setFormat("00.00");
                FTK_CHECK("00.00" == edit->getFormat());
                edit->setFormat("");
                edit->setFont(FontType::Mono);
                edit->setFont(FontType::Mono);
                FTK_CHECK(FontType::Mono == edit->getFont());
                edit->setFont(FontType::Regular);

                edit->takeKeyFocus();
                app->tick();
                edit->hide();
                app->tick();
                edit->show();
                app->tick();
                FTK_CHECK(!edit->hasKeyFocus());

                edit->takeKeyFocus();
                app->tick();
                edit->setEnabled(false);
                app->tick();
                edit->setEnabled(true);
                app->tick();
                FTK_CHECK(!edit->hasKeyFocus());

                // Selecting all on focus, the way the numeric edits do.
                edit->setText("Test");
                edit->setSelectAllOnFocus(true);
                FTK_CHECK(edit->hasSelectAllOnFocus());
                edit->takeKeyFocus();
                app->tick();
                const LineEditSelection& selection =
                    edit->getModel()->getSelection();
                FTK_CHECK(selection.isValid());
                FTK_CHECK(0 == selection.min());
                FTK_CHECK(4 == selection.max());
                edit->releaseKeyFocus();
                edit->setSelectAllOnFocus(false);

                // UTF-8: the cursor moves and deletes whole code
                // points, never splitting a character. "a", then a two
                // byte e-acute, then a three byte kanji.
                edit->setText("a\xC3\xA9\xE6\x84\x9F");
                auto model = edit->getModel();
                model->key(Key::End, 0);
                FTK_CHECK(6 == model->getCursor());
                model->key(Key::Left, 0);
                FTK_CHECK(3 == model->getCursor());
                model->key(Key::Left, 0);
                FTK_CHECK(1 == model->getCursor());
                model->key(Key::Left, 0);
                FTK_CHECK(0 == model->getCursor());
                model->key(Key::End, 0);
                model->key(Key::Backspace, 0);
                FTK_CHECK("a\xC3\xA9" == edit->getText());
                model->key(Key::Home, 0);
                model->key(Key::Right, 0);
                model->key(Key::Delete, 0);
                FTK_CHECK("a" == edit->getText());

                // An input method composition shows as the preedit until
                // it is committed as text input or canceled.
                edit->clearText();
                edit->takeKeyFocus();
                app->tick();
                window->textEditing("\xE3\x81\x8B\xE3\x82\x93", 2);
                app->tick();
                FTK_CHECK("\xE3\x81\x8B\xE3\x82\x93" == edit->getPreedit());
                FTK_CHECK(edit->getText().empty());
                window->text("\xE6\x84\x9F");
                app->tick();
                FTK_CHECK(edit->getPreedit().empty());
                FTK_CHECK("\xE6\x84\x9F" == edit->getText());
                window->textEditing("x", 1);
                FTK_CHECK("x" == edit->getPreedit());
                window->textEditing("", 0);
                FTK_CHECK(edit->getPreedit().empty());
                edit->releaseKeyFocus();
                app->tick();

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
                    FTK_CHECK(!hasUndo);
                    FTK_CHECK(!hasRedo);

                    // Type three characters, then undo back to empty and
                    // redo back to "abc".
                    model->input("a");
                    model->input("b");
                    model->input("c");
                    FTK_CHECK("abc" == model->getText());
                    FTK_CHECK(3 == model->getCursor());
                    FTK_CHECK(hasUndo);
                    FTK_CHECK(!hasRedo);
                    model->undo();
                    FTK_CHECK("ab" == model->getText());
                    FTK_CHECK(2 == model->getCursor());
                    model->undo();
                    model->undo();
                    FTK_CHECK(model->getText().empty());
                    FTK_CHECK(!hasUndo);
                    FTK_CHECK(hasRedo);
                    model->redo();
                    model->redo();
                    model->redo();
                    FTK_CHECK("abc" == model->getText());
                    FTK_CHECK(hasUndo);
                    FTK_CHECK(!hasRedo);

                    // Undo of a selection-replace restores the selection.
                    model->setText("hello");
                    FTK_CHECK(!hasUndo);
                    model->setSelection(LineEditSelection(0, 5));
                    model->input("X");
                    FTK_CHECK("X" == model->getText());
                    model->undo();
                    FTK_CHECK("hello" == model->getText());
                    FTK_CHECK(LineEditSelection(0, 5) == model->getSelection());

                    // A new edit after an undo truncates the redo branch.
                    model->setText("ab");
                    model->setCursor(2);
                    model->input("c");
                    model->undo();
                    FTK_CHECK("ab" == model->getText());
                    FTK_CHECK(hasRedo);
                    model->input("d");
                    FTK_CHECK("abd" == model->getText());
                    FTK_CHECK(!hasRedo);

                    // Setting the text clears the undo history.
                    model->input("e");
                    FTK_CHECK(hasUndo);
                    model->setText("zzz");
                    FTK_CHECK(!hasUndo);
                    FTK_CHECK(!hasRedo);
                }
            }
        }
    }
}

