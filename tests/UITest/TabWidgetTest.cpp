// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/TabWidgetTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/TabBar.h>
#include <ftk/UI/TabWidget.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        TabWidgetTest::TabWidgetTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::TabWidgetTest")
        {}

        TabWidgetTest::~TabWidgetTest()
        {}

        std::shared_ptr<TabWidgetTest> TabWidgetTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<TabWidgetTest>(new TabWidgetTest(context));
        }
                
        void TabWidgetTest::run()
        {
            _bar();
            _widget();
        }

        void TabWidgetTest::_bar()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("TabWidgetTest");
                auto app = App::create(
                    context,
                    argv,
                    "TabWidgetTest",
                    "Tab widget test.");
                auto window = Window::create(context, app, "TabWidgetTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto widget = TabBar::create(context, layout);
                std::vector<std::string> tabs =
                {
                    "Tab 0",
                    "Tab 1",
                };
                widget->setTabs(tabs);
                widget->setTabs(tabs);
                FTK_ASSERT(tabs == widget->getTabs());
                app->tick();
                widget->addTab("Tab 2");
                tabs.push_back("Tab 2");
                FTK_ASSERT(tabs == widget->getTabs());
                app->tick();
                widget->clear();
                FTK_ASSERT(widget->getTabs().empty());
                app->tick();
                widget->setTabs(tabs);
                int tab = 0;
                widget->setCallback(
                    [&tab](int value)
                    {
                        tab = value;
                    });
                widget->setCurrent(2);
                widget->setCurrent(2);
                FTK_ASSERT(2 == widget->getCurrent());
                app->tick();
            }
        }

        void TabWidgetTest::_widget()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("TabWidgetTest");
                auto app = App::create(
                    context,
                    argv,
                    "TabWidgetTest",
                    "Tab widget test.");
                auto window = Window::create(context, app, "TabWidgetTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto widget = TabWidget::create(context, layout);
                std::vector<std::string> tabs =
                {
                    "Tab 0",
                    "Tab 1",
                };
                auto label0 = Label::create(context, "Label 0");
                widget->addTab(tabs[0], label0);
                auto label1 = Label::create(context, "Label 1");
                widget->addTab(tabs[1], label1);
                FTK_ASSERT(tabs == widget->getTabs());
                app->tick();
                widget->clear();
                FTK_ASSERT(widget->getTabs().empty());
                app->tick();
                widget->addTab(tabs[0], label0);
                widget->addTab(tabs[1], label1);
                int tab = 0;
                widget->setCallback(
                    [&tab](int value)
                    {
                        tab = value;
                    });
                widget->setCurrent(1);
                widget->setCurrent(1);
                FTK_ASSERT(1 == widget->getCurrent());
                app->tick();
            }
        }
    }
}

