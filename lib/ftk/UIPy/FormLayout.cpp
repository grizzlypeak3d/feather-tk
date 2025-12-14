// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FormLayout.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void formLayout(py::module_& m)
        {
            py::class_<FormLayout, IWidget, std::shared_ptr<FormLayout> >(m, "FormLayout")
                .def(
                    py::init(&FormLayout::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def("addRow", &FormLayout::addRow, py::arg("text"), py::arg("widget"))
                .def("removeRow", py::overload_cast<int>(&FormLayout::removeRow))
                .def("removeRow", py::overload_cast<const std::shared_ptr<IWidget>&>(&FormLayout::removeRow))
                .def("clear", &FormLayout::clear)
                .def("setRowVisible", py::overload_cast<int, bool>(&FormLayout::setRowVisible))
                .def("setRowVisible", py::overload_cast<const std::shared_ptr<IWidget>&, bool>(&FormLayout::setRowVisible))
                .def_property("marginRole", &FormLayout::getMarginRole, &FormLayout::setMarginRole)
                .def_property("spacingRole", &FormLayout::getSpacingRole, &FormLayout::setSpacingRole)
                .def("addSpacer", py::overload_cast<>(&FormLayout::addSpacer))
                .def("addSpacer", py::overload_cast<SizeRole>(&FormLayout::addSpacer));
        }
    }
}
