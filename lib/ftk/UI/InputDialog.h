// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IDialog.h>

namespace ftk
{
    //! \name Dialogs
    ///@{

    //! Text input dialog.
    class FTK_UI_API_TYPE InputDialog : public IDialog
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::string& text,
            const std::string& value,
            const std::string& confirm,
            const std::string& cancel,
            const std::shared_ptr<IWidget>& parent);

        InputDialog();

    public:
        FTK_UI_API virtual ~InputDialog();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<InputDialog> create(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::string& text,
            const std::string& value,
            const std::string& confirm = "OK",
            const std::string& cancel = "Cancel",
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the callback. It is called once, when the input is accepted
        //! with the confirm button or the Return key; cancelling closes the
        //! dialog without calling it.
        FTK_UI_API void setCallback(const std::function<void(const std::string&)>&);

        FTK_UI_API std::shared_ptr<IWidget> getKeyFocus() const override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
