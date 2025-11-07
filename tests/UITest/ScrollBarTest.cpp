// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/ScrollBarTest.h>

#include <ftk/UI/ScrollBar.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        ScrollBarTest::ScrollBarTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ScrollBarTest")
        {}

        ScrollBarTest::~ScrollBarTest()
        {}

        std::shared_ptr<ScrollBarTest> ScrollBarTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ScrollBarTest>(new ScrollBarTest(context));
        }
                
        void ScrollBarTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("ScrollBarTest");
                auto app = App::create(
                    context,
                    argv,
                    "ScrollBarTest",
                    "Scroll bar test.");
                auto window = Window::create(context, app, "ScrollBarTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();
                _test(context, app, window, layout, Orientation::Horizontal);
                _test(context, app, window, layout, Orientation::Vertical);
            }
        }

        void ScrollBarTest::_test(
            const std::shared_ptr<Context>& context,
            const std::shared_ptr<App>& app,
            const std::shared_ptr<Window>& window,
            const std::shared_ptr<VerticalLayout>& layout,
            Orientation orientation)
        {
            auto widget = ScrollBar::create(context, orientation, layout);
            widget->setScrollSize(10000);
            widget->setScrollSize(10000);
            FTK_ASSERT(10000 == widget->getScrollSize());
            widget->setScrollPos(1000);
            widget->setScrollPos(1000);
            FTK_ASSERT(1000 == widget->getScrollPos());

            widget->setParent(nullptr);
        }
    }
}

