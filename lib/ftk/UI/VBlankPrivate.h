// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <chrono>
#include <memory>
#include <vector>

struct SDL_Window;

namespace ftk
{
    //! A monitor's vertical blanks as the system gives them: a thread
    //! waits on the monitor a window is on and notes when each one went
    //! by. Where a present does not wait for its refresh -- on Windows a
    //! driver queues frames ahead, and a present returns at once while
    //! there is room -- this is what the window's refresh estimate is made
    //! from instead. Windows (DXGI) only; nothing elsewhere.
    class VBlankWatcher
    {
    public:
        explicit VBlankWatcher(SDL_Window*);
        ~VBlankWatcher();

        //! Whether vertical blanks are coming in.
        bool isActive() const;

        //! The vertical blanks since the last call.
        std::vector<std::chrono::steady_clock::time_point> take();

    private:
        struct Private;
        std::unique_ptr<Private> _p;
    };
}
