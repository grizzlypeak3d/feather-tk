// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IncButtons.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void incButtons(py::module_& m)
        {
            py::class_<IncButton, IButton, std::shared_ptr<IncButton> >(m, "IncButton")
                .def(
                    py::init(&IncButton::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr);

            py::class_<IncButtons, IWidget, std::shared_ptr<IncButtons> >(m, "IncButtons")
                .def(
                    py::init(&IncButtons::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def("setIncCallback", &IncButtons::setIncCallback)
                .def("setDecCallback", &IncButtons::setDecCallback);
        }
    }
}
