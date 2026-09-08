// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IncButtons.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/function.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void incButtons(nb::module_& m)
        {
            nb::class_<IncButton, IButton>(m, "IncButton")
                .def(
                    nb::new_(&IncButton::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr);

            nb::class_<IncButtons, IWidget>(m, "IncButtons")
                .def(
                    nb::new_(&IncButtons::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def("setIncCallback", &IncButtons::setIncCallback)
                .def("setDecCallback", &IncButtons::setDecCallback);
        }
    }
}
