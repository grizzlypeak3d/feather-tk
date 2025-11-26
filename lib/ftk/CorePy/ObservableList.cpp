// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/ObservableList.h>

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
            observableList<int>(m, "I");
            observableList<float>(m, "F");
            observableList<double>(m, "D");
            observableList<bool>(m, "Bool");
        }
    }
}
