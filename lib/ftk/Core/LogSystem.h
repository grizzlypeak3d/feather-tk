// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>
#include <ftk/Core/IBaseSystem.h>
#include <ftk/Core/ObservableList.h>

#include <chrono>

namespace ftk
{
    //! \name Log System
    ///@{
        
    //! Log types.
    enum class FTK_CORE_API_TYPE LogType
    {
        Message,
        Warning,
        Error,

        Count,
        First = Message
    };
    FTK_ENUM(FTK_CORE_API, LogType);

    //! Log item.
    struct FTK_CORE_API_TYPE LogItem
    {
        //! When the item was logged. Wall clock rather than time since the
        //! application started: a log is read after the fact, and lining an
        //! entry up with when something happened is the whole point of it.
        std::chrono::system_clock::time_point time;

        //! What logged the item, such as "ftk::FontSystem".
        std::string prefix;
        //! The message.
        std::string message;
        //! Whether the item is a message, a warning, or an error.
        LogType     type    = LogType::Message;

        bool operator == (const LogItem&) const = default;
    };

    //! What to include when a log item is formatted.
    enum class FTK_CORE_API_TYPE LogLabel
    {
        //! Time, prefix and message: the log file.
        Full,

        //! Time and message, for a list of what has happened. The prefix
        //! names the code that noticed, which does not help whoever reads it.
        Time,

        //! The message alone, for a status bar or an overlay. The time is
        //! noise there: the message has only just appeared.
        Message
    };

    //! Get a log item label: what the given label includes of it.
    FTK_CORE_API std::string getLabel(const LogItem&, LogLabel = LogLabel::Full);
        
    //! Log system.
    class FTK_CORE_API_TYPE LogSystem : public IBaseSystem
    {
    protected:
        LogSystem(const std::shared_ptr<Context>&);

    public:
        FTK_CORE_API virtual ~LogSystem();

        //! Create a new system.
        FTK_CORE_API static std::shared_ptr<LogSystem> create(const std::shared_ptr<Context>&);

        //! Print to the log.
        FTK_CORE_API void print(
            const std::string& prefix,
            const std::string&,
            LogType = LogType::Message);
            
        //! Observe the log items.
        FTK_CORE_API std::shared_ptr<IObservableList<LogItem> > observeLogItems() const;

        FTK_CORE_API void tick() override;
        FTK_CORE_API std::chrono::milliseconds getTickTime() const override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
