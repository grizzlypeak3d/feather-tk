// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/OverlayLayout.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void overlayLayout(nb::module_& m)
        {
            nb::class_<OverlayLayout, IWidget>(m, "OverlayLayout")
                .def(
                    nb::new_(&OverlayLayout::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("marginRole",
                    &OverlayLayout::getMarginRole,
                    &OverlayLayout::setMarginRole);
        }
    }
}
