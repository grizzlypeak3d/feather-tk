// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UITest/ActionGroupTest.h>

#include <ftk/UI/ActionGroup.h>

#include <ftk/Core/Assert.h>

namespace ftk
{
    namespace ui_test
    {
        ActionGroupTest::ActionGroupTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::ui_test::ActionGroupTest")
        {}

        ActionGroupTest::~ActionGroupTest()
        {}

        std::shared_ptr<ActionGroupTest> ActionGroupTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ActionGroupTest>(new ActionGroupTest(context));
        }

        void ActionGroupTest::run()
        {
            _radio();
            _check();
            _enums();
        }

        void ActionGroupTest::_radio()
        {
            auto group = ActionGroup::create(ActionGroupType::Radio);
            int picked = -1;
            int fired = 0;
            group->setCheckedCallback(
                [this, &picked, &fired](int index, bool value)
                {
                    FTK_CHECK(value);
                    picked = index;
                    ++fired;
                });

            std::vector<std::shared_ptr<Action> > actions;
            for (int i = 0; i < 3; ++i)
            {
                auto action = Action::create("Action", [] {});
                actions.push_back(action);
                group->addAction(action);
                // Joining a radio group is what makes an action one of many.
                FTK_CHECK(ActionCheckType::Radio == action->getCheckType());
                FTK_CHECK(action->isCheckable());
            }
            FTK_CHECK(-1 == group->getChecked());

            // The model says which one, and the actions follow.
            group->setChecked(1);
            FTK_CHECK(!actions[0]->isChecked());
            FTK_CHECK(actions[1]->isChecked());
            FTK_CHECK(!actions[2]->isChecked());
            // Nothing the artist did, so nothing is reported.
            FTK_CHECK(0 == fired);

            // Picking one: the others let go, and it is reported once.
            actions[2]->setChecked(true);
            FTK_CHECK(2 == group->getChecked());
            FTK_CHECK(!actions[1]->isChecked());
            FTK_CHECK(1 == fired);
            FTK_CHECK(2 == picked);

            // The whole point. A button toggles the current one off on its way
            // here; there is no such thing as none of them, so it goes back and
            // nothing is reported.
            actions[2]->setChecked(false);
            FTK_CHECK(actions[2]->isChecked());
            FTK_CHECK(2 == group->getChecked());
            FTK_CHECK(1 == fired);

            // An action added to a group that already has a selection comes in
            // unchecked rather than assuming it.
            auto late = Action::create("Late", [] {});
            group->addAction(late);
            FTK_CHECK(!late->isChecked());
            FTK_CHECK(2 == group->getChecked());

            group->clear();
            FTK_CHECK(group->getActions().empty());
            FTK_CHECK(-1 == group->getChecked());
        }

        void ActionGroupTest::_check()
        {
            auto group = ActionGroup::create(ActionGroupType::Check);
            std::vector<std::pair<int, bool> > reported;
            group->setCheckedCallback(
                [&reported](int index, bool value)
                {
                    reported.push_back(std::make_pair(index, value));
                });

            std::vector<std::shared_ptr<Action> > actions;
            for (int i = 0; i < 2; ++i)
            {
                auto action = Action::create("Action", [](bool) {});
                actions.push_back(action);
                group->addAction(action);
                FTK_CHECK(ActionCheckType::Check == action->getCheckType());
            }

            // Switches are independent, and turning one off is a real thing to
            // want -- which is the difference from a radio group.
            actions[0]->setChecked(true);
            actions[1]->setChecked(true);
            actions[0]->setChecked(false);
            FTK_CHECK(!actions[0]->isChecked());
            FTK_CHECK(actions[1]->isChecked());
            FTK_CHECK(3 == reported.size());
            FTK_CHECK(!reported[2].second);
        }

        void ActionGroupTest::_enums()
        {
            for (auto i : getActionGroupTypeEnums())
            {
                ActionGroupType type = ActionGroupType::First;
                FTK_CHECK(from_string(to_string(i), type));
                FTK_CHECK(i == type);
            }
            for (auto i : getActionCheckTypeEnums())
            {
                ActionCheckType type = ActionCheckType::First;
                FTK_CHECK(from_string(to_string(i), type));
                FTK_CHECK(i == type);
            }
        }
    }
}
