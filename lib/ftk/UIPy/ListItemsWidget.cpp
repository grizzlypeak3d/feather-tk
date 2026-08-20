// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ListItemsWidget.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void listItemsWidget(py::module_& m)
        {
            py::class_<ListItem>(m, "ListItem")
                .def(py::init<>())
                .def(
                    py::init<const std::string&, const std::string&>(),
                    py::arg("text"),
                    py::arg("tooltip") = std::string())
                .def_readwrite("text", &ListItem::text)
                .def_readwrite("tooltip", &ListItem::tooltip)
                .def("__eq__", &ListItem::operator==)
                .def("__ne__", &ListItem::operator!=);

            py::class_<ListItemsWidget, IContainer, std::shared_ptr<ListItemsWidget> >(m, "ListItemsWidget")
                .def(
                    py::init(&ListItemsWidget::create),
                    py::arg("context"),
                    py::arg("type"),
                    py::arg("parent") = nullptr)
                .def("getItems", &ListItemsWidget::getItems)
                .def(
                    "setItems",
                    py::overload_cast<const std::vector<ListItem>&>(&ListItemsWidget::setItems))
                .def(
                    "setItems",
                    py::overload_cast<const std::vector<std::string>&>(&ListItemsWidget::setItems))
                .def("getChecked", &ListItemsWidget::getChecked, py::arg("index"))
                .def(
                    "setChecked",
                    &ListItemsWidget::setChecked,
                    py::arg("index"),
                    py::arg("checked"))
                .def("setCallback", &ListItemsWidget::setCallback)
                .def_property("current", &ListItemsWidget::getCurrent, &ListItemsWidget::setCurrent)
                .def("observeCurrent", &ListItemsWidget::observeCurrent)
                .def("observeScrollTo", &ListItemsWidget::observeScrollTo)
                .def_property("search", &ListItemsWidget::getSearch, &ListItemsWidget::setSearch)
                .def("clearSearch", &ListItemsWidget::clearSearch)
                .def("getRect", &ListItemsWidget::getRect, py::arg("index"));
        }
    }
}
