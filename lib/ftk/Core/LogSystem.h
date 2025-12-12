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
    enum class FTK_API_TYPE LogType
    {
        Message,
        Warning,
        Error
    };
        
    //! Log item.
    struct FTK_API_TYPE LogItem
    {
        float       time    = 0.F;
        std::string prefix;
        std::string message;
        LogType     type    = LogType::Message;

        bool operator == (const LogItem&) const;
        bool operator != (const LogItem&) const;
    };

    //! Convert a log item to a string.
    FTK_API std::string toString(const LogItem&);
        
    //! Log system.
    class FTK_API_TYPE LogSystem : public ISystem
    {
    protected:
        LogSystem(const std::shared_ptr<Context>&);

    public:
        FTK_API virtual ~LogSystem();

        //! Create a new system.
        FTK_API static std::shared_ptr<LogSystem> create(const std::shared_ptr<Context>&);

        //! Print to the log.
        FTK_API void print(
            const std::string& prefix,
            const std::string&,
            LogType = LogType::Message);
            
        //! Observe the log items.
        FTK_API std::shared_ptr<IObservableList<LogItem> > observeLogItems() const;

        FTK_API void tick() override;
        FTK_API std::chrono::milliseconds getTickTime() const override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
