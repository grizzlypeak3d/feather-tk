// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/Action.h>

#include <ftk/Core/Observable.h>

#include <vector>

namespace ftk
{
    //! \name Actions
    ///@{

    //! Action group type.
    enum class FTK_UI_API_TYPE ActionGroupType
    {
        Click,
        Check,
        Radio,
        Toggle,

        Count,
        First = Click
    };
    FTK_ENUM(FTK_UI_API, ActionGroupType);

    //! A group of related actions.
    //!
    //! ButtonGroup does this for widgets, and cannot be used for actions: a
    //! menu makes its own buttons, so the application never sees them, and one
    //! action commonly drives several -- the same action is a menu item and a
    //! tool bar button at once. Relatedness belongs where identity is, which is
    //! the action rather than the widget.
    //!
    //! Radio is the case worth having. Without it an application writes, for
    //! every set of mutually exclusive actions, an observer that walks the set
    //! calling setChecked(this one == the current one), and creates the actions
    //! with a plain callback so that picking one cannot un-pick it. That works
    //! and is not discoverable from anything.
    class FTK_UI_API_TYPE ActionGroup : public std::enable_shared_from_this<ActionGroup>
    {
        FTK_NON_COPYABLE(ActionGroup);

    protected:
        void _init(ActionGroupType);

        ActionGroup();

    public:
        FTK_UI_API ~ActionGroup();

        //! Create a new group.
        FTK_UI_API static std::shared_ptr<ActionGroup> create(ActionGroupType);

        //! Get the actions.
        FTK_UI_API const std::vector<std::shared_ptr<Action> >& getActions() const;

        //! Add an action. The group sets its check type.
        FTK_UI_API void addAction(const std::shared_ptr<Action>&);

        //! Remove every action, leaving them as they are.
        FTK_UI_API void clear();

        //! \name Checked
        //!
        //! For a radio group: which action is current, by index, or -1 for
        //! none. Setting it is how the application says what the model
        //! actually is -- there is no need to touch the actions themselves.
        ///@{

        FTK_UI_API int getChecked() const;
        FTK_UI_API std::shared_ptr<IObservable<int> > observeChecked() const;
        FTK_UI_API void setChecked(int);

        ///@}

        //! Set the callback. The index is which action, the bool its new
        //! state; for a radio group the bool is always true, since picking
        //! one is the only thing that can happen.
        FTK_UI_API void setCheckedCallback(const std::function<void(int, bool)>&);

    private:
        void _checkedChanged(int index, bool);

        FTK_PRIVATE();
    };

    ///@}
}
