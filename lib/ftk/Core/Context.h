// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>
#include <ftk/Core/LogSystem.h>

#include <chrono>
#include <list>
#include <map>
#include <memory>
#include <optional>

namespace ftk
{
    class IBaseSystem;

    //! The context provides centralized access to systems and other
    //! resources.
    class FTK_CORE_API_TYPE Context : public std::enable_shared_from_this<Context>
    {
        FTK_NON_COPYABLE(Context);

    protected:
        void _init();

        Context() = default;

    public:
        FTK_CORE_API ~Context();

        //! Create a new context.
        FTK_CORE_API static std::shared_ptr<Context> create();

        //! Add a system.
        FTK_CORE_API void addSystem(const std::shared_ptr<IBaseSystem>&);

        //! Get the systems.
        FTK_CORE_API const std::list<std::shared_ptr<IBaseSystem> >& getSystems() const;

        //! Get a system by type.
        template<typename T>
        std::shared_ptr<T> getSystem() const;

        //! Get a system by name.
        FTK_CORE_API std::shared_ptr<IBaseSystem> getSystemByName(const std::string&) const;

        //! Get the log system.
        const std::shared_ptr<LogSystem>& getLogSystem() const;

        //! Print to the log.
        FTK_CORE_API void log(
            const std::string& prefix,
            const std::string&,
            LogType = LogType::Message);

        //! Tick the context.
        FTK_CORE_API void tick();

        //! Whether any system is busy; see IBaseSystem::isBusy().
        FTK_CORE_API bool isBusy() const;

        //! When the frame being made now will be on screen: the refresh it
        //! will be shown at, when the application's loop is paced to the
        //! display, for a player to show the picture of that time rather
        //! than of the moment it happened to be asked. Nothing when it is
        //! not known, and the time of asking is the best there is.
        FTK_CORE_API void setFrameTime(
            const std::optional<std::chrono::steady_clock::time_point>&,
            double period = 0.0);
        FTK_CORE_API const std::optional<std::chrono::steady_clock::time_point>& getFrameTime() const;

        //! The display's refresh period, in seconds, with the frame time;
        //! zero when not known.
        FTK_CORE_API double getFramePeriod() const;

        //! Stop the systems' threads.
        //!
        //! Call this from the thread that owns the context, before the last
        //! reference to it goes. A system's thread may hold a reference of
        //! its own while it works, and whichever thread drops the last one
        //! runs this destructor: if that is a system's own thread, the system
        //! ends up joining the thread it is running on. Stopping the threads
        //! here means there is no such thread left to be last.
        //!
        //! Safe to call more than once. The destructor calls it, which covers
        //! a context that is only ever used from one thread, but not one
        //! whose systems are still working when it is let go.
        FTK_CORE_API void shutdown();

    private:
        std::shared_ptr<LogSystem> _logSystem;
        std::list<std::shared_ptr<IBaseSystem> > _systems;
        std::map<std::shared_ptr<IBaseSystem>, std::chrono::steady_clock::time_point> _systemTimes;
        std::optional<std::chrono::steady_clock::time_point> _frameTime;
        double _framePeriod = 0.0;
    };
}

#include <ftk/Core/ContextInline.h>

