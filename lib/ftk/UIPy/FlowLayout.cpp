// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/FlowLayout.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void flowLayout(nb::module_& m)
        {
            nb::class_<FlowLayout, IWidget>(m, "FlowLayout")
                .def(
                    nb::new_(&FlowLayout::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("marginRole", &FlowLayout::getMarginRole, &FlowLayout::setMarginRole)
                .def_prop_rw("spacingRole", &FlowLayout::getSpacingRole, &FlowLayout::setSpacingRole);
        }
    }
}
