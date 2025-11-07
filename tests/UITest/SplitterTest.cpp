// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/SplitterTest.h>

#include <ftk/UI/Label.h>
#include <ftk/UI/Splitter.h>

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
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("SplitterTest");
                auto app = App::create(
                    context,
                    argv,
                    "SplitterTest",
                    "Splitter test.");
                auto window = Window::create(context, app, "SplitterTest");
                window->show();
                app->tick();
                _test(context, app, window, Orientation::Horizontal);
                _test(context, app, window, Orientation::Vertical);
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
            FTK_ASSERT(split == splitter->getSplit());
            app->tick();

            auto label0 = Label::create(context, "Label 0", splitter);
            auto label1 = Label::create(context, "Label 1", splitter);
            app->tick();
            splitter->setSplit(.1F);
            app->tick();
            splitter->setSplit(.5F);
            app->tick();

            splitter->setParent(nullptr);
        }
    }
}

