// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the tlRender project.

#pragma once

#include <ftk/Core/IBaseSystem.h>
#include <ftk/Core/LogSystem.h>

namespace ftk
{
    //! Base class for systems.
    class FTK_API_TYPE ISystem : public IBaseSystem
    {
    protected:
        ISystem(
            const std::shared_ptr<Context>&,
            const std::string& name);

    public:
        FTK_API virtual ~ISystem();

    protected:
        FTK_API void _log(const std::string&, LogType = LogType::Message);

    private:
        std::weak_ptr<LogSystem> _logSystem;
    };
}
