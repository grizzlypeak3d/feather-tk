// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IDialog.h>

namespace ftk
{
    //! \name Dialogs
    ///@{

    //! Choice dialog: a question answered by one of several buttons,
    //! e.g. save, discard, or cancel. The last choice is the safe one --
    //! it takes the key focus, so the key that takes the dialog at face
    //! value is the one that changes nothing.
    class FTK_UI_API_TYPE ChoiceDialog : public IDialog
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::string& text,
            const std::vector<std::string>& choices,
            const std::shared_ptr<IWidget>& parent);

        ChoiceDialog();

    public:
        FTK_UI_API virtual ~ChoiceDialog();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ChoiceDialog> create(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::string& text,
            const std::vector<std::string>& choices,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the callback. It is given the index of the chosen button;
        //! dismissing the dialog chooses nothing and the callback does
        //! not run.
        FTK_UI_API void setCallback(const std::function<void(int)>&);

        FTK_UI_API std::shared_ptr<IWidget> getKeyFocus() const override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
