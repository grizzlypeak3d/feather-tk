// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/ObservableValue.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void observableValue(py::module_& m)
        {
            observableValue<int>(m, "I");
            observableValue<float>(m, "F");
            observableValue<double>(m, "D");
            observableValue<bool>(m, "Bool");
        }
    }
}
