// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Util.h>

#include <chrono>
#include <utility>

namespace ftk
{
    //! \name Time
    ///@{

    //! Sleep for a given time.
    FTK_API void sleep(const std::chrono::microseconds&);

    //! Sleep up to the given time.
    FTK_API void sleep(
        const std::chrono::microseconds&,
        const std::chrono::steady_clock::time_point& t0,
        const std::chrono::steady_clock::time_point& t1);

    //! Convert a floating point rate to a rational.
    FTK_API std::pair<int, int> toRational(double);
        
    ///@}
}
