// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/TextEditModelTest.h>

#include <ftk/UI/ClipboardSystem.h>
#include <ftk/UI/TextEditModel.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/String.h>

namespace ftk
{
    namespace ui_test
    {
        TextEditModelTest::TextEditModelTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::TextEditModelTest")
        {}

        TextEditModelTest::~TextEditModelTest()
        {}

        std::shared_ptr<TextEditModelTest> TextEditModelTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<TextEditModelTest>(new TextEditModelTest(context));
        }
                
        void TextEditModelTest::run()
        {
            {
                auto model = TextEditModel::create(_context);
                std::vector<std::string> text =
                {
                    "abcdefghijklmnopqrstuvwxyz",
                    "",
                    "0123456789"
                };
                model->setText(text);
                model->setText(text);
                FTK_ASSERT(text == model->getText());

                std::vector<std::string> text2;
                auto observer = ListObserver<std::string>::create(
                    model->observeText(),
                    [&text2](const std::vector<std::string>& value)
                    {
                        text2 = value;
                    });
                FTK_ASSERT(text2 == text);
                model->clearText();
                FTK_ASSERT(text2 == std::vector<std::string>({ "" }));
            }
            {
                auto model = TextEditModel::create(_context);
                FTK_ASSERT(model->key(Key::A));
                FTK_ASSERT(model->key(Key::A, static_cast<int>(commandKeyModifier)));
            }
            {
                auto model = TextEditModel::create(_context);
                std::vector<std::string> text =
                {
                    "abcdefghijklmnopqrstuvwxyz",
                    "",
                    "0123456789"
                };
                model->setText(text);

                TextEditPos cursor(0, 0);
                model->setCursor(cursor);
                model->setCursor(cursor);
                FTK_ASSERT(cursor == model->getCursor());

                TextEditPos cursor2;
                auto observer = Observer<TextEditPos>::create(
                    model->observeCursor(),
                    [&cursor2](const TextEditPos& value)
                    {
                        cursor2 = value;
                    });
                cursor.chr = 1000;
                model->setCursor(cursor);
                FTK_ASSERT(cursor2.chr == static_cast<int>(text[0].size()));
                cursor.line = 1000;
                model->setCursor(cursor);
                FTK_ASSERT(cursor2.line == static_cast<int>(text.size()) - 1);
                FTK_ASSERT(cursor2.chr == static_cast<int>(text.back().size()));
            }
            {
                auto model = TextEditModel::create(_context);
                std::vector<std::string> text =
                {
                    "abcdefghijklmnopqrstuvwxyz",
                    "",
                    "0123456789"
                };
                model->setText(text);

                TextEditSelection selection(
                    TextEditPos(0, 0),
                    TextEditPos(0, 5));
                model->setSelection(selection);
                model->setSelection(selection);
                FTK_ASSERT(selection == model->getSelection());

                TextEditSelection selection2;
                auto observer = Observer<TextEditSelection>::create(
                    model->observeSelection(),
                    [&selection2](const TextEditSelection& value)
                    {
                        selection2 = value;
                    });
                selection.first.chr = 1000;
                model->setSelection(selection);
                FTK_ASSERT(selection2.first.chr == static_cast<int>(text.front().size()));
                selection.second.line = 1000;
                model->setSelection(selection);
                FTK_ASSERT(selection2.second.line == static_cast<int>(text.size()) - 1);
                FTK_ASSERT(selection2.second.chr == 5);

                model->clearSelection();
                FTK_ASSERT(!selection2.isValid());
                model->selectAll();
                FTK_ASSERT(selection2.isValid());
                FTK_ASSERT(selection2.second.line == static_cast<int>(text.size()) - 1);
                FTK_ASSERT(selection2.second.chr == static_cast<int>(text.back().size()));
            }
            {
                auto model = TextEditModel::create(_context);
                std::vector<std::string> text =
                {
                    "abcdefghijklmnopqrstuvwxyz",
                    "",
                    "0123456789"
                };
                model->setText(text);

                std::vector<std::string> text2;
                auto observer = ListObserver<std::string>::create(
                    model->observeText(),
                    [&text2](const std::vector<std::string>& value)
                    {
                        text2 = value;
                    });

                model->input("345");
                FTK_ASSERT(text2[0] == "345abcdefghijklmnopqrstuvwxyz");
                model->setCursor(TextEditPos(0, 0));
                model->input("012");
                FTK_ASSERT(text2[0] == "012345abcdefghijklmnopqrstuvwxyz");
                model->setCursor(TextEditPos(0, 6));
                model->input("6789");
                FTK_ASSERT(text2[0] == "0123456789abcdefghijklmnopqrstuvwxyz");

                model->selectAll();
                model->input("abcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[0] == "abcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(!model->getSelection().isValid());

                model->setSelection(TextEditSelection(
                    TextEditPos(0, 0),
                    TextEditPos(0, 1)));
                model->input("0");
                FTK_ASSERT(text2[0] == "0bcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(!model->getSelection().isValid());

                model->setSelection(TextEditSelection(
                    TextEditPos(0, 1),
                    TextEditPos(0, 23)));
                model->input("123456789");
                FTK_ASSERT(text2[0] == "0123456789xyz");
                FTK_ASSERT(!model->getSelection().isValid());

                model->setSelection(TextEditSelection(
                    TextEditPos(0, 9),
                    TextEditPos(0, 13)));
                model->input("9");
                FTK_ASSERT(text2[0] == "0123456789");
                FTK_ASSERT(!model->getSelection().isValid());
            }
            {
                auto model = TextEditModel::create(_context);
                std::vector<std::string> text =
                {};
                model->setText(text);

                std::vector<std::string> text2;
                auto textObserver = ListObserver<std::string>::create(
                    model->observeText(),
                    [&text2](const std::vector<std::string>& value)
                    {
                        text2 = value;
                    });
                TextEditPos cursor2;
                auto cursorObserver = Observer<TextEditPos>::create(
                    model->observeCursor(),
                    [&cursor2](const TextEditPos& value)
                    {
                        cursor2 = value;
                    });
                TextEditSelection selection2;
                auto selectionObserver = Observer<TextEditSelection>::create(
                    model->observeSelection(),
                    [&selection2](const TextEditSelection& value)
                    {
                        selection2 = value;
                    });

                auto clipboard = _context->getSystem<ClipboardSystem>();
                clipboard->setText("\n0123456789\nabcdefghijklmnopqrstuvwxyz\n");

                model->paste();
                FTK_ASSERT(text2[0] == "");
                FTK_ASSERT(text2[1] == "0123456789");
                FTK_ASSERT(text2[2] == "abcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[3] == "");
                FTK_ASSERT(TextEditPos(3, 0) == cursor2);
                
                model->selectAll();
                model->paste();
                FTK_ASSERT(text2[0] == "");
                FTK_ASSERT(text2[1] == "0123456789");
                FTK_ASSERT(text2[2] == "abcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[3] == "");
                FTK_ASSERT(TextEditPos(3, 0) == cursor2);

                model->selectAll();
                model->setText({ "!!!!!!!!!!" });
                model->setCursor(TextEditPos(0, 5));
                model->paste();
                FTK_ASSERT(text2[0] == "!!!!!");
                FTK_ASSERT(text2[1] == "0123456789");
                FTK_ASSERT(text2[2] == "abcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[3] == "!!!!!");
                FTK_ASSERT(TextEditPos(3, 0) == cursor2);

                model->selectAll();
                model->setText({ "!!!!!!!!!!" });
                model->setSelection(TextEditSelection(
                    TextEditPos(0, 1),
                    TextEditPos(0, 9)));
                model->paste();
                FTK_ASSERT(text2[0] == "!");
                FTK_ASSERT(text2[1] == "0123456789");
                FTK_ASSERT(text2[2] == "abcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[3] == "!");
                FTK_ASSERT(TextEditPos(3, 0) == cursor2);
                FTK_ASSERT(!selection2.isValid());

                model->selectAll();
                model->setText({ "!!!!!!!!!!", "##########" });
                model->setSelection(TextEditSelection(
                    TextEditPos(0, 5),
                    TextEditPos(1, 5)));
                model->paste();
                FTK_ASSERT(text2[0] == "!!!!!");
                FTK_ASSERT(text2[1] == "0123456789");
                FTK_ASSERT(text2[2] == "abcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[3] == "#####");
                FTK_ASSERT(TextEditPos(3, 0) == cursor2);
                FTK_ASSERT(!selection2.isValid());

                model->selectAll();
                model->setText({ "!!!!!!!!!!", "##########" });
                model->setSelection(TextEditSelection(
                    TextEditPos(0, 5),
                    TextEditPos(1, 5)));
                clipboard->setText("0123456789");
                model->paste();
                FTK_ASSERT(text2[0] == "!!!!!0123456789#####");
                FTK_ASSERT(TextEditPos(0, 15) == cursor2);
                FTK_ASSERT(!selection2.isValid());

                model->setText({ "!!!!!!!!!!", "0123456789", "abcdefghijklmnopqrstuvwxyz", "##########" });
                model->setSelection(TextEditSelection(
                    TextEditPos(0, 0),
                    TextEditPos(0, 1)));
                model->copy();
                FTK_ASSERT("!" == clipboard->getText());

                model->setSelection(TextEditSelection(
                    TextEditPos(1, 5),
                    TextEditPos(2, 5)));
                model->copy();
                auto tmp = splitLines(clipboard->getText());
                FTK_ASSERT(tmp == std::vector<std::string>({ "56789", "abcde" }));

                model->setSelection(TextEditSelection(
                    TextEditPos(0, 0),
                    TextEditPos(0, 1)));
                model->cut();
                FTK_ASSERT("!" == clipboard->getText());
                FTK_ASSERT(text2[0] == "!!!!!!!!!");
                FTK_ASSERT(text2[1] == "0123456789");
                FTK_ASSERT(text2[2] == "abcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[3] == "##########");
                FTK_ASSERT(TextEditPos(0, 0) == cursor2);
                FTK_ASSERT(!selection2.isValid());

                model->setSelection(TextEditSelection(
                    TextEditPos(1, 5),
                    TextEditPos(2, 5)));
                model->cut();
                tmp = splitLines(clipboard->getText());
                FTK_ASSERT(tmp == std::vector<std::string>({ "56789", "abcde" }));
                FTK_ASSERT(text2[0] == "!!!!!!!!!");
                FTK_ASSERT(text2[1] == "01234fghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[2] == "##########");
                FTK_ASSERT(TextEditPos(1, 5) == cursor2);
                FTK_ASSERT(!selection2.isValid());

                model->selectAll();
                model->cut();
                tmp = splitLines(clipboard->getText());
                FTK_ASSERT(tmp[0] == "!!!!!!!!!");
                FTK_ASSERT(tmp[1] == "01234fghijklmnopqrstuvwxyz");
                FTK_ASSERT(tmp[2] == "##########");
                FTK_ASSERT(text2 == std::vector<std::string>({ "" }));
                FTK_ASSERT(TextEditPos(0, 0) == cursor2);
                FTK_ASSERT(!selection2.isValid());
            }
            {
                auto model = TextEditModel::create(_context);
                std::vector<std::string> text =
                {
                    "abcdefghijklmnopqrstuvwxyz",
                    "",
                    "0123456789"
                };
                model->setText(text);

                std::vector<std::string> text2;
                auto observer = ListObserver<std::string>::create(
                    model->observeText(),
                    [&text2](const std::vector<std::string>& value)
                    {
                        text2 = value;
                    });

                model->setCursor(TextEditPos(0, 0));
                model->key(Key::Left);
                FTK_ASSERT(TextEditPos(0, 0) == model->getCursor());
                model->key(Key::Right);
                FTK_ASSERT(TextEditPos(0, 1) == model->getCursor());
                for (int i = 0; i < text.size(); ++i)
                {
                    for (int j = 0; j <= text[i].size(); ++j)
                    {
                        model->key(Key::Right);
                    }
                }
                TextEditPos cursor(
                    static_cast<int>(text.size()) - 1,
                    static_cast<int>(text.back().size()));
                FTK_ASSERT(cursor == model->getCursor());
                model->key(Key::Right);
                FTK_ASSERT(cursor == model->getCursor());

                model->setCursor(TextEditPos(0, 0));
                model->key(Key::End);
                FTK_ASSERT(TextEditPos(0, static_cast<int>(text.front().size())) == model->getCursor());
                model->key(Key::Home);
                FTK_ASSERT(TextEditPos(0, 0) == model->getCursor());
                model->key(Key::Up);
                FTK_ASSERT(TextEditPos(0, 0) == model->getCursor());
                model->key(Key::Down);
                FTK_ASSERT(TextEditPos(1, 0) == model->getCursor());
                for (int i = 0; i <= text.size(); ++i)
                {
                    model->key(Key::Down);
                }
                cursor = TextEditPos(static_cast<int>(text.size()) - 1, 0);
                FTK_ASSERT(cursor == model->getCursor());
                model->key(Key::Down);
                FTK_ASSERT(cursor == model->getCursor());
            }
            {
                auto model = TextEditModel::create(_context);
                std::vector<std::string> text =
                {
                    "abcdefghijklmnopqrstuvwxyz",
                    "",
                    "0123456789"
                };
                model->setText(text);

                std::vector<std::string> text2;
                auto textObserver = ListObserver<std::string>::create(
                    model->observeText(),
                    [&text2](const std::vector<std::string>& value)
                    {
                        text2 = value;
                    });
                TextEditPos cursor2;
                auto cursorObserver = Observer<TextEditPos>::create(
                    model->observeCursor(),
                    [&cursor2](const TextEditPos& value)
                    {
                        cursor2 = value;
                    });
                TextEditSelection selection2;
                auto selectionObserver = Observer<TextEditSelection>::create(
                    model->observeSelection(),
                    [&selection2](const TextEditSelection& value)
                    {
                        selection2 = value;
                    });

                model->setCursor(TextEditPos(0, 0));
                model->key(Key::Right, static_cast<int>(KeyModifier::Shift));
                FTK_ASSERT(TextEditSelection(
                    TextEditPos(0, 0),
                    TextEditPos(0, 1)) == selection2);
                for (int i = 0; i < static_cast<int>(text.size()); ++i)
                {
                    for (int j = 0; j <= text[i].size(); ++j)
                    {
                        model->key(Key::Right, static_cast<int>(KeyModifier::Shift));
                    }
                }
                model->key(Key::Right, static_cast<int>(KeyModifier::Shift));
                TextEditPos cursor(TextEditPos(2, text[2].size()));
                FTK_ASSERT(cursor == cursor2);
                FTK_ASSERT(TextEditSelection(TextEditPos(0, 0), cursor) == selection2);

                model->clearSelection();
                model->key(Key::Left, static_cast<int>(KeyModifier::Shift));
                model->key(Key::Left, static_cast<int>(KeyModifier::Shift));
                FTK_ASSERT(TextEditSelection(
                    cursor,
                    TextEditPos(cursor.line, cursor.chr - 2)) == selection2);
                for (int i = static_cast<int>(text.size()) - 1; i >= 0; --i)
                {
                    for (int j = static_cast<int>(text[i].size()); j >= 0; --j)
                    {
                        model->key(Key::Left, static_cast<int>(KeyModifier::Shift));
                    }
                }
                FTK_ASSERT(TextEditSelection(cursor, TextEditPos(0, 0)) == selection2);

                model->clearSelection();
                cursor = TextEditPos(0, 0);
                model->setCursor(cursor);
                model->setPageRows(2);
                model->key(Key::PageDown, static_cast<int>(KeyModifier::Shift));
                model->key(Key::PageDown, static_cast<int>(KeyModifier::Shift));
                FTK_ASSERT(TextEditSelection(cursor, cursor2) == selection2);

                model->clearSelection();
                cursor = cursor2;
                model->setPageRows(2);
                model->key(Key::PageUp, static_cast<int>(KeyModifier::Shift));
                model->key(Key::PageUp, static_cast<int>(KeyModifier::Shift));
                FTK_ASSERT(TextEditSelection(cursor, cursor2) == selection2);
            }
            {
                auto model = TextEditModel::create(_context);
                std::vector<std::string> text =
                {
                    "abcdefghijklmnopqrstuvwxyz",
                    "   ",
                    "   0123456789"
                };
                model->setText(text);

                std::vector<std::string> text2;
                auto textObserver = ListObserver<std::string>::create(
                    model->observeText(),
                    [&text2](const std::vector<std::string>& value)
                    {
                        text2 = value;
                    });

                model->selectAll();
                model->key(Key::Tab);
                FTK_ASSERT(text2[0] == "    abcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[1] == "       ");
                FTK_ASSERT(text2[2] == "       0123456789");
                model->key(Key::Tab, static_cast<int>(KeyModifier::Shift));
                FTK_ASSERT(text2[0] == "abcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[1] == "   ");
                FTK_ASSERT(text2[2] == "   0123456789");
                model->key(Key::Tab, static_cast<int>(KeyModifier::Shift));
                FTK_ASSERT(text2[0] == "abcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[1] == "");
                FTK_ASSERT(text2[2] == "0123456789");

                model->setSelection(TextEditSelection(
                    TextEditPos(2, 1),
                    TextEditPos(2, 2)));
                model->key(Key::Tab);
                FTK_ASSERT(text2[0] == "abcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[1] == "");
                FTK_ASSERT(text2[2] == "    0123456789");

                model->setCursor(TextEditPos(0, 0));
                model->key(Key::Tab);
                FTK_ASSERT(text2[0] == "    abcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[1] == "");
                FTK_ASSERT(text2[2] == "    0123456789");
            }
            {
                auto model = TextEditModel::create(_context);
                std::vector<std::string> text =
                {
                    "abcdefghijklmnopqrstuvwxyz",
                    "!!!!!!!!!!",
                    "0123456789"
                };
                model->setText(text);

                std::vector<std::string> text2;
                auto textObserver = ListObserver<std::string>::create(
                    model->observeText(),
                    [&text2](const std::vector<std::string>& value)
                    {
                        text2 = value;
                    });

                model->key(Key::Backspace);
                model->key(Key::Backspace);
                FTK_ASSERT(TextEditPos(0, 0) == model->getCursor());
                model->setCursor(TextEditPos(text.size() - 1, text.back().size()));
                model->key(Key::Backspace);
                FTK_ASSERT(text2[2] == "012345678");
                while (!text2[2].empty())
                {
                    model->key(Key::Backspace);
                }
                model->key(Key::Backspace);
                FTK_ASSERT(text2.size() == 2);
                model->setCursor(TextEditPos(1, 0));
                model->key(Key::Backspace);
                FTK_ASSERT(text2.size() == 1);
                FTK_ASSERT(text2[0] == "abcdefghijklmnopqrstuvwxyz!!!!!!!!!!");

                model->setSelection(TextEditSelection(
                    TextEditPos(0, 5),
                    TextEditPos(0, 26)));
                model->key(Key::Backspace);
                FTK_ASSERT(text2[0] == "abcde!!!!!!!!!!");
            }
            {
                auto model = TextEditModel::create(_context);
                std::vector<std::string> text =
                {
                    "abcdefghijklmnopqrstuvwxyz",
                    "!!!!!!!!!!",
                    "0123456789"
                };
                model->setText(text);

                std::vector<std::string> text2;
                auto textObserver = ListObserver<std::string>::create(
                    model->observeText(),
                    [&text2](const std::vector<std::string>& value)
                    {
                        text2 = value;
                    });

                TextEditPos cursor(text.size() - 1, text.back().size());
                model->setCursor(cursor);
                model->key(Key::Delete);
                model->key(Key::Delete);
                FTK_ASSERT(cursor == model->getCursor());
                model->setCursor(TextEditPos(0, 0));
                model->key(Key::Delete);
                FTK_ASSERT(text2[0] == "bcdefghijklmnopqrstuvwxyz");
                while (!text2[0].empty())
                {
                    model->key(Key::Delete);
                }
                model->key(Key::Delete);
                FTK_ASSERT(text2.size() == 2);
                model->setCursor(TextEditPos(0, text2[0].size()));
                model->key(Key::Delete);
                FTK_ASSERT(text2.size() == 1);
                FTK_ASSERT(text2[0] == "!!!!!!!!!!0123456789");

                model->setSelection(TextEditSelection(
                    TextEditPos(0, 5),
                    TextEditPos(0, 15)));
                model->key(Key::Delete);
                FTK_ASSERT(text2[0] == "!!!!!56789");
            }
            {
                auto model = TextEditModel::create(_context);
                std::vector<std::string> text =
                {
                    "abcdefghijklmnopqrstuvwxyz",
                    "!!!!!!!!!!",
                    "0123456789"
                };
                model->setText(text);

                std::vector<std::string> text2;
                auto textObserver = ListObserver<std::string>::create(
                    model->observeText(),
                    [&text2](const std::vector<std::string>& value)
                    {
                        text2 = value;
                    });
                TextEditPos cursor2;
                auto observer = Observer<TextEditPos>::create(
                    model->observeCursor(),
                    [&cursor2](const TextEditPos& value)
                    {
                        cursor2 = value;
                    });

                model->key(Key::Return);
                FTK_ASSERT(text2.size() == 4);
                FTK_ASSERT(text2[0] == "");
                FTK_ASSERT(cursor2 == TextEditPos(1, 0));

                model->key(Key::Right);
                model->key(Key::Return);
                FTK_ASSERT(text2.size() == 5);
                FTK_ASSERT(text2[0] == "");
                FTK_ASSERT(text2[1] == "a");
                FTK_ASSERT(text2[2] == "bcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[3] == "!!!!!!!!!!");
                FTK_ASSERT(text2[4] == "0123456789");
                FTK_ASSERT(cursor2 == TextEditPos(2, 0));

                model->key(Key::End);
                model->key(Key::Return);
                FTK_ASSERT(text2.size() == 6);
                FTK_ASSERT(text2[0] == "");
                FTK_ASSERT(text2[1] == "a");
                FTK_ASSERT(text2[2] == "bcdefghijklmnopqrstuvwxyz");
                FTK_ASSERT(text2[3] == "");
                FTK_ASSERT(text2[4] == "!!!!!!!!!!");
                FTK_ASSERT(text2[5] == "0123456789");
                FTK_ASSERT(cursor2 == TextEditPos(3, 0));
            }
            {
                // Undo/redo.
                auto model = TextEditModel::create(_context);
                model->setText({ "abc" });

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

                // Type a character, then undo and redo it.
                model->setCursor(TextEditPos(0, 3));
                model->input("d");
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "abcd" }));
                FTK_ASSERT(model->getCursor() == TextEditPos(0, 4));
                FTK_ASSERT(hasUndo);
                FTK_ASSERT(!hasRedo);
                model->undo();
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "abc" }));
                FTK_ASSERT(model->getCursor() == TextEditPos(0, 3));
                FTK_ASSERT(!hasUndo);
                FTK_ASSERT(hasRedo);
                model->redo();
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "abcd" }));
                FTK_ASSERT(model->getCursor() == TextEditPos(0, 4));
                FTK_ASSERT(!hasRedo);

                // Splitting a line with Return, then undo restores the joined
                // line and the cursor.
                model->setText({ "abc" });
                FTK_ASSERT(!hasUndo);
                model->setCursor(TextEditPos(0, 1));
                model->key(Key::Return);
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "a", "bc" }));
                FTK_ASSERT(model->getCursor() == TextEditPos(1, 0));
                model->undo();
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "abc" }));
                FTK_ASSERT(model->getCursor() == TextEditPos(0, 1));
                model->redo();
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "a", "bc" }));

                // Undo of a multi-line paste restores the original lines, the
                // cursor, and the selection.
                model->setText({ "AAA", "BBB" });
                model->setSelection(TextEditSelection(
                    TextEditPos(0, 1),
                    TextEditPos(1, 1)));
                auto clipboard = _context->getSystem<ClipboardSystem>();
                clipboard->setText("1\n2");
                model->paste();
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "A1", "2BB" }));
                FTK_ASSERT(model->getCursor() == TextEditPos(1, 1));
                FTK_ASSERT(!model->getSelection().isValid());
                model->undo();
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "AAA", "BBB" }));
                FTK_ASSERT(model->getCursor() == TextEditPos(0, 0));
                FTK_ASSERT(model->getSelection() == TextEditSelection(
                    TextEditPos(0, 1),
                    TextEditPos(1, 1)));

                // Undo of a selection-replace restores the selection.
                model->setText({ "hello" });
                model->setSelection(TextEditSelection(
                    TextEditPos(0, 0),
                    TextEditPos(0, 5)));
                model->input("X");
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "X" }));
                model->undo();
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "hello" }));
                FTK_ASSERT(model->getSelection() == TextEditSelection(
                    TextEditPos(0, 0),
                    TextEditPos(0, 5)));

                // A new edit after an undo truncates the redo branch.
                model->setText({ "ab" });
                model->setCursor(TextEditPos(0, 2));
                model->input("c");
                model->undo();
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "ab" }));
                FTK_ASSERT(hasRedo);
                model->input("d");
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "abd" }));
                FTK_ASSERT(!hasRedo);

                // Setting the text clears the undo history.
                model->input("e");
                FTK_ASSERT(hasUndo);
                model->setText({ "zzz" });
                FTK_ASSERT(!hasUndo);
                FTK_ASSERT(!hasRedo);
            }
            {
                // Inserting multi-line text within a single line: the last
                // pasted line merges before the remainder of the original
                // line (regression test for the ordering in _replace).
                auto model = TextEditModel::create(_context);
                model->setText({ "xy" });

                auto clipboard = _context->getSystem<ClipboardSystem>();
                clipboard->setText("1\n2");
                model->setCursor(TextEditPos(0, 1));
                model->paste();
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "x1", "2y" }));
                FTK_ASSERT(model->getCursor() == TextEditPos(1, 1));

                model->undo();
                FTK_ASSERT(model->getText() == std::vector<std::string>({ "xy" }));
                FTK_ASSERT(model->getCursor() == TextEditPos(0, 1));
            }
        }
    }
}

