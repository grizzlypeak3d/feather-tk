// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ISystem.h>
#include <ftk/Core/ObservableList.h>

#include <chrono>

namespace ftk
{
    //! \name Log System
    ///@{
        
    //! Log types.
    enum class LogType
    {
        Message,
        Warning,
        Error
    };
        
    //! Log item.
    struct LogItem
    {
        float       time    = 0.F;
        std::string prefix;
        std::string message;
        LogType     type    = LogType::Message;

        bool operator == (const LogItem&) const;
        bool operator != (const LogItem&) const;
    };

    //! Convert a log item to a string.
    std::string toString(const LogItem&);
        
    //! Log system.
    class LogSystem : public ISystem
    {
    protected:
        LogSystem(const std::shared_ptr<Context>&);

    public:
        virtual ~LogSystem();

        //! Create a new system.
        static std::shared_ptr<LogSystem> create(const std::shared_ptr<Context>&);

        //! Print to the log.
        void print(
            const std::string& prefix,
            const std::string&,
            LogType = LogType::Message);
            
        //! Observe the log items.
        std::shared_ptr<IObservableList<LogItem> > observeLogItems() const;

        void tick() override;
        std::chrono::milliseconds getTickTime() const override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
