// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/FlowLayoutTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/FlowLayout.h>
#include <ftk/UI/Spacer.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>

namespace ftk
{
    namespace ui_test
    {
        FlowLayoutTest::FlowLayoutTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::FlowLayoutTest")
        {}

        FlowLayoutTest::~FlowLayoutTest()
        {}

        std::shared_ptr<FlowLayoutTest> FlowLayoutTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<FlowLayoutTest>(new FlowLayoutTest(context));
        }

        void FlowLayoutTest::run()
        {
            std::vector<std::string> argv;
            argv.push_back("FlowLayoutTest");
            auto app = App::create(
                _context,
                argv,
                "FlowLayoutTest",
                "Flow layout test.");
            auto window = Window::create(_context, app, "FlowLayoutTest");
            window->show();
            app->tick();

            auto layout = FlowLayout::create(_context, window);
            layout->setMarginRole(SizeRole::Margin);
            FTK_CHECK(SizeRole::Margin == layout->getMarginRole());
            layout->setMarginRole(SizeRole::None);
            layout->setSpacingRole(SizeRole::SpacingSmall);
            FTK_CHECK(SizeRole::SpacingSmall == layout->getSpacingRole());

            auto a = Spacer::create(_context, Orientation::Horizontal, layout);
            auto b = Spacer::create(_context, Orientation::Horizontal, layout);
            auto c = Spacer::create(_context, Orientation::Horizontal, layout);
            app->tick();

            // Measured from the children rather than assumed, so the test does
            // not depend on what a spacer happens to be worth at this scale.
            const int w = a->getSizeHint().w;
            FTK_CHECK(w > 0);

            // Wide enough for all three: one line, left to right.
            layout->setGeometry(Box2I(0, 0, w * 8, 1000));
            FTK_CHECK(a->getGeometry().min.y == b->getGeometry().min.y);
            FTK_CHECK(b->getGeometry().min.y == c->getGeometry().min.y);
            FTK_CHECK(a->getGeometry().min.x < b->getGeometry().min.x);
            FTK_CHECK(b->getGeometry().min.x < c->getGeometry().min.x);

            // Room for one at a time: three lines, each starting at the left.
            layout->setGeometry(Box2I(0, 0, w, 1000));
            FTK_CHECK(a->getGeometry().min.y < b->getGeometry().min.y);
            FTK_CHECK(b->getGeometry().min.y < c->getGeometry().min.y);
            FTK_CHECK(a->getGeometry().min.x == b->getGeometry().min.x);
            FTK_CHECK(b->getGeometry().min.x == c->getGeometry().min.x);

            // Narrower than a single child still places it, since there is
            // nowhere narrower to put it.
            layout->setGeometry(Box2I(0, 0, w / 4, 1000));
            FTK_CHECK(a->getGeometry().w() == w);

            // The height hint follows the width it was last given.
            const Size2I tall = layout->getSizeHint();
            layout->setGeometry(Box2I(0, 0, w * 8, 1000));
            FTK_CHECK(layout->getSizeHint().h < tall.h);
        }
    }
}
