// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/ProgressDialogTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/ProgressDialog.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        ProgressDialogTest::ProgressDialogTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ProgressDialogTest")
        {}

        ProgressDialogTest::~ProgressDialogTest()
        {}

        std::shared_ptr<ProgressDialogTest> ProgressDialogTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ProgressDialogTest>(new ProgressDialogTest(context));
        }
                
        void ProgressDialogTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("ProgressDialogTest");
                auto app = App::create(
                    context,
                    argv,
                    "ProgressDialogTest",
                    "Progress dialog test.");
                auto window = Window::create(context, app, "ProgressDialogTest");
                window->show();
                app->tick();
            }
        }
    }
}

