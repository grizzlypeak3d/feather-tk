// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ListWidget.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void listWidget(py::module_& m)
        {
            py::class_<ListWidget, IContainer, std::shared_ptr<ListWidget> >(m, "ListWidget")
                .def(
                    py::init(&ListWidget::create),
                    py::arg("context"),
                    py::arg("type"),
                    py::arg("parent") = nullptr)
                .def("getItems", &ListWidget::getItems)
                .def(
                    "setItems",
                    py::overload_cast<const std::vector<ListItem>&>(&ListWidget::setItems))
                .def(
                    "setItems",
                    py::overload_cast<const std::vector<std::string>&>(&ListWidget::setItems))
                .def("getChecked", &ListWidget::getChecked, py::arg("index"))
                .def(
                    "setChecked",
                    &ListWidget::setChecked,
                    py::arg("index"),
                    py::arg("checked") = true)
                .def("setCallback", &ListWidget::setCallback)
                .def_property("current", &ListWidget::getCurrent, &ListWidget::setCurrent)
                .def_property("search", &ListWidget::getSearch, &ListWidget::setSearch)
                .def("clearSearch", &ListWidget::clearSearch);
        }
    }
}
