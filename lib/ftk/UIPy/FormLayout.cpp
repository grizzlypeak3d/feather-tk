// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FormLayout.h>

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
        void formLayout(nb::module_& m)
        {
            nb::class_<FormLayout, IContainer>(m, "FormLayout")
                .def(
                    nb::new_(&FormLayout::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def("addRow", &FormLayout::addRow, nb::arg("text"), nb::arg("widget"))
                .def("removeRow", nb::overload_cast<int>(&FormLayout::removeRow))
                .def("removeRow", nb::overload_cast<const std::shared_ptr<IWidget>&>(&FormLayout::removeRow))
                .def("clear", &FormLayout::clear)
                .def("setRowVisible", nb::overload_cast<int, bool>(&FormLayout::setRowVisible))
                .def("setRowVisible", nb::overload_cast<const std::shared_ptr<IWidget>&, bool>(&FormLayout::setRowVisible))
                .def_prop_rw("marginRole", &FormLayout::getMarginRole, &FormLayout::setMarginRole)
                .def_prop_rw("spacingRole", &FormLayout::getSpacingRole, &FormLayout::setSpacingRole)
                .def("addSpacer", nb::overload_cast<>(&FormLayout::addSpacer))
                .def("addSpacer", nb::overload_cast<SizeRole>(&FormLayout::addSpacer));
        }
    }
}
