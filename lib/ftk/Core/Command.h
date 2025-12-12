// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Observable.h>

namespace ftk
{
    //! \name Commands
    ///@{
        
    //! Base class for commands.
    class FTK_API_TYPE ICommand : public std::enable_shared_from_this<ICommand>
    {
        FTK_NON_COPYABLE(ICommand);

    protected:
        ICommand() = default;

    public:
        FTK_API virtual ~ICommand() = 0;

        FTK_API virtual void exec() = 0;
        FTK_API virtual void undo() = 0;
    };

    //! Command stack.
    class FTK_API_TYPE CommandStack : public std::enable_shared_from_this<CommandStack>
    {
        FTK_NON_COPYABLE(CommandStack);

    protected:
        CommandStack();

    public:
        FTK_API ~CommandStack();

        //! Create a new command stack.
        FTK_API static std::shared_ptr<CommandStack> create();

        //! Execute a command and push it onto the stack.
        FTK_API void push(const std::shared_ptr<ICommand>&);

        //! Clear the stack.
        FTK_API void clear();

        //! Observer whether the stack can undo a command.
        FTK_API std::shared_ptr<IObservable<bool> > observeHasUndo() const;

        //! Observer whether the stack can redo a command.
        FTK_API std::shared_ptr<IObservable<bool> > observeHasRedo() const;

        //! Undo a command.
        FTK_API void undo();

        //! Redo a command.
        FTK_API void redo();

    private:
        FTK_PRIVATE();
    };

    ///@}
}
