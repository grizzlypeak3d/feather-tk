// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DialogSystem.h>

#include <ftk/UI/ChoiceDialog.h>

#include <ftk/UI/ConfirmDialog.h>
#include <ftk/UI/InputDialog.h>
#include <ftk/UI/MessageDialog.h>

namespace ftk
{
    struct DialogSystem::Private
    {
        std::shared_ptr<MessageDialog> messageDialog;
        std::shared_ptr<ConfirmDialog> confirmDialog;
        std::shared_ptr<InputDialog> inputDialog;
        std::shared_ptr<ChoiceDialog> choiceDialog;
    };

    DialogSystem::DialogSystem(const std::shared_ptr<Context>& context) :
        ISystem(context, "ftk::DialogSystem"),
        _p(new Private)
    {}

    DialogSystem::~DialogSystem()
    {}

    std::shared_ptr<DialogSystem> DialogSystem::create(const std::shared_ptr<Context>& context)
    {
        return std::shared_ptr<DialogSystem>(new DialogSystem(context));
    }

    std::shared_ptr<MessageDialog> DialogSystem::message(
        const std::string& title,
        const std::string& text,
        const std::shared_ptr<IWindow>& window)
    {
        FTK_P();
        if (p.messageDialog)
        {
            p.messageDialog->close();
            p.messageDialog.reset();
        }
        auto context = _context.lock();
        p.messageDialog = MessageDialog::create(context, title, text);
        p.messageDialog->open(window);
        p.messageDialog->setCallback(
            [this]
            {
                _p->messageDialog->close();
            });
        p.messageDialog->setCloseCallback(
            [this]
            {
                _p->messageDialog.reset();
            });
        return p.messageDialog;
    }

    std::shared_ptr<InputDialog> DialogSystem::input(
        const std::string& title,
        const std::string& text,
        const std::string& value,
        const std::shared_ptr<IWindow>& window,
        const std::function<void(const std::string&)>& callback)
    {
        FTK_P();
        if (p.inputDialog)
        {
            p.inputDialog->close();
            p.inputDialog.reset();
        }
        auto context = _context.lock();
        p.inputDialog = InputDialog::create(context, title, text, value);
        p.inputDialog->open(window);
        p.inputDialog->setCallback(
            [this, callback](const std::string& value)
            {
                callback(value);
                _p->inputDialog->close();
            });
        p.inputDialog->setCloseCallback(
            [this]
            {
                _p->inputDialog.reset();
            });
        return p.inputDialog;
    }

    std::shared_ptr<ChoiceDialog> DialogSystem::choice(
        const std::string& title,
        const std::string& text,
        const std::vector<std::string>& choices,
        const std::shared_ptr<IWindow>& window,
        const std::function<void(int)>& callback)
    {
        FTK_P();
        if (p.choiceDialog)
        {
            p.choiceDialog->close();
            p.choiceDialog.reset();
        }
        auto context = _context.lock();
        p.choiceDialog = ChoiceDialog::create(context, title, text, choices);
        p.choiceDialog->open(window);
        p.choiceDialog->setCallback(
            [this, callback](int value)
            {
                callback(value);
                _p->choiceDialog->close();
            });
        p.choiceDialog->setCloseCallback(
            [this]
            {
                _p->choiceDialog.reset();
            });
        return p.choiceDialog;
    }

    std::shared_ptr<ConfirmDialog> DialogSystem::confirm(
        const std::string& title,
        const std::string& text,
        const std::shared_ptr<IWindow>& window,
        const std::function<void(bool)>& callback,
        const std::string& confirm,
        const std::string& cancel)
    {
        FTK_P();
        if (p.confirmDialog)
        {
            p.confirmDialog->close();
            p.confirmDialog.reset();
        }
        auto context = _context.lock();
        p.confirmDialog = ConfirmDialog::create(context, title, text, confirm, cancel);
        p.confirmDialog->open(window);
        p.confirmDialog->setCallback(
            [this, callback](bool value)
            {
                callback(value);
                _p->confirmDialog->close();
            });
        p.confirmDialog->setCloseCallback(
            [this]
            {
                _p->confirmDialog.reset();
            });
        return p.confirmDialog;
    }
}
