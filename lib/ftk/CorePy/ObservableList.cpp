// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void observableList(py::module_& m)
        {
            observableList<int>(m, "Int");
            observableList<float>(m, "Float");
            observableList<double>(m, "Double");
            observableList<bool>(m, "Bool");
            observableList<std::string>(m, "String");
        }
    }
}
