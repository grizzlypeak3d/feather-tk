// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/ToolBarRowTest.h>

#include <ftk/UI/Action.h>
#include <ftk/UI/App.h>
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

            // Three tool bars of two actions each.
            std::vector<std::shared_ptr<ToolBar> > toolBars;
            for (size_t i = 0; i < 3; ++i)
            {
                auto toolBar = ToolBar::create(_context);
                for (int j = 0; j < 2; ++j)
                {
                    toolBar->addAction(Action::create("Action", "Add", [] {}));
                }
                row->addToolBar(toolBar);
                toolBars.push_back(toolBar);
            }
            FTK_CHECK(2 == toolBars[0]->getActions().size());
            FTK_CHECK(!row->isKeepLast());
            app->tick();

            // Room for all of them: nothing overflows, left to right.
            const Size2I hint = row->getSizeHint();
            row->setGeometry(Box2I(0, 0, hint.w, hint.h));
            FTK_CHECK(row->getOverflow().empty());
            FTK_CHECK(toolBars[0]->getGeometry().min.x < toolBars[1]->getGeometry().min.x);
            FTK_CHECK(toolBars[1]->getGeometry().min.x < toolBars[2]->getGeometry().min.x);
            FTK_CHECK(toolBars[2]->getGeometry().max.x < hint.w);

            // A little short: the last goes, whole, past the end.
            row->setGeometry(Box2I(0, 0, hint.w - 4, hint.h));
            auto overflow = row->getOverflow();
            FTK_CHECK(1 == overflow.size());
            FTK_CHECK(!overflow.empty() && toolBars[2] == overflow[0]);
            FTK_CHECK(toolBars[2]->getGeometry().min.x > row->getGeometry().max.x);

            // Room for one: the first stays.
            const int one = toolBars[0]->getSizeHint().w * 2;
            row->setGeometry(Box2I(0, 0, one, hint.h));
            overflow = row->getOverflow();
            FTK_CHECK(2 == overflow.size());
            FTK_CHECK(toolBars[0]->getGeometry().min.x == 0);

            // With the last kept, the one before it goes first, and then
            // the first; the last stays at the start of the row.
            row->setKeepLast(true);
            FTK_CHECK(row->isKeepLast());
            row->setGeometry(Box2I(0, 0, hint.w - 4, hint.h));
            overflow = row->getOverflow();
            FTK_CHECK(1 == overflow.size());
            FTK_CHECK(!overflow.empty() && toolBars[1] == overflow[0]);
            row->setGeometry(Box2I(0, 0, one, hint.h));
            overflow = row->getOverflow();
            FTK_CHECK(2 == overflow.size());
            FTK_CHECK(toolBars[2]->getGeometry().min.x == 0);
            row->setKeepLast(false);

            // A hidden tool bar is not in the row, and leaves room.
            toolBars[0]->setVisible(false);
            app->tick();
            row->setGeometry(Box2I(0, 0, hint.w - 4, hint.h));
            FTK_CHECK(row->getOverflow().empty());
            toolBars[0]->setVisible(true);

            // An action stood for by a widget is listed all the same.
            toolBars[0]->addOverflowAction(Action::create("Other", [] {}));
            FTK_CHECK(3 == toolBars[0]->getActions().size());
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
