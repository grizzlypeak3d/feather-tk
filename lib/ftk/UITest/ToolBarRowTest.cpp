// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/ToolBarRowTest.h>

#include <ftk/UI/Action.h>
#include <ftk/UI/App.h>
#include <ftk/UI/Divider.h>
#include <ftk/UI/ToolBar.h>
#include <ftk/UI/ToolBarRow.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>

namespace ftk
{
    namespace ui_test
    {
        ToolBarRowTest::ToolBarRowTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ToolBarRowTest")
        {}

        ToolBarRowTest::~ToolBarRowTest()
        {}

        std::shared_ptr<ToolBarRowTest> ToolBarRowTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ToolBarRowTest>(new ToolBarRowTest(context));
        }

        void ToolBarRowTest::run()
        {
            std::vector<std::string> argv;
            argv.push_back("ToolBarRowTest");
            auto app = App::create(
                _context,
                argv,
                "ToolBarRowTest",
                "Tool bar row test.");
            auto window = Window::create(_context, app, "ToolBarRowTest");
            window->show();
            app->tick();

            auto row = ToolBarRow::create(_context, window);
            row->setMarginRole(SizeRole::None);
            FTK_CHECK(SizeRole::None == row->getMarginRole());
            row->setSpacingRole(SizeRole::SpacingSmall);
            FTK_CHECK(SizeRole::SpacingSmall == row->getSpacingRole());

            // Three tool bars of four actions each.
            std::vector<std::shared_ptr<ToolBar> > toolBars;
            for (size_t i = 0; i < 3; ++i)
            {
                auto toolBar = ToolBar::create(_context);
                for (int j = 0; j < 4; ++j)
                {
                    toolBar->addAction(Action::create("Action", "Add", [] {}));
                }
                row->addToolBar(toolBar);
                toolBars.push_back(toolBar);
            }
            FTK_CHECK(4 == toolBars[0]->getActions().size());
            FTK_CHECK(4 == toolBars[0]->getOverflowMax());
            FTK_CHECK(!row->isKeepLast());
            app->tick();

            // Room for all of them: nothing overflows, left to right.
            const Size2I hint = row->getSizeHint();
            row->setGeometry(Box2I(0, 0, hint.w, hint.h));
            FTK_CHECK(row->getOverflow().empty());
            FTK_CHECK(toolBars[0]->getGeometry().min.x < toolBars[1]->getGeometry().min.x);
            FTK_CHECK(toolBars[1]->getGeometry().min.x < toolBars[2]->getGeometry().min.x);
            FTK_CHECK(toolBars[2]->getGeometry().max.x < hint.w);

            // A little short: buttons go from the end of the last tool bar,
            // enough for the menu button too, and the tool bar stays.
            row->setGeometry(Box2I(0, 0, hint.w - 4, hint.h));
            auto overflow = row->getOverflow();
            FTK_CHECK(1 == overflow.size());
            FTK_CHECK(!overflow.empty() && toolBars[2] == overflow[0]);
            const size_t short2 = toolBars[2]->getOverflow();
            FTK_CHECK(short2 >= 1 && short2 < 4);
            FTK_CHECK(short2 == toolBars[2]->getOverflowActions().size());
            FTK_CHECK(0 == toolBars[1]->getOverflow());
            FTK_CHECK(toolBars[2]->getGeometry().max.x <= row->getGeometry().max.x);
            // The size hint is still what the row would like.
            FTK_CHECK(hint == row->getSizeHint());

            // The same width again changes nothing.
            row->setGeometry(Box2I(0, 0, hint.w - 4, hint.h));
            FTK_CHECK(short2 == toolBars[2]->getOverflow());

            // Room for not much more than one tool bar: the last goes
            // altogether, with its divider, the middle one in part, and
            // the first is where it was.
            const int one = toolBars[0]->getOverflowWidth(0) * 2;
            row->setGeometry(Box2I(0, 0, one, hint.h));
            FTK_CHECK(4 == toolBars[2]->getOverflow());
            FTK_CHECK(toolBars[1]->getOverflow() > 0);
            FTK_CHECK(0 == toolBars[0]->getOverflow());
            FTK_CHECK(toolBars[0]->getGeometry().min.x == 0);
            FTK_CHECK(toolBars[2]->getGeometry().min.x > row->getGeometry().max.x);

            // With the last kept, the buttons before it go first; the last
            // follows the first.
            row->setKeepLast(true);
            FTK_CHECK(row->isKeepLast());
            row->setGeometry(Box2I(0, 0, hint.w - 4, hint.h));
            FTK_CHECK(0 == toolBars[2]->getOverflow());
            FTK_CHECK(short2 == toolBars[1]->getOverflow());
            row->setGeometry(Box2I(0, 0, one, hint.h));
            FTK_CHECK(0 == toolBars[2]->getOverflow());
            FTK_CHECK(4 == toolBars[1]->getOverflow());
            FTK_CHECK(toolBars[0]->getOverflow() > 0 && toolBars[0]->getOverflow() < 4);
            FTK_CHECK(toolBars[2]->getGeometry().min.x > toolBars[0]->getGeometry().min.x);
            row->setKeepLast(false);

            // Back to full width, everything is back.
            row->setGeometry(Box2I(0, 0, hint.w, hint.h));
            FTK_CHECK(row->getOverflow().empty());
            FTK_CHECK(0 == toolBars[1]->getOverflow());

            // A divider left at the end of a tool bar goes with what
            // followed it, and is not counted.
            {
                auto toolBar = ToolBar::create(_context);
                toolBar->addAction(Action::create("A", "Add", [] {}));
                toolBar->addWidget(Divider::create(_context, Orientation::Horizontal));
                toolBar->addAction(Action::create("B", "Add", [] {}));
                toolBar->setParent(window);
                app->tick();
                FTK_CHECK(2 == toolBar->getOverflowMax());
                const int a = toolBar->getOverflowWidth(1);
                toolBar->setOverflow(1);
                FTK_CHECK(1 == toolBar->getOverflow());
                FTK_CHECK(1 == toolBar->getOverflowActions().size());
                app->tick();
                FTK_CHECK(a == toolBar->getSizeHint().w);
                toolBar->setOverflow(0);
                FTK_CHECK(toolBar->getOverflowActions().empty());
                toolBar->setParent(nullptr);
            }

            // A hidden tool bar is not in the row, and leaves room.
            toolBars[0]->setVisible(false);
            app->tick();
            row->setGeometry(Box2I(0, 0, hint.w - 4, hint.h));
            FTK_CHECK(row->getOverflow().empty());
            toolBars[0]->setVisible(true);

            // An action stood for by a widget is listed all the same.
            toolBars[0]->addOverflowAction(Action::create("Other", [] {}));
            FTK_CHECK(5 == toolBars[0]->getActions().size());
            toolBars[0]->clear();
            FTK_CHECK(toolBars[0]->getActions().empty());

            // Taken out of the row, with its divider.
            toolBars[2]->setParent(nullptr);
            app->tick();
            row->setGeometry(Box2I(0, 0, row->getSizeHint().w, hint.h));
            FTK_CHECK(row->getOverflow().empty());
        }
    }
}
