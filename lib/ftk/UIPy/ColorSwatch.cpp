// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ColorSwatch.h>

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
        void colorSwatch(nb::module_& m)
        {
            nb::class_<ColorSwatch, IWidget>(m, "ColorSwatch")
                .def(
                    nb::new_(&ColorSwatch::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("color", &ColorSwatch::getColor, &ColorSwatch::setColor, nb::rv_policy::copy)
                .def_prop_rw("border", &ColorSwatch::hasBorder, &ColorSwatch::setBorder)
                .def_prop_rw("editable", &ColorSwatch::isEditable, &ColorSwatch::setEditable)
                .def("setCallback", &ColorSwatch::setCallback)
                .def("setPressedCallback", &ColorSwatch::setPressedCallback)
                .def_prop_rw("sizeRole", &ColorSwatch::getSizeRole, &ColorSwatch::setSizeRole);
        }
    }
}
