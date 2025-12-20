// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/IBaseSystem.h>

namespace ftk
{
    IBaseSystem::IBaseSystem(
        const std::shared_ptr<Context>& context,
        const std::string& name) :
        _context(context),
        _name(name)
    {}

    IBaseSystem::~IBaseSystem()
    {}

    void IBaseSystem::tick()
    {}

    std::chrono::milliseconds IBaseSystem::getTickTime() const
    {
        return std::chrono::milliseconds(0);
    }
}