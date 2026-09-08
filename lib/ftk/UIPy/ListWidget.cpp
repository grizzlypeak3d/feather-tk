// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ListWidget.h>

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
        void listWidget(nb::module_& m)
        {
            nb::class_<ListWidget, IContainer>(m, "ListWidget")
                .def(
                    nb::new_(&ListWidget::create),
                    nb::arg("context"),
                    nb::arg("type"),
                    nb::arg("parent") = nullptr)
                .def("getItems", &ListWidget::getItems)
                .def(
                    "setItems",
                    nb::overload_cast<const std::vector<ListItem>&>(&ListWidget::setItems))
                .def(
                    "setItems",
                    nb::overload_cast<const std::vector<std::string>&>(&ListWidget::setItems))
                .def("getChecked", &ListWidget::getChecked, nb::arg("index"))
                .def(
                    "setChecked",
                    &ListWidget::setChecked,
                    nb::arg("index"),
                    nb::arg("checked") = true)
                .def("setCallback", &ListWidget::setCallback)
                .def_prop_rw("current", &ListWidget::getCurrent, &ListWidget::setCurrent)
                .def_prop_rw("search", &ListWidget::getSearch, &ListWidget::setSearch)
                .def("clearSearch", &ListWidget::clearSearch);
        }
    }
}
