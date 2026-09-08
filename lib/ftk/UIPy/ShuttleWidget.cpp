// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ShuttleWidget.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/function.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void shuttleWidget(nb::module_& m)
        {
            nb::class_<ShuttleWidget, IMouseWidget>(m, "ShuttleWidget")
                .def(
                    nb::new_(&ShuttleWidget::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_ro("modifiers", &ShuttleWidget::getModifiers)
                .def("setCallback", &ShuttleWidget::setCallback)
                .def("setDeltaCallback", &ShuttleWidget::setDeltaCallback)
                .def("setActiveCallback", &ShuttleWidget::setActiveCallback);
        }
    }
}
