// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Util.h>

#include <chrono>
#include <memory>
#include <string>

namespace ftk
{
    class Context;

    //! Base class for systems.
    class FTK_API_TYPE ISystem : public std::enable_shared_from_this<ISystem>
    {
        FTK_NON_COPYABLE(ISystem);

    protected:
        ISystem(
            const std::shared_ptr<Context>&,
            const std::string& name);

    public:
        FTK_API virtual ~ISystem() = 0;

        //! Get the context.
        std::shared_ptr<Context> getContext() const;

        //! Get the system name.
        const std::string& getName() const;

        //! Tick the system.
        FTK_API virtual void tick();

        //! Get the system tick time interval.
        FTK_API virtual std::chrono::milliseconds getTickTime() const;

    protected:
        std::weak_ptr<Context> _context;
        std::string _name;
    };
}

#include <ftk/Core/ISystemInline.h>

