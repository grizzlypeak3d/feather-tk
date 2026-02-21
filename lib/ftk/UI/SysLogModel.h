// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ObservableList.h>

#include <string>

namespace ftk
{
    class Context;

    //! System log model.
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

        //! Observe the messages.
        std::shared_ptr<IObservableList<std::string> > observeMessages() const;

        //! Observe the log.
        std::shared_ptr<IObservableList<std::string> > observeLog() const;

    private:
        FTK_PRIVATE();
    };
}
