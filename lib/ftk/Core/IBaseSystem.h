// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>
#include <ftk/Core/Util.h>

#include <chrono>
#include <memory>
#include <string>

namespace ftk
{
    class Context;

    //! Base class for systems.
    class FTK_CORE_API_TYPE IBaseSystem : public std::enable_shared_from_this<IBaseSystem>
    {
        FTK_NON_COPYABLE(IBaseSystem);

    protected:
        IBaseSystem(
            const std::shared_ptr<Context>&,
            const std::string& name);

    public:
        FTK_CORE_API virtual ~IBaseSystem() = 0;

        //! Get the context.
        std::shared_ptr<Context> getContext() const;

        //! Get the system name.
        const std::string& getName() const;

        //! Stop the system's threads.
        //!
        //! Called by the context before it lets go of its systems, so that a
        //! system with threads is stopped while the context is still alive
        //! and on the thread that owns it. A thread that has locked the
        //! context weakly and is still running when the last reference goes
        //! would otherwise destroy the context itself, and take its own
        //! system down with it -- leaving the system's destructor joining the
        //! thread it is running on.
        //!
        //! Must be safe to call more than once, and safe to call on a system
        //! that has no threads. The default does nothing.
        FTK_CORE_API virtual void shutdown();

        //! Tick the system.
        FTK_CORE_API virtual void tick();

        //! Get the system tick time interval.
        FTK_CORE_API virtual std::chrono::milliseconds getTickTime() const;

    protected:
        std::weak_ptr<Context> _context;
        std::string _name;
    };
}

#include <ftk/Core/IBaseSystemInline.h>

