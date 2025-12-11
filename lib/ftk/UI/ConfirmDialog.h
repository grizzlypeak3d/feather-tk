// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IDialog.h>

namespace ftk
{
    //! \name Dialogs
    ///@{

    //! Confirmation dialog.
    class FTK_API_TYPE ConfirmDialog : public IDialog
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::string& text,
            const std::string& confirm,
            const std::string& cancel,
            const std::shared_ptr<IWidget>& parent);

        ConfirmDialog();

    public:
        virtual ~ConfirmDialog();

        //! Create a new widget.
        static std::shared_ptr<ConfirmDialog> create(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::string& text,
            const std::string& confirm = "OK",
            const std::string& cancel = "Cancel",
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the callback.
        void setCallback(const std::function<void(bool)>&);

    private:
        FTK_PRIVATE();
    };

    ///@}
}
