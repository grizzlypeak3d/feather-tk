// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Time.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/chrono.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void time(nb::module_& m)
        {
            m.def(
                "sleep",
                nb::overload_cast<const std::chrono::microseconds&>(&sleep),
                "Sleep for a given time.");
            m.def(
                "sleep",
                nb::overload_cast<
                    const std::chrono::microseconds&,
                    const std::chrono::steady_clock::time_point&,
                    const std::chrono::steady_clock::time_point&>(&sleep),
                "Sleep up to the given time.");

            m.def("toRational", &toRational, "Convert a floating point rate to a rational.");
        }
    }
}
