// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/IDialogTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/IDialog.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>

namespace ftk
{
    namespace ui_test
    {
        namespace
        {
            //! A dialog holding a short label, so that what each size does to
            //! the content can be read from the label's geometry.
            class TestDialog : public IDialog
            {
            protected:
                void _init(const std::shared_ptr<Context>& context)
                {
                    IDialog::_init(context, "ftk::ui_test::TestDialog");
                    label = Label::create(context, "A short line", shared_from_this());
                }

                TestDialog() = default;

            public:
                static std::shared_ptr<TestDialog> create(
                    const std::shared_ptr<Context>& context)
                {
                    auto out = std::shared_ptr<TestDialog>(new TestDialog);
                    out->_init(context);
                    return out;
                }

                std::shared_ptr<Label> label;
            };
        }

        IDialogTest::IDialogTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::IDialogTest")
        {}

        IDialogTest::~IDialogTest()
        {}

        std::shared_ptr<IDialogTest> IDialogTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<IDialogTest>(new IDialogTest(context));
        }

        void IDialogTest::run()
        {
            {
                std::vector<std::string> argv;
                argv.push_back("IDialogTest");
                auto app = App::create(
                    _context,
                    argv,
                    "IDialogTest",
                    "Dialog test.");
                auto window = Window::create(_context, app, "IDialogTest", Size2I(1280, 960));
                window->show();
                app->tick();

                auto dialog = TestDialog::create(_context);
                FTK_CHECK(DialogSize::Fit == dialog->getDialogSize());
                FTK_CHECK(100 == dialog->getTextWidth());
                dialog->open(window);
                app->tick();

                // Fit: the size of the content.
                const Box2I fit = dialog->label->getGeometry();
                FTK_CHECK(fit.size() == dialog->label->getSizeHint());

                // Full: the window, less the margins.
                dialog->setDialogSize(DialogSize::Full);
                dialog->setDialogSize(DialogSize::Full);
                FTK_CHECK(DialogSize::Full == dialog->getDialogSize());
                app->tick();
                const Box2I full = dialog->label->getGeometry();
                FTK_CHECK(full.w() > fit.w());
                FTK_CHECK(full.h() > fit.h());
                FTK_CHECK(full.w() < window->getGeometry().w());

                // Text: wider than the label asks for, narrower than the
                // window, and as tall as Full.
                dialog->setDialogSize(DialogSize::Text, 40);
                FTK_CHECK(DialogSize::Text == dialog->getDialogSize());
                FTK_CHECK(40 == dialog->getTextWidth());
                app->tick();
                const Box2I text = dialog->label->getGeometry();
                FTK_CHECK(text.w() > fit.w());
                FTK_CHECK(text.w() < full.w());
                FTK_CHECK(text.h() == full.h());

                // A text width wider than the window is held to it.
                dialog->setDialogSize(DialogSize::Text, 10000);
                app->tick();
                FTK_CHECK(dialog->label->getGeometry().w() == full.w());

                // And back.
                dialog->setDialogSize(DialogSize::Fit);
                app->tick();
                FTK_CHECK(dialog->label->getGeometry().size() == fit.size());

                FTK_CHECK(to_string(DialogSize::Text) == "Text");
                dialog->close();
                app->tick();
            }
        }
    }
}
