// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/DiagWidget.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void diagWidget(py::module_& m)
        {
            py::class_<DiagWidget, IContainer, std::shared_ptr<DiagWidget> >(m, "DiagWidget")
                .def(
                    py::init(&DiagWidget::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("marginRole", &DiagWidget::getMarginRole, &DiagWidget::setMarginRole);
        }
    }
}
