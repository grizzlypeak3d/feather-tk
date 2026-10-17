// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/SplitterTest.h>

#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/Splitter.h>
#include <ftk/UI/Splitter2D.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

#include <cmath>

namespace ftk
{
    namespace ui_test
    {
        namespace
        {
            //! A widget that says whether it is hovered.
            class HoverProbe : public IMouseWidget
            {
            protected:
                void _init(
                    const std::shared_ptr<Context>& context,
                    const std::shared_ptr<IWidget>& parent)
                {
                    IMouseWidget::_init(context, "HoverProbe", parent);
                    _setMouseHoverEnabled(true);
                    _setMousePressEnabled(true);
                }

            public:
                static std::shared_ptr<HoverProbe> create(
                    const std::shared_ptr<Context>& context,
                    const std::shared_ptr<IWidget>& parent)
                {
                    auto out = std::shared_ptr<HoverProbe>(new HoverProbe);
                    out->_init(context, parent);
                    return out;
                }

                bool isHovered() const { return _isMouseInside(); }
            };
        }

        SplitterTest::SplitterTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::SplitterTest")
        {}

        SplitterTest::~SplitterTest()
        {}

        std::shared_ptr<SplitterTest> SplitterTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<SplitterTest>(new SplitterTest(context));
        }
                
        void SplitterTest::run()
        {
            {
                std::vector<std::string> argv;
                argv.push_back("SplitterTest");
                auto app = App::create(
                    _context,
                    argv,
                    "SplitterTest",
                    "Splitter test.");
                auto window = Window::create(_context, app, "SplitterTest");
                window->show();
                app->tick();
                _test(_context, app, window, Orientation::Horizontal);
                _test(_context, app, window, Orientation::Vertical);
                _test2D(_context, app, window);

                // A finger has no hover: what it lifts from is not left lit,
                // a splitter handle least of all, until the mouse moves.
                auto probe = HoverProbe::create(_context, window);
                window->layout(Size2I(400, 300));
                app->tick();
                const V2I c = center(probe->getGeometry());
                window->click(c);
                app->tick();
                FTK_CHECK(probe->isHovered());
                window->setTouchInput(true);
                window->click(c);
                app->tick();
                FTK_CHECK(!probe->isHovered());
                window->setTouchInput(false);
                window->hover(V2I(c.x + 1, c.y));
                app->tick();
                FTK_CHECK(probe->isHovered());
                probe->setParent(nullptr);
            }
        }

        void SplitterTest::_test(
            const std::shared_ptr<Context>& context,
            const std::shared_ptr<App>& app,
            const std::shared_ptr<Window>& window,
            Orientation orientation)
        {
            auto splitter = Splitter::create(context, orientation, window);
            float split = .9F;
            splitter->setSplit(split);
            splitter->setSplit(split);
            FTK_CHECK(split == splitter->getSplit());
            app->tick();

            auto label0 = Label::create(context, "Label 0", splitter);
            auto label1 = Label::create(context, "Label 1", splitter);
            app->tick();
            splitter->setSplit(.1F);
            app->tick();
            splitter->setSplit(.5F);
            app->tick();

            // A fingertip takes the handle from beside it, where a mouse
            // press lands on the widget next to it; the split follows the
            // finger without jumping under it, and goes back when a second
            // finger takes the press away.
            {
                window->layout(Size2I(400, 300));
                app->tick();
                const Box2I& g = splitter->getGeometry();
                const bool h = Orientation::Horizontal == orientation;
                const int length = h ? g.w() : g.h();
                const V2I c = center(g);
                const V2I beside = h ? V2I(c.x + 15, c.y) : V2I(c.x, c.y + 15);
                const V2I moved = h ? V2I(beside.x + 40, beside.y) : V2I(beside.x, beside.y + 40);
                const V2I far = h ? V2I(c.x + 100, c.y) : V2I(c.x, c.y + 100);
                FTK_CHECK(splitter->isTouchHandle(beside));
                FTK_CHECK(!splitter->isTouchHandle(far));

                window->drag({ beside, moved });
                FTK_CHECK(.5F == splitter->getSplit());

                window->setTouchInput(true);
                window->drag({ beside, moved });
                const float expected = .5F + 40.F / length;
                FTK_CHECK(std::abs(splitter->getSplit() - expected) < 2.F / length);
                const float split = splitter->getSplit();
                app->tick();

                const V2I beside2 = h ? V2I(beside.x + 40, beside.y) : V2I(beside.x, beside.y + 40);
                window->press(beside2);
                window->hover(h ? V2I(beside2.x + 30, beside2.y) : V2I(beside2.x, beside2.y + 30));
                FTK_CHECK(splitter->getSplit() != split);
                window->cancelPress();
                FTK_CHECK(split == splitter->getSplit());
                window->setTouchInput(false);
                splitter->setSplit(.5F);
                app->tick();
            }

            // A split at either end used to put half the handle outside the
            // splitter, where it was drawn over the widget next to it, and
            // gave the collapsing child a negative size on the way.
            const Box2I g(0, 0, 400, 300);
            for (float value : { 0.F, 1.F })
            {
                splitter->setSplit(value);
                splitter->setGeometry(g);
                for (const auto& child : { label0, label1 })
                {
                    const Box2I& cg = child->getGeometry();
                    FTK_CHECK(cg.w() >= 0);
                    FTK_CHECK(cg.h() >= 0);
                    if (cg.isValid())
                    {
                        FTK_CHECK(cg == intersect(cg, g));
                    }
                }
                // One of them collapses, so the split still reaches the end.
                FTK_CHECK(
                    !label0->getGeometry().isValid() ||
                    !label1->getGeometry().isValid());
            }

            splitter->setParent(nullptr);
        }

        void SplitterTest::_test2D(
            const std::shared_ptr<Context>& context,
            const std::shared_ptr<App>& app,
            const std::shared_ptr<Window>& window)
        {
            auto splitter = Splitter2D::create(context, window);
            const V2F split(.9F, .2F);
            splitter->setSplit(split);
            splitter->setSplit(split);
            FTK_CHECK(split == splitter->getSplit());
            app->tick();

            std::vector<std::shared_ptr<Label> > labels;
            for (int i = 0; i < 4; ++i)
            {
                labels.push_back(Label::create(
                    context, Format("Label {0}").arg(i), splitter));
            }
            app->tick();

            // Both divisions at once and each on its own, so that a corner is
            // covered as well as an edge.
            const Box2I g(0, 0, 400, 300);
            for (const auto& value : {
                V2F(0.F, 0.F), V2F(1.F, 1.F),
                V2F(0.F, 1.F), V2F(1.F, 0.F), V2F(0.F, .5F), V2F(.5F, 1.F) })
            {
                splitter->setSplit(value);
                splitter->setGeometry(g);
                size_t collapsed = 0;
                for (const auto& label : labels)
                {
                    const Box2I& lg = label->getGeometry();
                    FTK_CHECK(lg.w() >= 0);
                    FTK_CHECK(lg.h() >= 0);
                    if (lg.isValid())
                    {
                        FTK_CHECK(lg == intersect(lg, g));
                    }
                    else
                    {
                        ++collapsed;
                    }
                }
                // A division at an end collapses the two children on that
                // side; both at once leaves one child holding everything.
                FTK_CHECK(collapsed >= 2);
            }

            splitter->setParent(nullptr);
        }
    }
}

