// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/SplitterTest.h>

#include <ftk/UI/Label.h>
#include <ftk/UI/Splitter.h>
#include <ftk/UI/Splitter2D.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
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

