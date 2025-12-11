// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IButton.h>

namespace ftk
{
    //! \name Buttons
    ///@{
        
    //! Button group type.
    enum class FTK_API_TYPE ButtonGroupType
    {
        Click,
        Check,
        Radio,
        Toggle,
        
        Count,
        First = Click
    };
    FTK_ENUM(ButtonGroupType);

    //! Button group.
    class FTK_API_TYPE ButtonGroup : public std::enable_shared_from_this<ButtonGroup>
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            ButtonGroupType);

        ButtonGroup();

    public:
        ~ButtonGroup();

        //! Create a new button group.
        static std::shared_ptr<ButtonGroup> create(
            const std::shared_ptr<Context>&,
            ButtonGroupType);

        //! Get the buttons in the group.
        const std::vector<std::shared_ptr<IButton> >& getButtons() const;

        //! Add a button to the group.
        void addButton(const std::shared_ptr<IButton>&);

        //! Clear the buttons in the group.
        void clearButtons();

        //! Set whether the button is checked.
        void setChecked(int, bool = true);

        //! Set the clicked callback.
        void setClickedCallback(const std::function<void(int)>&);

        //! Set the checked callback.
        void setCheckedCallback(const std::function<void(int, bool)>&);

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
