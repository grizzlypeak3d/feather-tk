// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/StackLayoutTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Divider.h>
#include <ftk/UI/StackLayout.h>
#include <ftk/UI/Spacer.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        StackLayoutTest::StackLayoutTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::StackLayoutTest")
        {}

        StackLayoutTest::~StackLayoutTest()
        {}

        std::shared_ptr<StackLayoutTest> StackLayoutTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<StackLayoutTest>(new StackLayoutTest(context));
        }

        void StackLayoutTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("StackLayoutTest");
                auto app = App::create(
                    context,
                    argv,
                    "StackLayoutTest",
                    "Stack layout test.");
                auto window = Window::create(context, app, "StackLayoutTest");
                window->show();
                app->tick();

                auto layout = StackLayout::create(context, window);
                layout->setMarginRole(SizeRole::Margin);
                layout->setMarginRole(SizeRole::Margin);
                layout->setMarginRole(SizeRole::None);
                FTK_ASSERT(SizeRole::None == layout->getMarginRole());

                auto spacer0 = Spacer::create(context, Orientation::Horizontal, layout);
                auto spacer1 = Spacer::create(context, Orientation::Horizontal, layout);
                auto spacer2 = Spacer::create(context, Orientation::Horizontal, layout);
                app->tick();
                FTK_ASSERT(0 == layout->getCurrentIndex());
                FTK_ASSERT(spacer0->isVisible());
                FTK_ASSERT(!spacer1->isVisible());
                FTK_ASSERT(!spacer2->isVisible());

                layout->setCurrentIndex(1);
                app->tick();
                FTK_ASSERT(!spacer0->isVisible());
                FTK_ASSERT(spacer1->isVisible());
                FTK_ASSERT(!spacer2->isVisible());

                layout->setCurrentWidget(spacer2);
                app->tick();
                FTK_ASSERT(spacer2 == layout->getCurrentWidget());
                FTK_ASSERT(!spacer0->isVisible());
                FTK_ASSERT(!spacer1->isVisible());
                FTK_ASSERT(spacer2->isVisible());

                spacer2->setParent(nullptr);
                app->tick();
                FTK_ASSERT(1 == layout->getCurrentIndex());
                FTK_ASSERT(!spacer0->isVisible());
                FTK_ASSERT(spacer1->isVisible());
            }
        }
    }
}

