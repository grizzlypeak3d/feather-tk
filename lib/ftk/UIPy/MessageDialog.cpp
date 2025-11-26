// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/MessageDialog.h>

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
        void messageDialog(py::module_& m)
        {
            py::class_<MessageDialog, IDialog, std::shared_ptr<MessageDialog> >(m, "MessageDialog")
                .def(
                    py::init(&MessageDialog::create),
                    py::arg("context"),
                    py::arg("title"),
                    py::arg("text"),
                    py::arg("parent") = nullptr)
                .def("setCallback", &MessageDialog::setCallback);
        }
    }
}
