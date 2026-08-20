// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FlowLayout.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void flowLayout(py::module_& m)
        {
            py::class_<FlowLayout, IWidget, std::shared_ptr<FlowLayout> >(m, "FlowLayout")
                .def(
                    py::init(&FlowLayout::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("marginRole", &FlowLayout::getMarginRole, &FlowLayout::setMarginRole)
                .def_property("spacingRole", &FlowLayout::getSpacingRole, &FlowLayout::setSpacingRole);
        }
    }
}
