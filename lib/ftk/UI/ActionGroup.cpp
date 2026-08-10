// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ActionGroup.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <algorithm>
#include <array>

namespace ftk
{
    FTK_ENUM_IMPL(
        ActionGroupType,
        "Click",
        "Check",
        "Radio",
        "Toggle");

    struct ActionGroup::Private
    {
        ActionGroupType type = ActionGroupType::Click;
        std::vector<std::shared_ptr<Action> > actions;
        std::shared_ptr<Observable<int> > checked;
        std::function<void(int, bool)> checkedCallback;

        std::vector<std::shared_ptr<Observer<bool> > > checkedObservers;

        //! Set while the group is writing the actions' checked states, so that
        //! the observers those writes fire are not mistaken for the artist
        //! picking something.
        bool updating = false;
    };

    void ActionGroup::_init(ActionGroupType type)
    {
        FTK_P();
        p.type = type;
        p.checked = Observable<int>::create(-1);
    }

    ActionGroup::ActionGroup() :
        _p(new Private)
    {}

    ActionGroup::~ActionGroup()
    {}

    std::shared_ptr<ActionGroup> ActionGroup::create(ActionGroupType type)
    {
        auto out = std::shared_ptr<ActionGroup>(new ActionGroup);
        out->_init(type);
        return out;
    }

    const std::vector<std::shared_ptr<Action> >& ActionGroup::getActions() const
    {
        return _p->actions;
    }

    void ActionGroup::addAction(const std::shared_ptr<Action>& action)
    {
        FTK_P();
        switch (p.type)
        {
        case ActionGroupType::Click:
            action->setCheckType(ActionCheckType::None);
            break;
        case ActionGroupType::Radio:
            action->setCheckType(ActionCheckType::Radio);
            break;
        default:
            action->setCheckType(ActionCheckType::Check);
            break;
        }

        const int index = static_cast<int>(p.actions.size());
        p.actions.push_back(action);

        // Watched rather than intercepted. An action's callbacks are given
        // when it is made and belong to whoever made it; the group has no
        // business replacing them, and does not need to -- what it cares about
        // is the checked state, which is observable.
        p.checkedObservers.push_back(Observer<bool>::create(
            action->observeChecked(),
            [this, index](bool value) { _checkedChanged(index, value); },
            ObserverAction::Suppress));

        if (ActionGroupType::Radio == p.type)
        {
            action->setChecked(index == p.checked->get());
        }
    }

    void ActionGroup::clear()
    {
        FTK_P();
        p.checkedObservers.clear();
        p.actions.clear();
        p.checked->setIfChanged(-1);
    }

    int ActionGroup::getChecked() const
    {
        return _p->checked->get();
    }

    std::shared_ptr<IObservable<int> > ActionGroup::observeChecked() const
    {
        return _p->checked;
    }

    void ActionGroup::setChecked(int value)
    {
        FTK_P();
        if (!p.checked->setIfChanged(value))
            return;
        p.updating = true;
        for (size_t i = 0; i < p.actions.size(); ++i)
        {
            p.actions[i]->setChecked(static_cast<int>(i) == value);
        }
        p.updating = false;
    }

    void ActionGroup::setCheckedCallback(const std::function<void(int, bool)>& value)
    {
        _p->checkedCallback = value;
    }

    void ActionGroup::_checkedChanged(int index, bool value)
    {
        FTK_P();
        if (p.updating)
            return;
        switch (p.type)
        {
        case ActionGroupType::Radio:
        {
            if (!value)
            {
                // The one that was current has been picked again, and the
                // button toggled it off on the way here. There is no such
                // thing as none of them, so it goes straight back.
                p.updating = true;
                p.actions[index]->setChecked(true);
                p.updating = false;
                break;
            }
            const bool changed = index != p.checked->get();
            setChecked(index);
            if (changed && p.checkedCallback)
            {
                p.checkedCallback(index, true);
            }
            break;
        }
        case ActionGroupType::Toggle:
            p.updating = true;
            for (size_t i = 0; i < p.actions.size(); ++i)
            {
                if (static_cast<int>(i) != index)
                {
                    p.actions[i]->setChecked(false);
                }
            }
            p.updating = false;
            if (p.checkedCallback)
            {
                p.checkedCallback(index, value);
            }
            break;
        default:
            if (p.checkedCallback)
            {
                p.checkedCallback(index, value);
            }
            break;
        }
    }

}
