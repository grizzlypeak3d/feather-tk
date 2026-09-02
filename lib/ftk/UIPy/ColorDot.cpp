// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ColorDot.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void colorDot(py::module_& m)
        {
            py::class_<ColorDot, IWidget, std::shared_ptr<ColorDot> >(m, "ColorDot")
                .def(
                    py::init(&ColorDot::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("color", &ColorDot::getColor, &ColorDot::setColor, py::return_value_policy::copy)
                .def_property("editable", &ColorDot::isEditable, &ColorDot::setEditable)
                .def("setCallback", &ColorDot::setCallback);
        }
    }
}
