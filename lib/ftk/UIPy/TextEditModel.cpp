// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/TextEditModel.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void textEditModel(py::module_& m)
        {
            py::class_<TextEditPos>(m, "TextEditPos")
                .def(py::init<>())
                .def(py::init<int, int>(), py::arg("line"), py::arg("chr"))
                .def_readwrite("line", &TextEditPos::line)
                .def_readwrite("chr", &TextEditPos::chr)
                .def("isValid", &TextEditPos::isValid)
                .def("__eq__", &TextEditPos::operator==)
                .def("__ne__", &TextEditPos::operator!=)
                .def("__lt__", &TextEditPos::operator<)
                .def("__gt__", &TextEditPos::operator>);

            py::class_<TextEditSelection>(m, "TextEditSelection")
                .def(py::init<>())
                .def(py::init<const TextEditPos&>(), py::arg("pos"))
                .def(py::init<const TextEditPos&, const TextEditPos&>(),
                    py::arg("first"), py::arg("second"))
                .def_readwrite("first", &TextEditSelection::first)
                .def_readwrite("second", &TextEditSelection::second)
                .def("isValid", &TextEditSelection::isValid)
                .def("min", &TextEditSelection::min)
                .def("max", &TextEditSelection::max)
                .def("__eq__", &TextEditSelection::operator==)
                .def("__ne__", &TextEditSelection::operator!=);

            py::class_<TextEditModelOptions>(m, "TextEditModelOptions")
                .def(py::init<>())
                .def_readwrite("tabSpaces", &TextEditModelOptions::tabSpaces)
                .def("__eq__", &TextEditModelOptions::operator==)
                .def("__ne__", &TextEditModelOptions::operator!=);

            py::class_<TextEditModel, std::shared_ptr<TextEditModel> >(m, "TextEditModel")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::vector<std::string>&>(&TextEditModel::create)),
                    py::arg("context"),
                    py::arg("text") = std::vector<std::string>{})
                .def_property("text", &TextEditModel::getText, &TextEditModel::setText)
                .def("observeText", &TextEditModel::observeText)
                .def("clearText", &TextEditModel::clearText)
                .def_property("readOnly", &TextEditModel::isReadOnly, &TextEditModel::setReadOnly)
                .def("observeReadOnly", &TextEditModel::observeReadOnly)
                .def_property("cursor", &TextEditModel::getCursor, &TextEditModel::setCursor)
                .def("observeCursor", &TextEditModel::observeCursor)
                .def_property("selection", &TextEditModel::getSelection, &TextEditModel::setSelection)
                .def("observeSelection", &TextEditModel::observeSelection)
                .def("selectAll", &TextEditModel::selectAll)
                .def("clearSelection", &TextEditModel::clearSelection)
                .def("undo", &TextEditModel::undo)
                .def("redo", &TextEditModel::redo)
                .def("cut", &TextEditModel::cut)
                .def("copy", &TextEditModel::copy)
                .def("paste", &TextEditModel::paste)
                .def("input", &TextEditModel::input, py::arg("text"))
                .def("key", &TextEditModel::key, py::arg("key"), py::arg("modifiers") = 0)
                .def_property("options", &TextEditModel::getOptions, &TextEditModel::setOptions)
                .def("observeOptions", &TextEditModel::observeOptions)
                .def("setPageRows", &TextEditModel::setPageRows, py::arg("rows"));
        }
    }
}
