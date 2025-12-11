// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

#include <nlohmann/json.hpp>

#include <cstddef>
#include <iostream>

namespace ftk
{
    //! \name Ranges
    ///@{
        
    //! Numeric range.
    template<typename T>
    class Range
    {
    public:
        constexpr Range();
        constexpr Range(T min, T max);

        constexpr T min() const;
        constexpr T max() const;

        constexpr bool equal() const;

        constexpr bool operator == (const Range<T>&) const;
        constexpr bool operator != (const Range<T>&) const;
        constexpr bool operator < (const Range<T>&) const;

    private:
        T _min;
        T _max;
    };

    typedef Range<int> RangeI;
    typedef Range<int64_t> RangeI64;
    typedef Range<std::size_t> RangeSizeT;
    typedef Range<float> RangeF;
    typedef Range<double> RangeD;

    //! Does the range contain the given number?
    template<typename T>
    bool contains(const Range<T>&, T);

    //! Does the range interset the given range?
    template<typename T>
    bool intersects(const Range<T>&, const Range<T>&);

    //! Expand the range to include the given number.
    template<typename T>
    Range<T> expand(const Range<T>&, T);

    //! Expand the range to include the given range.
    template<typename T>
    Range<T> expand(const Range<T>&, const Range<T>&);

    FTK_API void to_json(nlohmann::json&, const RangeI&);
    FTK_API void to_json(nlohmann::json&, const RangeI64&);
    FTK_API void to_json(nlohmann::json&, const RangeSizeT&);
    FTK_API void to_json(nlohmann::json&, const RangeF&);
    FTK_API void to_json(nlohmann::json&, const RangeD&);

    FTK_API void from_json(const nlohmann::json&, RangeI&);
    FTK_API void from_json(const nlohmann::json&, RangeI64&);
    FTK_API void from_json(const nlohmann::json&, RangeSizeT&);
    FTK_API void from_json(const nlohmann::json&, RangeF&);
    FTK_API void from_json(const nlohmann::json&, RangeD&);

    template<typename T>
    std::ostream& operator << (std::ostream&, const Range<T>&);

    ///@}

}

#include <ftk/Core/RangeInline.h>
