// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ConfirmDialog.h>
#include <ftk/UI/DialogSystem.h>
#include <ftk/UI/ChoiceDialog.h>
#include <ftk/UI/InputDialog.h>
#include <ftk/UI/MessageDialog.h>
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
        void dialogSystem(nb::module_& m)
        {
            nb::class_<DialogSystem, ISystem>(m, "DialogSystem")
                .def(
                    nb::new_(&DialogSystem::create),
                    nb::arg("context"))
                .def("message",
                    &DialogSystem::message,
                    nb::arg("title"),
                    nb::arg("text"),
                    nb::arg("window"))
                .def("choice",
                    &DialogSystem::choice,
                    nb::arg("title"),
                    nb::arg("text"),
                    nb::arg("choices"),
                    nb::arg("window"),
                    nb::arg("callback"))
                .def("input",
                    &DialogSystem::input,
                    nb::arg("title"),
                    nb::arg("text"),
                    nb::arg("value"),
                    nb::arg("window"),
                    nb::arg("callback"))
                .def("confirm",
                    &DialogSystem::confirm,
                    nb::arg("title"),
                    nb::arg("text"),
                    nb::arg("window"),
                    nb::arg("callback"),
                    nb::arg("confirm") = "OK",
                    nb::arg("cancel") = "Cancel");
        }
    }
}
