// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/ItemButtonListTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/ItemButton.h>
#include <ftk/UI/ItemButtonList.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>

namespace ftk
{
    namespace ui_test
    {
        ItemButtonListTest::ItemButtonListTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ItemButtonListTest")
        {}

        ItemButtonListTest::~ItemButtonListTest()
        {}

        std::shared_ptr<ItemButtonListTest> ItemButtonListTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ItemButtonListTest>(new ItemButtonListTest(context));
        }

        void ItemButtonListTest::run()
        {
            _doubleClick();
        }

        void ItemButtonListTest::_doubleClick()
        {
            std::vector<std::string> argv;
            argv.push_back("ItemButtonListTest");
            auto app = App::create(
                _context,
                argv,
                "ItemButtonListTest",
                "Item button list test.");
            auto window = Window::create(_context, app, "ItemButtonListTest");
            auto layout = VerticalLayout::create(_context, window);
            auto list = ItemButtonList::create(_context, layout);
            for (int i = 0; i < 3; ++i)
            {
                auto item = ItemButton::create(_context, list);
                item->setAcceptsKeyFocus(false);
                item->setWidget(Label::create(_context, "Row"));
            }
            int current = -1;
            int doubleClick = -1;
            int doubleClicks = 0;
            list->setCurrentCallback(
                [&current](int value)
                {
                    current = value;
                });
            list->setDoubleClickCallback(
                [&doubleClick, &doubleClicks](int value)
                {
                    doubleClick = value;
                    ++doubleClicks;
                });
            window->show();
            window->layout(Size2I(1280, 960));
            app->tick();

            const auto rows = list->getItems();
            auto center = [](const std::shared_ptr<IWidget>& widget)
                {
                    const Box2I& g = widget->getGeometry();
                    return V2I(g.min.x + g.w() / 2, g.min.y + g.h() / 2);
                };

            // One click is not a double click.
            window->click(center(rows[0]));
            app->tick();
            FTK_CHECK(0 == current);
            FTK_CHECK(0 == doubleClicks);

            // A second click on the same row is. The current item does not
            // move, so it is not reported again.
            current = -1;
            window->click(center(rows[0]));
            app->tick();
            FTK_CHECK(-1 == current);
            FTK_CHECK(1 == doubleClicks);
            FTK_CHECK(0 == doubleClick);

            // A third click is a new first click, not a second double one.
            window->click(center(rows[0]));
            app->tick();
            FTK_CHECK(1 == doubleClicks);

            // Two clicks on different rows are two single clicks.
            window->click(center(rows[1]));
            app->tick();
            window->click(center(rows[2]));
            app->tick();
            FTK_CHECK(2 == current);
            FTK_CHECK(1 == doubleClicks);

            // A double click is free to rebuild the rows: it is counted
            // from the presses, and the release of a row that has been
            // destroyed never arrives.
            list->setDoubleClickCallback(
                [this, &doubleClicks, &list](int value)
                {
                    ++doubleClicks;
                    list->clear();
                    for (int i = 0; i < 3; ++i)
                    {
                        auto item = ItemButton::create(_context, list);
                        item->setAcceptsKeyFocus(false);
                        item->setWidget(Label::create(_context, "Row"));
                    }
                });
            window->click(center(rows[2]));
            app->tick();
            FTK_CHECK(2 == doubleClicks);
        }
    }
}
