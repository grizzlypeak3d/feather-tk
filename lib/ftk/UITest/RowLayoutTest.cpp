// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/RowLayoutTest.h>

#include <ftk/UI/Divider.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/Spacer.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        RowLayoutTest::RowLayoutTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::RowLayoutTest")
        {}

        RowLayoutTest::~RowLayoutTest()
        {}

        std::shared_ptr<RowLayoutTest> RowLayoutTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<RowLayoutTest>(new RowLayoutTest(context));
        }

        void RowLayoutTest::run()
        {
            {
                std::vector<std::string> argv;
                argv.push_back("RowLayoutTest");
                auto app = App::create(
                    _context,
                    argv,
                    "RowLayoutTest",
                    "Row layout test.");
                auto window = Window::create(_context, app, "RowLayoutTest");
                window->show();
                app->tick();

                std::shared_ptr<RowLayout> layout = VerticalLayout::create(_context, window);
                layout->setSpacingRole(SizeRole::None);
                layout->setSpacingRole(SizeRole::None);
                layout->setSpacingRole(SizeRole::Spacing);
                FTK_CHECK(SizeRole::Spacing == layout->getSpacingRole());
                layout->setMarginRole(SizeRole::Margin);
                layout->setMarginRole(SizeRole::Margin);
                layout->setMarginRole(SizeRole::None);
                FTK_CHECK(SizeRole::None == layout->getMarginRole());
                _test(_context, app, window, layout, Orientation::Horizontal);
                layout->setParent(nullptr);
                layout.reset();

                layout = HorizontalLayout::create(_context, window);
                _test(_context, app, window, layout, Orientation::Vertical);
                layout->setParent(nullptr);
                layout.reset();
            }
            _dividers();
        }

        void RowLayoutTest::_dividers()
        {
            // Groups with a divider between each: a divider shows only with
            // a visible group on each side, and one between any two.
            auto layout = HorizontalLayout::create(_context);
            std::vector<std::shared_ptr<IWidget> > groups;
            std::vector<std::shared_ptr<Divider> > dividers;
            for (int i = 0; i < 4; ++i)
            {
                if (i > 0)
                {
                    dividers.push_back(Divider::create(_context, Orientation::Horizontal, layout));
                }
                groups.push_back(Label::create(_context, "Group", layout));
            }
            const auto visible = [&dividers]
            {
                std::vector<bool> out;
                for (const auto& divider : dividers)
                {
                    out.push_back(divider->isVisible(false));
                }
                return out;
            };

            updateDividers(layout);
            FTK_CHECK(std::vector<bool>({ true, true, true }) == visible());

            // The first group hidden: nothing to divide it from.
            groups[0]->hide();
            updateDividers(layout);
            FTK_CHECK(std::vector<bool>({ false, true, true }) == visible());

            // A middle group hidden: one divider between its neighbors.
            groups[0]->show();
            groups[2]->hide();
            updateDividers(layout);
            FTK_CHECK(std::vector<bool>({ true, true, false }) == visible());

            // The last group hidden.
            groups[2]->show();
            groups[3]->hide();
            updateDividers(layout);
            FTK_CHECK(std::vector<bool>({ true, true, false }) == visible());

            // One group left: no dividers.
            groups[0]->hide();
            groups[1]->hide();
            updateDividers(layout);
            FTK_CHECK(std::vector<bool>({ false, false, false }) == visible());
        }

        void RowLayoutTest::_test(
            const std::shared_ptr<Context>& context,
            const std::shared_ptr<App>& app,
            const std::shared_ptr<Window>& window,
            const std::shared_ptr<RowLayout>& layout,
            Orientation orientation)
        {
            auto label0 = Label::create(context, "Label 0", layout);
            auto spacer = Spacer::create(context, orientation, layout);
            spacer->setSpacingRole(SizeRole::SpacingLarge);
            spacer->setSpacingRole(SizeRole::SpacingLarge);
            FTK_CHECK(SizeRole::SpacingLarge == spacer->getSpacingRole());
            auto label1 = Label::create(context, "Label 1", layout);
            app->tick();

            switch (orientation)
            {
            case Orientation::Horizontal:
                label0->setHStretch(Stretch::Expanding);
                label0->setHStretch(Stretch::Expanding);
                label0->setStretch(Stretch::Fixed, Stretch::Fixed);
                label0->setStretch(Stretch::Fixed, Stretch::Fixed);
                label0->setStretch(Stretch::Expanding);
                FTK_CHECK(Stretch::Expanding == label0->getHStretch());
                app->tick();
                label0->setHAlign(HAlign::Right);
                label0->setHAlign(HAlign::Right);
                label0->setAlign(HAlign::Center, VAlign::Center);
                label0->setAlign(HAlign::Center, VAlign::Center);
                FTK_CHECK(HAlign::Center == label0->getHAlign());
                app->tick();
                label0->setHAlign(HAlign::Left);
                label1->setHAlign(HAlign::Right);
                break;
            case Orientation::Vertical:
                label0->setVStretch(Stretch::Expanding);
                label0->setVStretch(Stretch::Expanding);
                label0->setStretch(Stretch::Fixed, Stretch::Fixed);
                label0->setStretch(Stretch::Fixed, Stretch::Fixed);
                label0->setStretch(Stretch::Expanding);
                FTK_CHECK(Stretch::Expanding == label0->getVStretch());
                app->tick();
                label0->setVAlign(VAlign::Bottom);
                label0->setVAlign(VAlign::Bottom);
                label0->setAlign(HAlign::Center, VAlign::Center);
                label0->setAlign(HAlign::Center, VAlign::Center);
                FTK_CHECK(VAlign::Center == label0->getVAlign());
                app->tick();
                label0->setVAlign(VAlign::Top);
                label1->setVAlign(VAlign::Bottom);
                break;
            default: break;
            }
            app->tick();

            label0->hide();
            label0->hide();
            app->tick();
            FTK_CHECK(!label0->isVisible());
            FTK_CHECK(!label0->isVisible(false));
            FTK_CHECK(label0->isClipped());
            label0->show();
            app->tick();

            label0->setParent(nullptr);
            app->tick();
            auto children = layout->getChildren();
            FTK_CHECK(2 == children.size());
            FTK_CHECK(spacer == children.front());
            label0->setParent(layout);
            app->tick();
            children = layout->getChildren();
            FTK_CHECK(3 == children.size());
            FTK_CHECK(spacer == children.front());
            FTK_CHECK(label0 == children.back());
            label0->setParent(nullptr);
            app->tick();

            label1->setParent(nullptr);
            spacer->setParent(nullptr);
            app->tick();
            children = layout->getChildren();
            FTK_CHECK(children.empty());
        }
    }
}

