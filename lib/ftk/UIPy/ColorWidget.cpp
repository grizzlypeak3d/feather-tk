// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ColorWidget.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void colorWidget(nb::module_& m)
        {
            nb::class_<ColorWidget, IContainer>(m, "ColorWidget")
                .def(
                    nb::new_(&ColorWidget::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("color", &ColorWidget::getColor, &ColorWidget::setColor, nb::rv_policy::copy)
                .def("setCallback", &ColorWidget::setCallback)
                .def("setPressedCallback", &ColorWidget::setPressedCallback);
        }
    }
}
