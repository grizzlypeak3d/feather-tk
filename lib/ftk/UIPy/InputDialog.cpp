// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/InputDialog.h>
#include <ftk/UI/Window.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void inputDialog(py::module_& m)
        {
            py::class_<InputDialog, IDialog, std::shared_ptr<InputDialog> >(m, "InputDialog")
                .def(
                    py::init(&InputDialog::create),
                    py::arg("context"),
                    py::arg("title"),
                    py::arg("text"),
                    py::arg("value"),
                    py::arg("confirm") = "OK",
                    py::arg("cancel") = "Cancel",
                    py::arg("parent") = nullptr)
                .def("setCallback", &InputDialog::setCallback);
        }
    }
}
