// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/GridLayoutTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Divider.h>
#include <ftk/UI/FormLayout.h>
#include <ftk/UI/GridLayout.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
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
            {
                std::vector<std::string> argv;
                argv.push_back("GridLayoutTest");
                auto app = App::create(
                    _context,
                    argv,
                    "GridLayoutTest",
                    "Grid layout test.");
                auto window = Window::create(_context, app, "GridLayoutTest");
                window->show();
                app->tick();

                auto layout = GridLayout::create(_context, window);
                layout->setSpacingRole(SizeRole::None);
                layout->setSpacingRole(SizeRole::None);
                layout->setSpacingRole(SizeRole::Spacing);
                FTK_CHECK(SizeRole::Spacing == layout->getSpacingRole());
                layout->setMarginRole(SizeRole::Margin);
                layout->setMarginRole(SizeRole::Margin);
                layout->setMarginRole(SizeRole::None);
                FTK_CHECK(SizeRole::None == layout->getMarginRole());

                auto spacer0 = Spacer::create(_context, Orientation::Horizontal, layout);
                auto spacer1 = Spacer::create(_context, Orientation::Horizontal, layout);
                auto spacer2 = Spacer::create(_context, Orientation::Horizontal, layout);
                spacer2->setStretch(Stretch::Expanding);
                layout->setGridPos(spacer0, 0, 0);
                layout->setGridPos(spacer1, 0, 1);
                layout->setGridPos(spacer2, 1, 1);
                app->tick();

                spacer2->setParent(nullptr);
                app->tick();

                // A row with a background of its own.
                FTK_CHECK(ColorRole::None == layout->getRowBackgroundRole(0));
                layout->setRowBackgroundRole(0, ColorRole::Button);
                layout->setRowBackgroundRole(0, ColorRole::Button);
                FTK_CHECK(ColorRole::Button == layout->getRowBackgroundRole(0));
                app->tick();
                layout->setRowBackgroundRole(0, ColorRole::None);
                FTK_CHECK(ColorRole::None == layout->getRowBackgroundRole(0));

                // A column is no narrower than its minimum width.
                const int w = layout->getSizeHint().w;
                FTK_CHECK(0 == layout->getColumnMinWidth(0));
                layout->setColumnMinWidth(0, w + 100);
                layout->setColumnMinWidth(0, w + 100);
                FTK_CHECK(w + 100 == layout->getColumnMinWidth(0));
                FTK_CHECK(layout->getSizeHint().w >= w + 100);
                layout->setColumnMinWidth(0, 0);
                FTK_CHECK(w == layout->getSizeHint().w);
            }
            {
                // Forms in a group share the width of their labels, from
                // the first time they are laid out.
                std::vector<std::string> argv;
                argv.push_back("GridLayoutTest");
                auto app = App::create(
                    _context,
                    argv,
                    "GridLayoutTest",
                    "Grid layout test.");
                auto window = Window::create(_context, app, "GridLayoutTest");
                window->show();
                app->tick();

                auto layout = VerticalLayout::create(_context, window);
                auto form0 = FormLayout::create(_context, layout);
                auto form1 = FormLayout::create(_context, layout);
                auto widget0 = Label::create(_context, "Value", nullptr);
                auto widget1 = Label::create(_context, "Value", nullptr);
                auto widget2 = Label::create(_context, "Value", nullptr);
                form0->addRow("A:", widget0);
                form1->addRow("A considerably longer label:", widget1);
                form1->addRow("B:", widget2);

                auto group = FormGroup::create();
                form0->setGroup(group);
                form0->setGroup(group);
                form1->setGroup(group);
                FTK_CHECK(group == form0->getGroup());
                app->tick();
                const int shared = widget0->getGeometry().min.x;
                FTK_CHECK(shared == widget1->getGeometry().min.x);

                // A hidden row does not count.
                form1->setRowVisible(0, false);
                app->tick();
                FTK_CHECK(widget0->getGeometry().min.x < shared);
                FTK_CHECK(widget0->getGeometry().min.x == widget2->getGeometry().min.x);
                form1->setRowVisible(0, true);
                app->tick();
                FTK_CHECK(shared == widget0->getGeometry().min.x);

                // Out of the group, a form is as wide as its own labels.
                form0->setGroup(nullptr);
                FTK_CHECK(!form0->getGroup());
                app->tick();
                FTK_CHECK(widget0->getGeometry().min.x < shared);
                FTK_CHECK(shared == widget1->getGeometry().min.x);

                // Every form under a widget, at once.
                form0->setGroup(nullptr);
                form1->setGroup(nullptr);
                setFormGroup(layout, group);
                FTK_CHECK(group == form0->getGroup());
                FTK_CHECK(group == form1->getGroup());
                app->tick();
                FTK_CHECK(shared == widget0->getGeometry().min.x);
                FTK_CHECK(shared == widget1->getGeometry().min.x);

                // A form that is gone is forgotten.
                form0->setGroup(group);
                form1->setParent(nullptr);
                form1.reset();
                app->tick();
                FTK_CHECK(widget0->getGeometry().min.x < shared);
            }
        }
    }
}

