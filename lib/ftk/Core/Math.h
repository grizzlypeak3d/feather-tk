// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

#include <cstddef>

namespace ftk
{
    //! \name Math
    ///@{

    //! Approximate value of PI.
    constexpr float pi = 3.14159265359F;

    //! Approximate value of PI times two.
    constexpr float pi2 = pi * 2.F;

    //! Convert degrees to radians.
    constexpr float deg2rad(float);

    //! Convert radians to degrees.
    constexpr float rad2deg(float);

    //! Clamp a value.
    template<typename T>
    constexpr T clamp(T value, T min, T max);

    //! Linear interpolation.
    template<typename T, typename U>
    constexpr T lerp(U value, T min, T max);

    //! Smooth step function.
    template<typename T>
    constexpr T smoothStep(T value, T min, T max);

    //! Count the number of digits.
    FTK_API size_t digits(int);

    //! Fuzzy double comparison.
    FTK_API bool fuzzyCompare(double a, double b, double e = .1e-9);

    //! Fuzzy float comparison.
    FTK_API bool fuzzyCompare(float a, float b, float e = .1e-6F);
        
    ///@}
}

#include <ftk/Core/MathInline.h>

