// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ListItemsWidget.h>

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
        void listItemsWidget(nb::module_& m)
        {
            nb::class_<ListItem>(m, "ListItem")
                .def(nb::init<>())
                .def(
                    nb::init<const std::string&, const std::string&>(),
                    nb::arg("text"),
                    nb::arg("tooltip") = std::string())
                .def_rw("text", &ListItem::text)
                .def_rw("tooltip", &ListItem::tooltip)
                .def("__eq__", &ListItem::operator==)
                .def("__ne__", &ListItem::operator!=);

            nb::class_<ListItemsWidget, IContainer>(m, "ListItemsWidget")
                .def(
                    nb::new_(&ListItemsWidget::create),
                    nb::arg("context"),
                    nb::arg("type"),
                    nb::arg("parent") = nullptr)
                .def("getItems", &ListItemsWidget::getItems)
                .def(
                    "setItems",
                    nb::overload_cast<const std::vector<ListItem>&>(&ListItemsWidget::setItems))
                .def(
                    "setItems",
                    nb::overload_cast<const std::vector<std::string>&>(&ListItemsWidget::setItems))
                .def("getChecked", &ListItemsWidget::getChecked, nb::arg("index"))
                .def(
                    "setChecked",
                    &ListItemsWidget::setChecked,
                    nb::arg("index"),
                    nb::arg("checked"))
                .def("setCallback", &ListItemsWidget::setCallback)
                .def_prop_rw("current", &ListItemsWidget::getCurrent, &ListItemsWidget::setCurrent)
                .def("observeCurrent", &ListItemsWidget::observeCurrent)
                .def("observeScrollTo", &ListItemsWidget::observeScrollTo)
                .def_prop_rw("search", &ListItemsWidget::getSearch, &ListItemsWidget::setSearch)
                .def("clearSearch", &ListItemsWidget::clearSearch)
                .def("getRect", &ListItemsWidget::getRect, nb::arg("index"));
        }
    }
}
