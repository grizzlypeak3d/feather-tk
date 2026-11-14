// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/BellowsTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/Bellows.h>
#include <ftk/UI/CheckBox.h>
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
            {
                std::vector<std::string> argv;
                argv.push_back("BellowsTest");
                auto app = App::create(
                    _context,
                    argv,
                    "BellowsTest",
                    "Bellows test.");
                auto window = Window::create(_context, app, "BellowsTest");
                auto layout = VerticalLayout::create(_context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto bellows = Bellows::create(_context, "Bellows", layout);
                bellows->setText("Test");
                FTK_CHECK("Test" == bellows->getText());
                auto label = Label::create(_context, "Label");
                bellows->setWidget(label);
                bellows->setWidget(label);
                FTK_CHECK(label == bellows->getWidget());
                bellows->setOpen(true);
                bellows->setOpen(true);
                FTK_CHECK(bellows->isOpen());
                bellows->setOpen(false);

                // A check widget between the arrow and the title, and a tool
                // widget at the right.
                auto checkBox = CheckBox::create(_context);
                bellows->setCheckWidget(checkBox);
                bellows->setCheckWidget(checkBox);
                FTK_CHECK(checkBox == bellows->getCheckWidget());
                auto tool = Label::create(_context, "Tool");
                bellows->setToolWidget(tool);
                FTK_CHECK(tool == bellows->getToolWidget());
                app->tick();
                const Box2I headerBox = checkBox->getParent()->getGeometry();
                FTK_CHECK(checkBox->getGeometry().x() < tool->getGeometry().x());
                FTK_CHECK(checkBox->getGeometry().x() < headerBox.x() + headerBox.w() / 2);
                FTK_CHECK(tool->getGeometry().max.x == headerBox.max.x);
                // The header is the one tab stop; the box keeps no focus of
                // its own there, and has it back when it leaves.
                FTK_CHECK(!checkBox->acceptsKeyFocus());
                FTK_CHECK(!checkBox->hasMouseHighlight());
                bellows->setCheckWidget(nullptr);
                FTK_CHECK(!bellows->getCheckWidget());
                FTK_CHECK(checkBox->acceptsKeyFocus());
                FTK_CHECK(checkBox->hasMouseHighlight());
                bellows->setToolWidget(nullptr);
                app->tick();

                bellows->hide();
                app->tick();
                bellows->show();
                app->tick();
            }
        }
    }
}

