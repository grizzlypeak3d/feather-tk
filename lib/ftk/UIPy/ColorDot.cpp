// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ColorDot.h>

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
        void colorDot(nb::module_& m)
        {
            nb::class_<ColorDot, IWidget>(m, "ColorDot")
                .def(
                    nb::new_(&ColorDot::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("color", &ColorDot::getColor, &ColorDot::setColor, nb::rv_policy::copy)
                .def_prop_rw("editable", &ColorDot::isEditable, &ColorDot::setEditable)
                .def("setCallback", &ColorDot::setCallback);
        }
    }
}
