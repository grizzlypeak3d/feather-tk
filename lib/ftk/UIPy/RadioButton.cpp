// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/RadioButton.h>

#include <ftk/UI/RadioButton.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void radioButton(py::module_& m)
        {
            py::class_<RadioButton, IButton, std::shared_ptr<RadioButton> >(m, "RadioButton")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&RadioButton::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::string&,
                        const std::shared_ptr<IWidget>&>(&RadioButton::create)),
                    py::arg("context"),
                    py::arg("text"),
                    py::arg("parent") = nullptr);
        }
    }
}
