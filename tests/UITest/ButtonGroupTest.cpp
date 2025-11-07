// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <UITest/ButtonGroupTest.h>

#include <ftk/UI/App.h>
#include <ftk/UI/ButtonGroup.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace ui_test
    {
        ButtonGroupTest::ButtonGroupTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ButtonGroupTest")
        {}

        ButtonGroupTest::~ButtonGroupTest()
        {}

        std::shared_ptr<ButtonGroupTest> ButtonGroupTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ButtonGroupTest>(new ButtonGroupTest(context));
        }
                
        void ButtonGroupTest::run()
        {
            if (auto context = _context.lock())
            {
                std::vector<std::string> argv;
                argv.push_back("ButtonGroupTest");
                auto app = App::create(
                    context,
                    argv,
                    "ButtonGroupTest",
                    "Button group test.");
                auto window = Window::create(context, app, "ButtonGroupTest");
                auto layout = VerticalLayout::create(context, window);
                layout->setMarginRole(SizeRole::MarginLarge);
                window->show();
                app->tick();

                auto button0 = PushButton::create(context, "Button 0", layout);
                auto button1 = PushButton::create(context, "Button 0", layout);
                auto button2 = PushButton::create(context, "Button 0", layout);
                auto group = ButtonGroup::create(context, ButtonGroupType::Click);
                group->addButton(button0);
                FTK_ASSERT(!group->getButtons().empty());
                group->clearButtons();
                FTK_ASSERT(group->getButtons().empty());
                group->addButton(button0);
                group->addButton(button1);
                group->addButton(button2);
                int clicked = -1;
                group->setClickedCallback(
                    [&clicked](int value)
                    {
                        clicked = value;
                    });

                group = ButtonGroup::create(context, ButtonGroupType::Check);
                group->addButton(button0);
                group->addButton(button1);
                group->addButton(button2);
                clicked = -1;
                bool checked = false;
                group->setCheckedCallback(
                    [&clicked, &checked](int index, bool value)
                    {
                        clicked = index;
                        checked = value;
                    });

                group = ButtonGroup::create(context, ButtonGroupType::Radio);
                group->addButton(button0);
                group->addButton(button1);
                group->addButton(button2);
                clicked = -1;
                checked = false;
                group->setCheckedCallback(
                    [&clicked, &checked](int index, bool value)
                    {
                        clicked = index;
                        checked = value;
                    });

                group = ButtonGroup::create(context, ButtonGroupType::Toggle);
                group->addButton(button0);
                group->addButton(button1);
                group->addButton(button2);
                clicked = -1;
                checked = false;
                group->setCheckedCallback(
                    [&clicked, &checked](int index, bool value)
                    {
                        clicked = index;
                        checked = value;
                    });
            }
        }
    }
}

