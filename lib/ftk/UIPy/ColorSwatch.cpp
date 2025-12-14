// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ColorSwatch.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void colorSwatch(py::module_& m)
        {
            py::class_<ColorSwatch, IWidget, std::shared_ptr<ColorSwatch> >(m, "ColorSwatch")
                .def(
                    py::init(&ColorSwatch::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("color", &ColorSwatch::getColor, &ColorSwatch::setColor)
                .def("setPressedCallback", &ColorSwatch::setPressedCallback)
                .def_property("editable", &ColorSwatch::isEditable, &ColorSwatch::setEditable)
                .def("setColorCallback", &ColorSwatch::setColorCallback)
                .def_property("sizeRole", &ColorSwatch::getSizeRole, &ColorSwatch::setSizeRole);
        }
    }
}
