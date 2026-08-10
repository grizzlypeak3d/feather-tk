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
        namespace
        {
            //! Window that can be laid out without the window system
            //! delivering a resize. Until that happens the window and
            //! everything in it has an empty geometry and is clipped.
            class TestWindow : public Window
            {
            protected:
                TestWindow() = default;

            public:
                static std::shared_ptr<TestWindow> create(
                    const std::shared_ptr<Context>& context,
                    const std::shared_ptr<App>& app,
                    const std::string& title)
                {
                    auto out = std::shared_ptr<TestWindow>(new TestWindow);
                    out->_init(context, app, title, Size2I(1280, 960));
                    return out;
                }

                void resize(const Size2I& size)
                {
                    _setSize(size, size);
                }
            };

            //! The text of the button Return would press.
            std::string keyFocusText(const std::shared_ptr<IWindow>& window)
            {
                auto button = std::dynamic_pointer_cast<PushButton>(
                    window->getKeyFocus());
                return button ? button->getText() : std::string();
            }
        }

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
            // Whichever button has the key focus is the one Return presses,
            // so it has to be the cancel: the focus order on its own gives it
            // to the confirm, which is how a stray Return could overwrite a
            // file. Checked both on a window that is already laid out and on
            // one that is not yet, because opening runs before the first
            // layout and a widget that is still clipped drops the focus again
            // as soon as it is laid out.
            // 0: laid out before the dialog opens.
            // 1: laid out after the dialog opens, before it is ever clipped.
            // 2: ticked with no geometry first, so the dialog is clipped and
            //    drops the focus, and only laid out after that.
            for (int i = 0; i < 3; ++i)
            {
                std::vector<std::string> argv;
                argv.push_back("ConfirmDialogTest");
                auto app = App::create(
                    _context,
                    argv,
                    "ConfirmDialogTest",
                    "Confirmation dialog test.");
                auto window = TestWindow::create(_context, app, "ConfirmDialogTest");
                window->show();
                if (0 == i)
                {
                    window->resize(Size2I(1280, 960));
                }
                app->tick();

                auto system = _context->getSystem<DialogSystem>();
                bool confirmed = false;
                auto dialog = system->confirm(
                    "Confirm",
                    "This is a confirmation?",
                    window,
                    [&confirmed](bool value) { confirmed = value; });

                if (2 == i)
                {
                    app->tick();
                }
                if (0 != i)
                {
                    window->resize(Size2I(1280, 960));
                }
                app->tick();
                _print(Format("Case {0} key focus: {1}").
                    arg(i).
                    arg(keyFocusText(window)));
                FTK_CHECK("Cancel" == keyFocusText(window));

                dialog->close();
                app->tick();
            }
        }
    }
}
