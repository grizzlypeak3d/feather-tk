// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/LogSystem.h>
#include <ftk/Core/ObservableList.h>

#include <string>

namespace ftk
{
    class Context;

    //! System log model.
    //! 
    //! The system log model keeps a buffer of log items for display in the UI.
    class SysLogModel : public std::enable_shared_from_this<SysLogModel>
    {
        FTK_NON_COPYABLE(SysLogModel);

    protected:
        void _init(const std::shared_ptr<Context>&);

        SysLogModel();

    public:
        virtual ~SysLogModel();

        //! Create a new model.
        static std::shared_ptr<SysLogModel> create(const std::shared_ptr<Context>&);

        //! Observe messages (warnings and errors).
        //!
        //! The items are unformatted: a status bar wants the message alone,
        //! while a list of what has happened wants the time with it, so the
        //! choice belongs to whoever is showing them. See getLabel().
        std::shared_ptr<IObservableList<LogItem> > observeMessages() const;

        //! Clear the messages.
        void clearMessages();

        //! Observe the log.
        std::shared_ptr<IObservableList<std::string> > observeLog() const;

        //! Clear the log.
        void clearLog();

    private:
        FTK_PRIVATE();
    };
}
