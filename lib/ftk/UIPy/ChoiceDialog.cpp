// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ChoiceDialog.h>
#include <ftk/UI/Window.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void choiceDialog(py::module_& m)
        {
            py::class_<ChoiceDialog, IDialog, std::shared_ptr<ChoiceDialog> >(m, "ChoiceDialog")
                .def(
                    py::init(&ChoiceDialog::create),
                    py::arg("context"),
                    py::arg("title"),
                    py::arg("text"),
                    py::arg("choices"),
                    py::arg("parent") = nullptr)
                .def("setCallback", &ChoiceDialog::setCallback);
        }
    }
}
