// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/LogSystem.h>

#include <chrono>
#include <list>
#include <map>
#include <memory>

namespace ftk
{
    class ISystem;

    //! The context provides centralized access to systems and other
    //! resources.
    class FTK_API_TYPE Context : public std::enable_shared_from_this<Context>
    {
        FTK_NON_COPYABLE(Context);

    protected:
        void _init();

        Context() = default;

    public:
        FTK_API ~Context();

        //! Create a new context.
        FTK_API static std::shared_ptr<Context> create();

        //! Add a system.
        FTK_API void addSystem(const std::shared_ptr<ISystem>&);

        //! Get the systems.
        FTK_API const std::list<std::shared_ptr<ISystem> >& getSystems() const;

        //! Get a system by type.
        template<typename T>
        std::shared_ptr<T> getSystem() const;

        //! Get a system by name.
        FTK_API std::shared_ptr<ISystem> getSystemByName(const std::string&) const;

        //! Get the log system.
        const std::shared_ptr<LogSystem>& getLogSystem() const;

        //! Print to the log.
        FTK_API void log(
            const std::string& prefix,
            const std::string&,
            LogType = LogType::Message);

        //! Tick the context.
        FTK_API void tick();

    private:
        std::shared_ptr<LogSystem> _logSystem;
        std::list<std::shared_ptr<ISystem> > _systems;
        std::map<std::shared_ptr<ISystem>, std::chrono::steady_clock::time_point> _systemTimes;
    };
}

#include <ftk/Core/ContextInline.h>

