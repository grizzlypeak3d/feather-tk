// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/ConfirmDialogTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/DialogSystem.h>
#include <ftk/UI/ConfirmDialog.h>
#include <ftk/UI/PushButton.h>
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
            {
                std::vector<std::string> argv;
                argv.push_back("ConfirmDialogTest");
                auto app = App::create(
                    _context,
                    argv,
                    "ConfirmDialogTest",
                    "Confirmation dialog test.");
                auto window = Window::create(_context, app, "ConfirmDialogTest");
                window->show();
                app->tick();

                auto system = _context->getSystem<DialogSystem>();
                bool confirmed = false;
                system->confirm(
                    "Confirm",
                    "This is a confirmation?",
                    window,
                    [&confirmed](bool value) { confirmed = value; });

                // Whichever button has the focus is the one Return presses,
                // so it has to be the cancel; the focus order alone would
                // give it to the confirm, which is how a stray Return could
                // overwrite a file. Checked before ticking, because that is
                // when opening decides, and a window that is never really
                // laid out drops the focus again on the tick.
                auto keyFocus = std::dynamic_pointer_cast<PushButton>(
                    window->getKeyFocus());
                FTK_CHECK(keyFocus);
                FTK_CHECK(keyFocus && "Cancel" == keyFocus->getText());

                app->tick();
            }
        }
    }
}

