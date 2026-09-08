// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ConfirmDialog.h>
#include <ftk/UI/Window.h>

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
        void confirmDialog(nb::module_& m)
        {
            nb::class_<ConfirmDialog, IDialog>(m, "ConfirmDialog")
                .def(
                    nb::new_(&ConfirmDialog::create),
                    nb::arg("context"),
                    nb::arg("title"),
                    nb::arg("text"),
                    nb::arg("confirm") = "OK",
                    nb::arg("cancel") = "Cancel",
                    nb::arg("parent") = nullptr)
                .def("setCallback", &ConfirmDialog::setCallback);
        }
    }
}
