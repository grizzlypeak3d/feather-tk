// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Observable.h>

namespace ftk
{
    //! \name Commands
    ///@{
        
    //! Base class for commands.
    class ICommand : public std::enable_shared_from_this<ICommand>
    {
        FTK_NON_COPYABLE(ICommand);

    protected:
        ICommand() = default;

    public:
        virtual ~ICommand() = 0;

        virtual void exec() = 0;
        virtual void undo() = 0;
    };

    //! Command stack.
    class CommandStack : public std::enable_shared_from_this<CommandStack>
    {
        FTK_NON_COPYABLE(CommandStack);

    protected:
        CommandStack();

    public:
        ~CommandStack();

        //! Create a new command stack.
        static std::shared_ptr<CommandStack> create();

        //! Execute a command and push it onto the stack.
        void push(const std::shared_ptr<ICommand>&);

        //! Clear the stack.
        void clear();

        //! Observer whether the stack can undo a command.
        std::shared_ptr<IObservable<bool> > observeHasUndo() const;

        //! Observer whether the stack can redo a command.
        std::shared_ptr<IObservable<bool> > observeHasRedo() const;

        //! Undo a command.
        void undo();

        //! Redo a command.
        void redo();

    private:
        FTK_PRIVATE();
    };

    ///@}
}
