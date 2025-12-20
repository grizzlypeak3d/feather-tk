// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

namespace ftk
{
    inline std::shared_ptr<Context> IBaseSystem::getContext() const
    {
        return _context.lock();
    }

    inline const std::string& IBaseSystem::getName() const
    {
        return _name;
    }
}
