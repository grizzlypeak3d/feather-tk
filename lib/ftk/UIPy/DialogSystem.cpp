// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ConfirmDialog.h>
#include <ftk/UI/DialogSystem.h>
#include <ftk/UI/MessageDialog.h>
#include <ftk/UI/Window.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void dialogSystem(py::module_& m)
        {
            py::class_<DialogSystem, ISystem, std::shared_ptr<DialogSystem> >(m, "DialogSystem")
                .def(
                    py::init(&DialogSystem::create),
                    py::arg("context"))
                .def("message",
                    &DialogSystem::message,
                    py::arg("title"),
                    py::arg("text"),
                    py::arg("window"))
                .def("confirm",
                    &DialogSystem::confirm,
                    py::arg("title"),
                    py::arg("text"),
                    py::arg("window"),
                    py::arg("callback"),
                    py::arg("confirm") = "OK",
                    py::arg("cancel") = "Cancel");
        }
    }
}
