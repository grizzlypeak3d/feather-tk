// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/BellowsTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Bellows.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        BellowsTest::BellowsTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::BellowsTest")
        {}

        BellowsTest::~BellowsTest()
        {}

        std::shared_ptr<BellowsTest> BellowsTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<BellowsTest>(new BellowsTest(context));
        }
                
        void BellowsTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("BellowsTest");
                auto app = App::create(
                    context,
                    argv,
                    "BellowsTest",
                    "Bellows test.");
                auto window = Window::create(context, app, "BellowsTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto bellows = Bellows::create(context, "Bellows", layout);
                bellows->setText("Test");
                FTK_ASSERT("Test" == bellows->getText());
                auto label = Label::create(context, "Label");
                bellows->setWidget(label);
                bellows->setWidget(label);
                FTK_ASSERT(label == bellows->getWidget());
                bellows->setOpen(true);
                bellows->setOpen(true);
                FTK_ASSERT(bellows->isOpen());
                bellows->setOpen(false);

                bellows->hide();
                app->tick();
                bellows->show();
                app->tick();
            }
        }
    }
}

