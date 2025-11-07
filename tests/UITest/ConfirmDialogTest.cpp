// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/ConfirmDialogTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/DialogSystem.h>
#include <ftk/UI/ConfirmDialog.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        ConfirmDialogTest::ConfirmDialogTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ConfirmDialogTest")
        {}

        ConfirmDialogTest::~ConfirmDialogTest()
        {}

        std::shared_ptr<ConfirmDialogTest> ConfirmDialogTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ConfirmDialogTest>(new ConfirmDialogTest(context));
        }
                
        void ConfirmDialogTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("ConfirmDialogTest");
                auto app = App::create(
                    context,
                    argv,
                    "ConfirmDialogTest",
                    "Confirmation dialog test.");
                auto window = Window::create(context, app, "ConfirmDialogTest");
                window->show();
                app->tick();

                auto system = context->getSystem<DialogSystem>();
                bool confirmed = false;
                system->confirm(
                    "Confirm",
                    "This is a confirmation?",
                    window,
                    [&confirmed](bool value) { confirmed = value; });
                app->tick();
            }
        }
    }
}

