// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/ListWidgetTest.h>

#include <ftk/UI/ListWidget.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        ListWidgetTest::ListWidgetTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ListWidgetTest")
        {}

        ListWidgetTest::~ListWidgetTest()
        {}

        std::shared_ptr<ListWidgetTest> ListWidgetTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ListWidgetTest>(new ListWidgetTest(context));
        }
                
        void ListWidgetTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("ListWidgetTest");
                auto app = App::create(
                    context,
                    argv,
                    "ListWidgetTest",
                    "List widget test.");
                auto window = Window::create(context, app, "ListWidgetTest");
                window->show();
                app->tick();
                _test(context, app, window, ButtonGroupType::Click);
                _test(context, app, window, ButtonGroupType::Check);
                _test(context, app, window, ButtonGroupType::Radio);
                _test(context, app, window, ButtonGroupType::Toggle);
            }
        }

        void ListWidgetTest::_test(
            const std::shared_ptr<Context>& context,
            const std::shared_ptr<App>& app,
            const std::shared_ptr<Window>& window,
            ButtonGroupType type)
        {
            auto widget = ListWidget::create(context, type, window);
            std::vector<ListItem> items =
            {
                ListItem("Item 0"),
                ListItem("Item 1"),
                ListItem("Item 2"),
                ListItem("Item 3")
            };
            widget->setItems(items);
            widget->setItems(items);
            FTK_ASSERT(items == widget->getItems());
            app->tick();
            items.push_back(ListItem("Item 4"));
            widget->setItems(items);
            app->tick();
            widget->setSearch("4");
            widget->setSearch("4");
            FTK_ASSERT("4" == widget->getSearch());
            app->tick();
            widget->clearSearch();
            FTK_ASSERT(widget->getSearch().empty());
            app->tick();
        }
    }
}

