// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Tooltip.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void tooltip(nb::module_& m)
        {
            nb::class_<Tooltip, IPopup>(m, "Tooltip")
                .def(
                    nb::new_(&Tooltip::create),
                    nb::arg("context"),
                    nb::arg("text"),
                    nb::arg("pos"),
                    nb::arg("window"));
        }
    }
}
