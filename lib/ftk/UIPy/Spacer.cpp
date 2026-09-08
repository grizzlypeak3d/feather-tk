// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Spacer.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void spacer(nb::module_& m)
        {
            nb::class_<Spacer, IWidget>(m, "Spacer")
                .def(
                    nb::new_(&Spacer::create),
                    nb::arg("context"),
                    nb::arg("orientation"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("spacingRole",
                    &Spacer::getSpacingRole,
                    &Spacer::setSpacingRole);
        }
    }
}
