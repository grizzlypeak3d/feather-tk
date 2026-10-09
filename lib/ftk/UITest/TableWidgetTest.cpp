// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/TableWidgetTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/TableWidget.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>

namespace ftk
{
    namespace ui_test
    {
        TableWidgetTest::TableWidgetTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::TableWidgetTest")
        {}

        TableWidgetTest::~TableWidgetTest()
        {}

        std::shared_ptr<TableWidgetTest> TableWidgetTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<TableWidgetTest>(new TableWidgetTest(context));
        }

        void TableWidgetTest::run()
        {
            {
                const TableCell cell("Text", true, ColorRole::Red);
                FTK_CHECK("Text" == cell.text);
                FTK_CHECK(cell.editable);
                FTK_CHECK(ColorRole::Red == cell.colorRole);
                const TableRow row({ cell }, true);
                FTK_CHECK(1 == row.cells.size());
                FTK_CHECK(row.heading);
                FTK_CHECK(row.visible);
                FTK_CHECK(!TableIndex().isValid());
                FTK_CHECK(TableIndex(1, 2).isValid());
                FTK_CHECK(TableIndex(1, 2) != TableIndex(2, 1));
            }
            {
                std::vector<std::string> argv;
                argv.push_back("TableWidgetTest");
                auto app = App::create(
                    _context,
                    argv,
                    "TableWidgetTest",
                    "Table widget test.");
                auto window = Window::create(_context, app, "TableWidgetTest");
                window->show();
                app->tick();

                auto table = TableWidget::create(_context, window);
                std::vector<TableRow> rows =
                {
                    TableRow({ TableCell("Group A") }, true),
                    TableRow({ TableCell("One:"), TableCell("1", true), TableCell("", true) }),
                    TableRow({ TableCell("Two:"), TableCell("2", true), TableCell("", true) }),
                    TableRow({ TableCell("Group B") }, true),
                    TableRow({ TableCell("Three:"), TableCell("3", true), TableCell("", true) })
                };
                table->setRows(rows);
                table->setRows(rows);
                FTK_CHECK(rows == table->getRows());
                FTK_CHECK(3 == table->getColumnCount());
                FTK_CHECK(!table->getColumnStretch(1));
                table->setColumnStretch(1, true);
                table->setColumnStretch(1, true);
                table->setColumnStretch(2, true);
                FTK_CHECK(table->getColumnStretch(1));
                FTK_CHECK(!table->getColumnStretch(-1));
                table->setMarginRole(SizeRole::Margin);
                table->setMarginRole(SizeRole::Margin);
                FTK_CHECK(SizeRole::Margin == table->getMarginRole());
                FTK_CHECK(!table->hasColumnLines());
                table->setColumnLines(true);
                table->setColumnLines(true);
                FTK_CHECK(table->hasColumnLines());
                table->setHeadingRole(ColorRole::Header);
                table->setHeadingRole(ColorRole::Header);
                FTK_CHECK(ColorRole::Header == table->getHeadingRole());
                window->layout(Size2I(1280, 960));
                app->tick();

                // The cells of a row sit side by side, the stretched
                // columns share the spare width, and a heading other than
                // the first has space above it.
                const Box2I a = table->getCellRect(TableIndex(1, 0));
                const Box2I b = table->getCellRect(TableIndex(1, 1));
                const Box2I c = table->getCellRect(TableIndex(1, 2));
                FTK_CHECK(a.isValid() && b.isValid() && c.isValid());
                FTK_CHECK(a.max.x < b.min.x);
                FTK_CHECK(b.max.x < c.min.x);
                FTK_CHECK(b.w() > a.w());
                FTK_CHECK(std::abs(b.w() - c.w()) <= 1);
                FTK_CHECK(c.max.x <= table->getGeometry().max.x);
                const Box2I h0 = table->getCellRect(TableIndex(0, 0));
                const Box2I r2 = table->getCellRect(TableIndex(2, 0));
                const Box2I h1 = table->getCellRect(TableIndex(3, 0));
                FTK_CHECK(a.min.y == h0.min.y + h0.h());
                FTK_CHECK(h1.min.y > r2.min.y + r2.h());
                FTK_CHECK(!table->getCellRect(TableIndex(9, 0)).isValid());

                // Only an editable cell can be current.
                table->setCurrent(TableIndex(1, 0));
                FTK_CHECK(!table->getCurrent().isValid());
                table->setCurrent(TableIndex(1, 1));
                FTK_CHECK(TableIndex(1, 1) == table->getCurrent());
                table->setCurrent(TableIndex());

                // A click on an editable cell makes it current and calls
                // the callback; a click on another cell does neither.
                TableIndex clicked;
                int clicks = 0;
                table->setCallback(
                    [&clicked, &clicks](const TableIndex& value)
                    {
                        clicked = value;
                        ++clicks;
                    });
                window->hover(center(b));
                app->tick();
                window->click(center(a));
                app->tick();
                FTK_CHECK(0 == clicks);
                window->click(center(c));
                app->tick();
                FTK_CHECK(1 == clicks);
                FTK_CHECK(TableIndex(1, 2) == clicked);
                FTK_CHECK(TableIndex(1, 2) == table->getCurrent());
                FTK_CHECK(table->hasKeyFocus());

                // The keys move between the editable cells, over the
                // headings.
                window->keyPress(Key::Left);
                FTK_CHECK(TableIndex(1, 1) == table->getCurrent());
                window->keyPress(Key::Left);
                FTK_CHECK(TableIndex(1, 1) == table->getCurrent());
                window->keyPress(Key::Down);
                window->keyPress(Key::Down);
                FTK_CHECK(TableIndex(4, 1) == table->getCurrent());
                window->keyPress(Key::Down);
                FTK_CHECK(TableIndex(4, 1) == table->getCurrent());
                window->keyPress(Key::Right);
                FTK_CHECK(TableIndex(4, 2) == table->getCurrent());
                window->keyPress(Key::Home);
                FTK_CHECK(TableIndex(1, 2) == table->getCurrent());
                window->keyPress(Key::End);
                FTK_CHECK(TableIndex(4, 2) == table->getCurrent());
                window->keyPress(Key::Up);
                FTK_CHECK(TableIndex(2, 2) == table->getCurrent());
                window->keyPress(Key::Return);
                FTK_CHECK(2 == clicks);
                FTK_CHECK(TableIndex(2, 2) == clicked);
                app->tick();

                // An editor covers its cell, and is closed when asked.
                auto edit = LineEdit::create(_context);
                table->openEditor(TableIndex(2, 2), edit);
                edit->takeKeyFocus();
                app->tick();
                FTK_CHECK(edit == table->getEditor());
                FTK_CHECK(TableIndex(2, 2) == table->getEditorIndex());
                FTK_CHECK(edit->getParent() == table);
                const Box2I cell = table->getCellRect(TableIndex(2, 2));
                FTK_CHECK(edit->getGeometry().min.x == cell.min.x);
                FTK_CHECK(edit->getGeometry().w() == cell.w());
                table->closeEditor();
                FTK_CHECK(table->getEditor());
                app->tick();
                FTK_CHECK(!table->getEditor());
                FTK_CHECK(!edit->getParent());
                FTK_CHECK(table->hasKeyFocus());

                // An editor is closed when the key focus leaves it.
                table->openEditor(TableIndex(2, 2), edit);
                edit->takeKeyFocus();
                app->tick();
                FTK_CHECK(table->getEditor());
                table->takeKeyFocus();
                app->tick();
                FTK_CHECK(!table->getEditor());

                // An editor is not opened over a cell that cannot be
                // edited, and is closed when its row is hidden.
                table->openEditor(TableIndex(2, 0), edit);
                FTK_CHECK(!table->getEditor());
                table->openEditor(TableIndex(2, 2), edit);
                edit->takeKeyFocus();
                app->tick();
                rows[2].visible = false;
                table->setRows(rows);
                FTK_CHECK(!table->getEditor());
                FTK_CHECK(TableIndex(2, 2) != table->getCurrent());
                app->tick();
                FTK_CHECK(!table->getCellRect(TableIndex(2, 2)).isValid());

                // Hidden rows take no room.
                const Box2I h1b = table->getCellRect(TableIndex(3, 0));
                FTK_CHECK(h1b.min.y < h1.min.y);

                rows.clear();
                table->setRows(rows);
                app->tick();
                FTK_CHECK(0 == table->getColumnCount());
            }
        }
    }
}
