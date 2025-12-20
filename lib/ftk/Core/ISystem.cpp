// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the tlRender project.

#include <ftk/Core/ISystem.h>

#include <ftk/Core/Context.h>

namespace ftk
{
    ISystem::ISystem(
        const std::shared_ptr<Context>& context,
        const std::string& name) :
        IBaseSystem(context, name)
    {
        auto logSystem = context->getSystem<LogSystem>();
        logSystem->print(_name, "Create...");
        _logSystem = logSystem;
    }

    ISystem::~ISystem()
    {
        if (auto logSystem = _logSystem.lock())
        {
            logSystem->print(_name, "Destroy...");
        }
    }

    void ISystem::_log(const std::string& value, LogType type)
    {
        if (auto logSystem = _logSystem.lock())
        {
            logSystem->print(_name, value, type);
        }
    }
}
