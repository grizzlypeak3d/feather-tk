// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Math.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void math(py::module_& m)
        {
            m.attr("pi") = pi;
            m.attr("pi2") = pi2;

            m.def("deg2rad", [](float v) { return deg2rad(v); });
            m.def("rad2deg", [](float v) { return rad2deg(v); });

            m.def("clamp", [](int value, int min, int max) { return clamp(value, min, max); });
            m.def("clamp", [](double value, double min, double max) { return clamp(value, min, max); });

            m.def("lerp", [](double value, int min, int max) { return lerp(value, min, max); });
            m.def("lerp", [](double value, double min, double max) { return lerp(value, min, max); });

            m.def("smoothStep", [](double value, double min, double max) { return smoothStep(value, min, max); });

            m.def("digits", [](int v) { return digits(v); });

            m.def("fuzzyCompare", [](double a, double b) { return fuzzyCompare(a, b); });
            m.def("fuzzyCompare", [](double a, double b, double e) { return fuzzyCompare(a, b, e); });
        }
    }
}
