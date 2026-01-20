// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/RowLayout.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void rowLayout(py::module_& m)
        {
            py::class_<RowMargins>(m, "RowMargins")
                .def(py::init<>())
                .def(py::init<SizeRole>())
                .def(
                    py::init<SizeRole, SizeRole>(),
                    py::arg("horizontal"),
                    py::arg("vertical"))
                .def(
                    py::init<SizeRole, SizeRole, SizeRole, SizeRole>(),
                    py::arg("left"),
                    py::arg("top"),
                    py::arg("right"),
                    py::arg("bottom"))
                .def_readwrite("left", &RowMargins::left)
                .def_readwrite("top", &RowMargins::top)
                .def_readwrite("right", &RowMargins::right)
                .def_readwrite("bottom", &RowMargins::bottom);

            py::class_<RowLayout, IWidget, std::shared_ptr<RowLayout> >(m, "RowLayout")
                .def(
                    py::init(&RowLayout::create),
                    py::arg("context"),
                    py::arg("orientation"),
                    py::arg("parent") = nullptr)
                .def_property("margins", &RowLayout::getMargins, &RowLayout::setMargins)
                .def_property("marginRole", &RowLayout::getMarginRole, &RowLayout::setMarginRole)
                .def_property("spacingRole", &RowLayout::getSpacingRole, &RowLayout::setSpacingRole)
                .def("addSpacer", py::overload_cast<Stretch>(&RowLayout::addSpacer), py::arg("stretch"))
                .def("addSpacer", py::overload_cast<SizeRole, Stretch>(&RowLayout::addSpacer), py::arg("spacingRole"), py::arg("stretch"))
                .def("clear", &RowLayout::clear);

            py::class_<VerticalLayout, RowLayout, std::shared_ptr<VerticalLayout> >(m, "VerticalLayout")
                .def(
                    py::init(&VerticalLayout::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr);

            py::class_<HorizontalLayout, RowLayout, std::shared_ptr<HorizontalLayout> >(m, "HorizontalLayout")
                .def(
                    py::init(&HorizontalLayout::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr);
        }
    }
}
