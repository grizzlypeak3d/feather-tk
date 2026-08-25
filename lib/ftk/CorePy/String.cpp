// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/String.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

#include <sstream>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void string(py::module_& m)
        {
            py::enum_<ElideMode>(m, "ElideMode")
                .value("Right", ElideMode::Right)
                .value("Left", ElideMode::Left)
                .value("Middle", ElideMode::Middle);

            m.def(
                "elide",
                &elide,
                py::arg("value"),
                py::arg("max") = 32,
                py::arg("mode") = ElideMode::Right);

            m.def(
                "getLoremIpsum",
                py::overload_cast<>(&getLoremIpsum));
            m.def(
                "getLoremIpsum",
                py::overload_cast<size_t>(&getLoremIpsum),
                py::arg("lineCount"));
        }
    }
}
