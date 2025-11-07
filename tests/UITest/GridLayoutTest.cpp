// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/GridLayoutTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Divider.h>
#include <ftk/UI/GridLayout.h>
#include <ftk/UI/Spacer.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        GridLayoutTest::GridLayoutTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::GridLayoutTest")
        {}

        GridLayoutTest::~GridLayoutTest()
        {}

        std::shared_ptr<GridLayoutTest> GridLayoutTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<GridLayoutTest>(new GridLayoutTest(context));
        }

        void GridLayoutTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("GridLayoutTest");
                auto app = App::create(
                    context,
                    argv,
                    "GridLayoutTest",
                    "Grid layout test.");
                auto window = Window::create(context, app, "GridLayoutTest");
                window->show();
                app->tick();

                auto layout = GridLayout::create(context, window);
                layout->setSpacingRole(SizeRole::None);
                layout->setSpacingRole(SizeRole::None);
                layout->setSpacingRole(SizeRole::Spacing);
                FTK_ASSERT(SizeRole::Spacing == layout->getSpacingRole());
                layout->setMarginRole(SizeRole::Margin);
                layout->setMarginRole(SizeRole::Margin);
                layout->setMarginRole(SizeRole::None);
                FTK_ASSERT(SizeRole::None == layout->getMarginRole());

                auto spacer0 = Spacer::create(context, Orientation::Horizontal, layout);
                auto spacer1 = Spacer::create(context, Orientation::Horizontal, layout);
                auto spacer2 = Spacer::create(context, Orientation::Horizontal, layout);
                spacer2->setStretch(Stretch::Expanding);
                layout->setGridPos(spacer0, 0, 0);
                layout->setGridPos(spacer1, 0, 1);
                layout->setGridPos(spacer2, 1, 1);
                app->tick();

                spacer2->setParent(nullptr);
                app->tick();
            }
        }
    }
}

