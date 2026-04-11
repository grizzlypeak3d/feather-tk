// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/LineEdit.h>
#include <ftk/UI/LineEditModel.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void lineEdit(py::module_& m)
        {
            py::class_<LineEdit, IWidget, std::shared_ptr<LineEdit> >(m, "LineEdit")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&LineEdit::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<LineEditModel>&,
                        const std::shared_ptr<IWidget>&>(&LineEdit::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def("getModel", &LineEdit::getModel)
                .def_property("text", &LineEdit::getText, &LineEdit::setText)
                .def("clearText", &LineEdit::clearText)
                .def("setCallback", &LineEdit::setCallback)
                .def_property_readonly("hasCallbackOnFocusLost", &LineEdit::hasCallbackOnFocusLost)
                .def("setCallbackOnFocusLost", &LineEdit::setCallbackOnFocusLost)
                .def("setTextChangedCallback", &LineEdit::setTextChangedCallback)
                .def_property("format", &LineEdit::getFormat, &LineEdit::setFormat)
                .def("setFocusCallback", &LineEdit::setFocusCallback)
                .def_property("readOnly", &LineEdit::isReadOnly, &LineEdit::setReadOnly)
                .def("selectAll", &LineEdit::selectAll)
                .def("clearSelection", &LineEdit::clearSelection)
                .def_property("font", &LineEdit::getFont, &LineEdit::setFont)
                .def_property("borderRole", &LineEdit::getBorderRole, &LineEdit::setBorderRole);
        }
    }
}
