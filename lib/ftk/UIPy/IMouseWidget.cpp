// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IMouseWidget.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void iMouseWidget(py::module_& m)
        {
            py::class_<IMouseWidget, std::shared_ptr<IMouseWidget>>(m, "IMouseWidget");
        }
    }
}
