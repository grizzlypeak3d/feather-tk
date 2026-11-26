// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/VBlankPrivate.h>

namespace ftk
{
    struct VBlankWatcher::Private
    {};

    VBlankWatcher::VBlankWatcher(SDL_Window*) :
        _p(new Private)
    {}

    VBlankWatcher::~VBlankWatcher()
    {}

    bool VBlankWatcher::isActive() const
    {
        return false;
    }

    std::vector<std::chrono::steady_clock::time_point> VBlankWatcher::take()
    {
        return {};
    }
}
