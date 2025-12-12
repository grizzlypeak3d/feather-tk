// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/ISystem.h>

#include <functional>

namespace ftk
{
    //! \name Timers
    ///@{
        
    //! Timer.
    class FTK_API_TYPE Timer : public std::enable_shared_from_this<Timer>
    {
        FTK_NON_COPYABLE(Timer);

    protected:
        void _init(const std::shared_ptr<Context>&);

        Timer();

    public:
        FTK_API ~Timer();

        //! Create a new timer.
        FTK_API static std::shared_ptr<Timer> create(
            const std::shared_ptr<Context>&);

        //! Does the timer repeat?
        FTK_API bool isRepeating() const;

        //! Set whether the timer repeats.
        FTK_API void setRepeating(bool);

        //! Start the timer.
        FTK_API void start(
            const std::chrono::microseconds& timeout,
            const std::function<void(void)>& callback);

        //! Start the timer.
        FTK_API void start(
            const std::chrono::microseconds& timeout,
            const std::function<void(
                const std::chrono::steady_clock::time_point&,
                const std::chrono::microseconds&)>& callback);

        //! Stop the timer.
        FTK_API void stop();

        //! Is the timer active?
        FTK_API bool isActive() const;

        //! Get the timeout.
        FTK_API const std::chrono::microseconds& getTimeout() const;

        FTK_API void tick();

    private:
        FTK_PRIVATE();
    };

    //! Timer system.
    class FTK_API_TYPE TimerSystem : public ISystem
    {
    protected:
        TimerSystem(const std::shared_ptr<Context>&);

    public:
        FTK_API virtual ~TimerSystem();

        FTK_API static std::shared_ptr<TimerSystem> create(
            const std::shared_ptr<Context>&);

        FTK_API void addTimer(const std::shared_ptr<Timer>&);

        FTK_API void tick() override;
        FTK_API std::chrono::milliseconds getTickTime() const override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
