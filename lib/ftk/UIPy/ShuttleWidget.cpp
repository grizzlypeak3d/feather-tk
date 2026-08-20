// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ShuttleWidget.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void shuttleWidget(py::module_& m)
        {
            py::class_<ShuttleWidget, IMouseWidget, std::shared_ptr<ShuttleWidget> >(m, "ShuttleWidget")
                .def(
                    py::init(&ShuttleWidget::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property_readonly("modifiers", &ShuttleWidget::getModifiers)
                .def("setCallback", &ShuttleWidget::setCallback)
                .def("setDeltaCallback", &ShuttleWidget::setDeltaCallback)
                .def("setActiveCallback", &ShuttleWidget::setActiveCallback);
        }
    }
}
