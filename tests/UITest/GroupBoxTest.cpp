// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/GroupBoxTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/GroupBox.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        GroupBoxTest::GroupBoxTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::GroupBoxTest")
        {}

        GroupBoxTest::~GroupBoxTest()
        {}

        std::shared_ptr<GroupBoxTest> GroupBoxTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<GroupBoxTest>(new GroupBoxTest(context));
        }
                
        void GroupBoxTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("GroupBoxTest");
                auto app = App::create(
                    context,
                    argv,
                    "GroupBoxTest",
                    "Group box test.");
                auto window = Window::create(context, app, "GroupBoxTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto widget = GroupBox::create(context, "Test", layout);
                widget->setText("Group");
                widget->setText("Group");
                FTK_ASSERT("Group" == widget->getText());
                widget->setFontRole(FontRole::Mono);
                widget->setFontRole(FontRole::Mono);
                FTK_ASSERT(FontRole::Mono == widget->getFontRole());
                widget->setFontRole(FontRole::Label);
                app->tick();
            }
        }
    }
}

