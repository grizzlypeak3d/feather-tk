// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/RowLayout.h>

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
        void rowLayout(nb::module_& m)
        {
            nb::class_<RowMargins>(m, "RowMargins")
                .def(nb::init<>())
                .def(nb::init<SizeRole>())
                .def(
                    nb::init<SizeRole, SizeRole>(),
                    nb::arg("horizontal"),
                    nb::arg("vertical"))
                .def(
                    nb::init<SizeRole, SizeRole, SizeRole, SizeRole>(),
                    nb::arg("left"),
                    nb::arg("top"),
                    nb::arg("right"),
                    nb::arg("bottom"))
                .def_rw("left", &RowMargins::left)
                .def_rw("top", &RowMargins::top)
                .def_rw("right", &RowMargins::right)
                .def_rw("bottom", &RowMargins::bottom);

            nb::class_<RowLayout, IWidget>(m, "RowLayout")
                .def(
                    nb::new_(&RowLayout::create),
                    nb::arg("context"),
                    nb::arg("orientation"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("margins", &RowLayout::getMargins, &RowLayout::setMargins, nb::rv_policy::copy)
                .def_prop_rw("marginRole", &RowLayout::getMarginRole, &RowLayout::setMarginRole)
                .def_prop_rw("spacingRole", &RowLayout::getSpacingRole, &RowLayout::setSpacingRole)
                .def("addSpacer", nb::overload_cast<Stretch>(&RowLayout::addSpacer), nb::arg("stretch"))
                .def("addSpacer", nb::overload_cast<SizeRole, Stretch>(&RowLayout::addSpacer), nb::arg("spacingRole"), nb::arg("stretch"))
                .def("clear", &RowLayout::clear);

            nb::class_<VerticalLayout, RowLayout>(m, "VerticalLayout")
                .def(
                    nb::new_(&VerticalLayout::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr);

            nb::class_<HorizontalLayout, RowLayout>(m, "HorizontalLayout")
                .def(
                    nb::new_(&HorizontalLayout::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr);
        }
    }
}
