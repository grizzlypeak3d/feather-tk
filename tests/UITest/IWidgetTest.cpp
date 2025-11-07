// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/IWidgetTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        IWidgetTest::IWidgetTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::IWidgetTest")
        {}

        IWidgetTest::~IWidgetTest()
        {}

        std::shared_ptr<IWidgetTest> IWidgetTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<IWidgetTest>(new IWidgetTest(context));
        }

        namespace
        {
            class Widget : public IWidget
            {
            protected:
                Widget()
                {}

            public:
                static std::shared_ptr<Widget> create(
                    const std::shared_ptr<Context>& context,
                    const std::shared_ptr<IWidget>& parent)
                {
                    auto out = std::shared_ptr<Widget>(new Widget);
                    out->_init(context, "Widget", parent);
                    return out;
                }
            };
        }

        void IWidgetTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("IWidgetTest");
                auto app = App::create(
                    context,
                    argv,
                    "IWidgetTest",
                    "IWidget test.");
                auto window = Window::create(context, app, "IWidgetTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto widget0 = Widget::create(context, layout);
                FTK_ASSERT(0 == layout->getChildIndex(widget0));
                auto widget1 = Widget::create(context, layout);
                FTK_ASSERT(1 == layout->getChildIndex(widget1));
                auto widget2 = Widget::create(context, layout);
                FTK_ASSERT(2 == layout->getChildIndex(widget2));
                app->tick();

                layout->moveToBack(widget2);
                app->tick();
                FTK_ASSERT(0 == layout->getChildIndex(widget2));
                FTK_ASSERT(1 == layout->getChildIndex(widget0));
                FTK_ASSERT(2 == layout->getChildIndex(widget1));

                layout->moveToFront(widget2);
                app->tick();
                FTK_ASSERT(0 == layout->getChildIndex(widget0));
                FTK_ASSERT(1 == layout->getChildIndex(widget1));
                FTK_ASSERT(2 == layout->getChildIndex(widget2));

                layout->moveToIndex(widget2, 2);
                app->tick();
                FTK_ASSERT(0 == layout->getChildIndex(widget0));
                FTK_ASSERT(1 == layout->getChildIndex(widget1));
                FTK_ASSERT(2 == layout->getChildIndex(widget2));

                layout->moveToIndex(widget2, 3);
                app->tick();
                FTK_ASSERT(0 == layout->getChildIndex(widget0));
                FTK_ASSERT(1 == layout->getChildIndex(widget1));
                FTK_ASSERT(2 == layout->getChildIndex(widget2));

                layout->moveToIndex(widget2, 1);
                app->tick();
                FTK_ASSERT(0 == layout->getChildIndex(widget0));
                FTK_ASSERT(1 == layout->getChildIndex(widget2));
                FTK_ASSERT(2 == layout->getChildIndex(widget1));

                layout->moveToIndex(widget0, 1);
                app->tick();
                FTK_ASSERT(0 == layout->getChildIndex(widget2));
                FTK_ASSERT(1 == layout->getChildIndex(widget0));
                FTK_ASSERT(2 == layout->getChildIndex(widget1));
            }
        }
    }
}

