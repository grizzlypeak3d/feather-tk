// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/GridLayout.h>

#include <nanobind/nanobind.h>
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
        void gridLayout(nb::module_& m)
        {
            nb::class_<GridLayout, IWidget>(m, "GridLayout")
                .def(
                    nb::new_(&GridLayout::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def("getGridPos", &GridLayout::getGridPos)
                .def("getGridRow", &GridLayout::getGridRow)
                .def("getGridColumn", &GridLayout::getGridColumn)
                .def(
                    "setGridPos",
                    &GridLayout::setGridPos,
                    nb::arg("child"),
                    nb::arg("row"),
                    nb::arg("column"))
                .def_prop_rw("rowBackgroundRole",
                    nb::overload_cast<>(&GridLayout::getRowBackgroundRole, nb::const_),
                    nb::overload_cast<ColorRole>(&GridLayout::setRowBackgroundRole))
                .def(
                    "getRowBackgroundRole",
                    nb::overload_cast<int>(&GridLayout::getRowBackgroundRole, nb::const_),
                    nb::arg("row"))
                .def(
                    "setRowBackgroundRole",
                    nb::overload_cast<int, ColorRole>(&GridLayout::setRowBackgroundRole),
                    nb::arg("row"),
                    nb::arg("role"))
                .def_prop_rw("marginRole", &GridLayout::getMarginRole, &GridLayout::setMarginRole)
                .def_prop_rw("spacingRole",
                    &GridLayout::getSpacingRole,
                    nb::overload_cast<SizeRole>(&GridLayout::setSpacingRole))
                .def("setSpacingRole", nb::overload_cast<SizeRole, SizeRole>(&GridLayout::setSpacingRole))
                .def("clear", &GridLayout::clear);
        }
    }
}
