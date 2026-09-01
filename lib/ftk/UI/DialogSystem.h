// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IDialog.h>

#include <ftk/Core/ISystem.h>

namespace ftk
{
    class ChoiceDialog;
    class ConfirmDialog;
    class InputDialog;
    class MessageDialog;

    //! \name Dialogs
    ///@{

    //! Dialog system.
    class FTK_UI_API_TYPE DialogSystem : public ISystem
    {
    protected:
        DialogSystem(const std::shared_ptr<Context>&);

    public:
        FTK_UI_API virtual ~DialogSystem();

        //! Create a new system.
        FTK_UI_API static std::shared_ptr<DialogSystem> create(
            const std::shared_ptr<Context>&);

        //! Open a message dialog.
        FTK_UI_API std::shared_ptr<MessageDialog> message(
            const std::string& title,
            const std::string& text,
            const std::shared_ptr<IWindow>& window);

        //! Open a text input dialog. The callback is called when the
        //! input is accepted; cancelling closes the dialog without it.
        FTK_UI_API std::shared_ptr<InputDialog> input(
            const std::string& title,
            const std::string& text,
            const std::string& value,
            const std::shared_ptr<IWindow>& window,
            const std::function<void(const std::string&)>& callback);

        //! Open a choice dialog. The callback is given the index of the
        //! chosen button; dismissing the dialog chooses nothing.
        FTK_UI_API std::shared_ptr<ChoiceDialog> choice(
            const std::string& title,
            const std::string& text,
            const std::vector<std::string>& choices,
            const std::shared_ptr<IWindow>& window,
            const std::function<void(int)>& callback);

        //! Open a confirmation dialog.
        FTK_UI_API std::shared_ptr<ConfirmDialog> confirm(
            const std::string& title,
            const std::string& text,
            const std::shared_ptr<IWindow>& window,
            const std::function<void(bool)>& callback,
            const std::string& confirm = "OK",
            const std::string& cancel = "Cancel");

    private:
        FTK_PRIVATE();
    };

    ///@}
}
