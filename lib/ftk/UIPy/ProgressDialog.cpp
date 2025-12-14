// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ProgressDialog.h>
#include <ftk/UI/Window.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void progressDialog(py::module_& m)
        {
            py::class_<ProgressDialog, IDialog, std::shared_ptr<ProgressDialog> >(m, "ProgressDialog")
                .def(
                    py::init(&ProgressDialog::create),
                    py::arg("context"),
                    py::arg("title"),
                    py::arg("text"),
                    py::arg("parent") = nullptr)
                .def_property("text", &ProgressDialog::getText, &ProgressDialog::setText)
                .def_property("message", &ProgressDialog::getMessage, &ProgressDialog::setMessage)
                .def_property("range", &ProgressDialog::getRange, py::overload_cast<const RangeD&>(&ProgressDialog::setRange))
                .def("setRange", py::overload_cast<double, double>(&ProgressDialog::setRange))
                .def_property("value", &ProgressDialog::getValue, &ProgressDialog::setValue);
        }
    }
}
