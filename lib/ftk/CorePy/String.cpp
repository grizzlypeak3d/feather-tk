// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/String.h>

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
