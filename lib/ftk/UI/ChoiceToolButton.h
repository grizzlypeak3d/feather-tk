// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IContainer.h>

namespace ftk
{
    class Action;
    class ToolButton;

    //! \name Buttons
    ///@{

    //! One button for a choice among checkable actions, at most one of them
    //! checked: a tool bar with room for one button rather than a row of
    //! them. The button shows the action chosen last, and turns it on and
    //! off; an arrow beside it lists them all. The actions are the state,
    //! so the button follows them however they are changed, from a menu, a
    //! shortcut, or anywhere else.
    class FTK_UI_API_TYPE ChoiceToolButton : public IContainer
    {
    protected:
        FTK_UI_API void _init(
            const std::shared_ptr<Context>&,
            const std::vector<std::shared_ptr<Action> >&,
            const std::shared_ptr<IWidget>& parent);

        FTK_UI_API ChoiceToolButton();

    public:
        FTK_UI_API virtual ~ChoiceToolButton();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ChoiceToolButton> create(
            const std::shared_ptr<Context>&,
            const std::vector<std::shared_ptr<Action> >&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the actions.
        FTK_UI_API const std::vector<std::shared_ptr<Action> >& getActions() const;

        //! Get the action the button stands for: the one checked, or the
        //! one checked last.
        FTK_UI_API std::shared_ptr<Action> getCurrent() const;

        //! Get the button.
        FTK_UI_API const std::shared_ptr<ToolButton>& getButton() const;

        //! Get the button that opens the list.
        FTK_UI_API const std::shared_ptr<ToolButton>& getMenuButton() const;

    private:
        void _update();
        void _openMenu();

        FTK_PRIVATE();
    };

    ///@}
}
