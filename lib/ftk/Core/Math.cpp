// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Math.h>

#include <cmath>

namespace ftk
{
    int digits(int value)
    {
        int out = 0;
        if (value != 0)
        {
            while (value)
            {
                value /= 10;
                ++out;
            }
        }
        else
        {
            out = 1;
        }
        return out;
    }

    bool fuzzyCompare(double a, double b, double e)
    {
        return fabs(a - b) < e;
    }

    bool fuzzyCompare(float a, float b, float e)
    {
        return fabsf(a - b) < e;
    }
}
