// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/MessageDialogTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/MessageDialog.h>
#include <ftk/UI/DialogSystem.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        MessageDialogTest::MessageDialogTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::MessageDialogTest")
        {}

        MessageDialogTest::~MessageDialogTest()
        {}

        std::shared_ptr<MessageDialogTest> MessageDialogTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<MessageDialogTest>(new MessageDialogTest(context));
        }
                
        void MessageDialogTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("MessageDialogTest");
                auto app = App::create(
                    context,
                    argv,
                    "MessageDialogTest",
                    "Message dialog test.");
                auto window = Window::create(context, app, "MessageDialogTest");
                window->show();
                app->tick();
            }
        }
    }
}

