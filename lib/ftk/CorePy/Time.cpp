// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Time.h>

#include <pybind11/pybind11.h>
#include <pybind11/chrono.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void time(py::module_& m)
        {
            m.def(
                "sleep",
                py::overload_cast<const std::chrono::microseconds&>(&sleep),
                "Sleep for a given time.");
            m.def(
                "sleep",
                py::overload_cast<
                    const std::chrono::microseconds&,
                    const std::chrono::steady_clock::time_point&,
                    const std::chrono::steady_clock::time_point&>(&sleep),
                "Sleep up to the given time.");

            m.def("toRational", &toRational, "Convert a floating point rate to a rational.");
        }
    }
}
