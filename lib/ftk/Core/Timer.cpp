// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Timer.h>

#include <ftk/Core/Context.h>

#include <vector>

namespace ftk
{
    struct Timer::Private
    {
        bool repeating = false;
        bool active = false;
        std::chrono::microseconds timeout;
        std::function<void(void)> callback;
        std::function<void(
            const std::chrono::steady_clock::time_point&,
            const std::chrono::microseconds&)> callback2;
        std::chrono::steady_clock::time_point start;
    };

    void Timer::_init(const std::shared_ptr<Context>& context)
    {
        FTK_P();
        if (auto system = context->getSystem<TimerSystem>())
        {
            system->addTimer(shared_from_this());
        }
    }

    Timer::Timer() :
        _p(new Private)
    {}

    Timer::~Timer()
    {}

    std::shared_ptr<Timer> Timer::create(
        const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<Timer>(new Timer);
        out->_init(context);
        return out;
    }

    bool Timer::isRepeating() const
    {
        return _p->repeating;
    }

    void Timer::setRepeating(bool value)
    {
        _p->repeating = value;
    }

    void Timer::start(
        const std::chrono::microseconds& timeout,
        const std::function<void(void)>& callback)
    {
        FTK_P();
        p.active = true;
        p.timeout = timeout;
        p.callback = callback;
        p.start = std::chrono::steady_clock::now();
    }

    void Timer::start(
        const std::chrono::microseconds& timeout,
        const std::function<void(
            const std::chrono::steady_clock::time_point&,
            const std::chrono::microseconds&)>& callback)
    {
        FTK_P();
        p.active = true;
        p.timeout = timeout;
        p.callback2 = callback;
        p.start = std::chrono::steady_clock::now();
    }

    void Timer::stop()
    {
        _p->active = false;
    }

    bool Timer::isActive() const
    {
        return _p->active;
    }

    const std::chrono::microseconds& Timer::getTimeout() const
    {
        return _p->timeout;
    }

    void Timer::tick()
    {
        FTK_P();
        if (p.active)
        {
            const auto now = std::chrono::steady_clock::now();
            if (now >= (p.start + p.timeout))
            {
                if (p.callback)
                {
                    p.callback();
                }
                if (p.callback2)
                {
                    p.callback2(
                        now,
                        std::chrono::duration_cast<std::chrono::microseconds>(now - p.start));
                }
                if (p.repeating)
                {
                    p.start = now;
                }
                else
                {
                    p.active = false;
                }
            }
        }
    }

    struct TimerSystem::Private
    {
        std::vector<std::weak_ptr<Timer> > timers;
    };

    TimerSystem::TimerSystem(const std::shared_ptr<Context>& context) :
        ISystem(context, "ftk::TimerSystem"),
        _p(new Private)
    {}

    TimerSystem::~TimerSystem()
    {}

    std::shared_ptr<TimerSystem> TimerSystem::create(
        const std::shared_ptr<Context>& context)
    {
        auto out = context->getSystem<TimerSystem>();
        if (!out)
        {
            out = std::shared_ptr<TimerSystem>(new TimerSystem(context));
        }
        return out;
    }

    void TimerSystem::addTimer(const std::shared_ptr<Timer>& timer)
    {
        _p->timers.push_back(timer);
    }

    void TimerSystem::tick()
    {
        FTK_P();

        // Delete the expired timers.
        auto timers = p.timers;
        auto i = timers.begin();
        while (i != timers.end())
        {
            if (i->expired())
            {
                i = timers.erase(i);
            }
            else
            {
                ++i;
            }
        }

        // Tick the active timers.
        for (i = timers.begin(); i != timers.end(); ++i)
        {
            if (auto timer = i->lock())
            {
                if (timer->isActive())
                {
                    timer->tick();
                }
            }
        }
    }

    std::chrono::milliseconds TimerSystem::getTickTime() const
    {
        return std::chrono::milliseconds(1);
    }
}