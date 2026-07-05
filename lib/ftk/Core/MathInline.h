// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <algorithm>

namespace ftk
{
    constexpr float deg2rad(float value)
    {
        return value / 360.F * pi2;
    }

    constexpr float rad2deg(float value)
    {
        return value / pi2 * 360.F;
    }

    template<typename T>
    constexpr T clamp(T value, T min, T max)
    {
        return std::min(std::max(value, min), max);
    }

    template<class T, class U>
    constexpr T lerp(U value, T min, T max)
    {
        return T(min * (U(1) - value) + max * value);
    }

    constexpr float smoothStep(float value, float min, float max)
    {
        const float t = clamp(value, 0.F, 1.F);
        return lerp(t * t * (3.F - (2.F * t)), min, max);
    }

    constexpr double smoothStep(double value, double min, double max)
    {
        const double t = clamp(value, 0.0, 1.0);
        return lerp(t * t * (3. - (2. * t)), min, max);
    }
}
