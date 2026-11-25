// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

#include <vector>

namespace ftk
{
    //! \name Noise
    ///@{
        
    //! Noise
    //! 
    //! References:
    //! - https://mrl.cs.nyu.edu/~perlin/noise/
    class FTK_CORE_API_TYPE Noise
    {
    public:
        FTK_CORE_API Noise();

        //! Get the noise at a point, roughly between -1 and 1. It is zero where
        //! x, y and z are all whole numbers, and repeats every 256 units on
        //! each axis. Every Noise gives the same values.
        FTK_CORE_API double get(double x, double y, double z);

    private:
        std::vector<int> _p;
    };

    ///@}
}
