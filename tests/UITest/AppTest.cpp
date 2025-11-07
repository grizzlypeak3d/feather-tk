// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/AppTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Window.h>

#include <ftk/UI/PushButton.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        AppTest::AppTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::AppTest")
        {}

        AppTest::~AppTest()
        {}

        std::shared_ptr<AppTest> AppTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<AppTest>(new AppTest(context));
        }
                
        void AppTest::run()
        {
            int r = 0;
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv = { "app", "-exit" };
                try
                {
                    auto app = App::create(
                        context,
                        argv,
                        "ftk::ui_test::App",
                        "Test UI application");
                    r = app->getExit();
                    if (0 == r)
                    {
                        auto window = Window::create(context, app, "ftk::ui_test::App");
                        auto layout = VerticalLayout::create(context, window);
                        auto button = PushButton::create(context, "Button", layout);
                        FTK_ASSERT(app->getWindows().front() == window);
                        window->show();
                        window->setSize(Size2I(1920, 1080));

                        _print(Format("Window size: {0}").arg(window->getSize()));
                        _print(Format("Frame buffer size: {0}").
                            arg(window->getFrameBufferSize()));
                        _print(Format("Display scale: {0}").
                            arg(window->getDisplayScale()));

                        app->run();
                        app->exit();
                        window->close();
                    }
                }
                catch (const std::exception&)
                {}
            }
            FTK_ASSERT(0 == r);
        }
    }
}
