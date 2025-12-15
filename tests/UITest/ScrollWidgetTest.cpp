// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/ScrollWidgetTest.h>

#include <ftk/UI/GridLayout.h>
#include <ftk/UI/Label.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        ScrollWidgetTest::ScrollWidgetTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ScrollWidgetTest")
        {}

        ScrollWidgetTest::~ScrollWidgetTest()
        {}

        std::shared_ptr<ScrollWidgetTest> ScrollWidgetTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ScrollWidgetTest>(new ScrollWidgetTest(context));
        }
                
        void ScrollWidgetTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("ScrollWidgetTest");
                auto app = App::create(
                    context,
                    argv,
                    "ScrollWidgetTest",
                    "Scroll widget test.");
                auto window = Window::create(context, app, "ScrollWidgetTest");
                window->show();
                app->tick();
                _test(context, app, window, ScrollType::Both);
                _test(context, app, window, ScrollType::Horizontal);
                _test(context, app, window, ScrollType::Vertical);
            }
        }

        void ScrollWidgetTest::_test(
            const std::shared_ptr<Context>& context,
            const std::shared_ptr<App>& app,
            const std::shared_ptr<Window>& window,
            ScrollType scrollType)
        {
            auto scrollWidget = ScrollWidget::create(context, scrollType, window);
            auto layout = GridLayout::create(context);
            layout->setMarginRole(SizeRole::MarginLarge);
            for (int row = 0; row < 20; ++row)
            {
                for (int column = 0; column < 20; ++column)
                {
                    auto label = Label::create(
                        context,
                        Format("Label {0} {1}").arg(row).arg(column),
                        layout);
                    label->setFontRole(FontRole::Title);
                    label->setMarginRole(SizeRole::MarginLarge);
                    layout->setGridPos(label, row, column);
                }
            }
            scrollWidget->setWidget(layout);
            scrollWidget->setWidget(layout);
            FTK_ASSERT(layout == scrollWidget->getWidget());
            app->tick();

            _print(Format("Scroll type: {0}").arg(scrollType));
            _print(Format("Scroll size: {0}").arg(scrollWidget->getScrollSize()));
            _print(Format("Scroll pos: {0}").arg(scrollWidget->getScrollPos()));

            V2I scroll(100, 100);
            scrollWidget->setScrollPos(scroll);
            FTK_ASSERT(scrollWidget->getScrollPos() == scroll);
            scrollWidget->setScrollPosCallback(
                [&scroll](const V2I& value)
                {
                    scroll = value;
                });
            scrollWidget->setScrollBarsVisible(false);
            scrollWidget->setScrollBarsVisible(false);
            FTK_ASSERT(!scrollWidget->areScrollBarsVisible());
            scrollWidget->setScrollBarsVisible(true);
            scrollWidget->setScrollEventsEnabled(false);
            scrollWidget->setScrollEventsEnabled(false);
            FTK_ASSERT(!scrollWidget->areScrollEventsEnabled());
            scrollWidget->setScrollEventsEnabled(true);
            scrollWidget->setBorder(false);
            scrollWidget->setBorder(false);
            FTK_ASSERT(!scrollWidget->hasBorder());
            scrollWidget->setBorder(true);

            Size2I size = scrollWidget->getScrollSize();
            scrollWidget->setScrollPos(V2I(size.w, size.h));
            app->tick();
            window->setSize(Size2I(size.w * 2, size.h * 2));
            app->tick();
            window->setSize(Size2I(1280, 960));

            scrollWidget->setParent(nullptr);
        }
    }
}

