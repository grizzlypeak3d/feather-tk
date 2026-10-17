// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/ChoiceToolButtonTest.h>

#include <ftk/UI/Action.h>
#include <ftk/UI/App.h>
#include <ftk/UI/ChoiceToolButton.h>
#include <ftk/UI/ToolButton.h>
#include <ftk/UI/Window.h>

#include <ftk/Core/Assert.h>

namespace ftk
{
    namespace ui_test
    {
        ChoiceToolButtonTest::ChoiceToolButtonTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ChoiceToolButtonTest")
        {}

        ChoiceToolButtonTest::~ChoiceToolButtonTest()
        {}

        std::shared_ptr<ChoiceToolButtonTest> ChoiceToolButtonTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ChoiceToolButtonTest>(new ChoiceToolButtonTest(context));
        }

        void ChoiceToolButtonTest::run()
        {
            std::vector<std::string> argv;
            argv.push_back("ChoiceToolButtonTest");
            auto app = App::create(
                _context,
                argv,
                "ChoiceToolButtonTest",
                "Choice tool button test.");
            auto window = Window::create(_context, app, "ChoiceToolButtonTest");
            window->show();
            app->tick();

            // Three actions that are one choice: checking one unchecks the
            // others, as an application's model would.
            std::vector<std::shared_ptr<Action> > actions;
            const std::vector<std::string> icons = { "Add", "Remove", "Close" };
            for (size_t i = 0; i < icons.size(); ++i)
            {
                actions.push_back(Action::create(
                    icons[i],
                    icons[i],
                    [&actions, i](bool value)
                    {
                        for (size_t j = 0; j < actions.size(); ++j)
                        {
                            actions[j]->setChecked(j == i ? value : false);
                        }
                    }));
            }

            auto widget = ChoiceToolButton::create(_context, actions, window);
            app->tick();
            FTK_CHECK(3 == widget->getActions().size());
            auto button = widget->getButton();

            // Nothing chosen: the first stands in, off.
            FTK_CHECK(actions[0] == widget->getCurrent());
            FTK_CHECK(!button->isChecked());
            FTK_CHECK("Add" == button->getIcon());

            // The button turns it on and off.
            button->click();
            FTK_CHECK(actions[0]->isChecked());
            FTK_CHECK(button->isChecked());
            button->click();
            FTK_CHECK(!actions[0]->isChecked());
            FTK_CHECK(!button->isChecked());

            // Chosen elsewhere, the button follows, and remembers it.
            actions[2]->doCheckedCallback(true);
            FTK_CHECK(actions[2] == widget->getCurrent());
            FTK_CHECK(button->isChecked());
            FTK_CHECK("Close" == button->getIcon());
            button->click();
            FTK_CHECK(!actions[2]->isChecked());
            FTK_CHECK("Close" == button->getIcon());
            button->click();
            FTK_CHECK(actions[2]->isChecked());

            // Disabled, the button is too; the list is while any is not.
            actions[2]->setEnabled(false);
            FTK_CHECK(!button->isEnabled());
            FTK_CHECK(widget->getMenuButton()->isEnabled());
            for (const auto& action : actions)
            {
                action->setEnabled(false);
            }
            FTK_CHECK(!widget->getMenuButton()->isEnabled());
        }
    }
}
