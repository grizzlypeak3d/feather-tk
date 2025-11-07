// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/IconTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Icon.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Time.h>

namespace ftk
{
    namespace ui_test
    {
        IconTest::IconTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::IconTest")
        {}

        IconTest::~IconTest()
        {}

        std::shared_ptr<IconTest> IconTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<IconTest>(new IconTest(context));
        }
                
        void IconTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("IconTest");
                auto app = App::create(
                    context,
                    argv,
                    "IconTest",
                    "Icon test.");
                auto window = Window::create(context, app, "IconTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto widget = Icon::create(context, "PlaybackForward", layout);
                widget->setIcon("PlaybackStop");
                widget->setIcon("PlaybackStop");
                FTK_ASSERT("PlaybackStop" == widget->getIcon());
                widget->setIcon("PlaybackForward");
                widget->setIcon("PlaybackForward");
                widget->setMarginRole(SizeRole::Margin);
                widget->setMarginRole(SizeRole::Margin);
                FTK_ASSERT(SizeRole::Margin == widget->getMarginRole());
                widget->setMarginRole(SizeRole::None);

                widget->setEnabled(false);
                app->tick();
                widget->setEnabled(true);
                app->tick();

                widget->hide();
                app->tick();
                widget->show();
                app->tick();

                app->setDisplayScale(2.F);
                app->tick();
                app->setDisplayScale(1.F);
                app->tick();
            }
        }
    }
}

