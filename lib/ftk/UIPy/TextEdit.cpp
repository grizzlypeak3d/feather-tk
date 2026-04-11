// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ScrollWidget.h>
#include <ftk/UI/TextEdit.h>
#include <ftk/UI/TextEditModel.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void textEdit(py::module_& m)
        {
            py::class_<TextEditOptions>(m, "TextEditOptions")
                .def(py::init<>())
                .def_readwrite("fontInfo", &TextEditOptions::fontInfo)
                .def_readwrite("cursorBlink", &TextEditOptions::cursorBlink)
                .def_readwrite("autoScrollTimeout", &TextEditOptions::autoScrollTimeout)
                .def("__eq__", &TextEditOptions::operator==)
                .def("__ne__", &TextEditOptions::operator!=);

            py::class_<TextEdit, IWidget, std::shared_ptr<TextEdit> >(m, "TextEdit")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<IWidget>&>(&TextEdit::create)),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::shared_ptr<TextEditModel>&,
                        const std::shared_ptr<IWidget>&>(&TextEdit::create)),
                    py::arg("context"),
                    py::arg("model"),
                    py::arg("parent") = nullptr)
                .def("getModel", &TextEdit::getModel)
                .def("getScrollWidget", &TextEdit::getScrollWidget)
                .def_property("text", &TextEdit::getText, &TextEdit::setText)
                .def("clearText", &TextEdit::clearText)
                .def("setCallback", &TextEdit::setCallback)
                .def_property("readOnly", &TextEdit::isReadOnly, &TextEdit::setReadOnly)
                .def("selectAll", &TextEdit::selectAll)
                .def("clearSelection", &TextEdit::clearSelection)
                .def_property("options", &TextEdit::getOptions, &TextEdit::setOptions)
                .def("observeOptions", &TextEdit::observeOptions)
                .def_property("sizeHintRole",
                    &TextEdit::getSizeHintRole,
                    &TextEdit::setSizeHintRole)
                .def_property("marginRole",
                    &TextEdit::getMarginRole,
                    &TextEdit::setMarginRole);
        }
    }
}
