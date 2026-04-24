// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ObservableList.h>
#include <ftk/Core/LogSystem.h>

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
        std::shared_ptr<IObservableList<LogItem> > observeMessages() const;

        //! Observe the log.
        std::shared_ptr<IObservableList<LogItem> > observeLog() const;

    private:
        FTK_PRIVATE();
    };
}
