// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/LineEditModel.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void lineEditModel(py::module_& m)
        {
            py::class_<LineEditSelection>(m, "LineEditSelection")
                .def(py::init<>())
                .def(py::init<int>(), py::arg("pos"))
                .def(py::init<int, int>(), py::arg("first"), py::arg("second"))
                .def_readwrite("first", &LineEditSelection::first)
                .def_readwrite("second", &LineEditSelection::second)
                .def("isValid", &LineEditSelection::isValid)
                .def("min", &LineEditSelection::min)
                .def("max", &LineEditSelection::max)
                .def("__eq__", &LineEditSelection::operator==)
                .def("__ne__", &LineEditSelection::operator!=);

            py::class_<LineEditModel, std::shared_ptr<LineEditModel> >(m, "LineEditModel")
                .def(
                    py::init(py::overload_cast<
                        const std::shared_ptr<Context>&,
                        const std::string&>(&LineEditModel::create)),
                    py::arg("context"),
                    py::arg("text") = std::string())
                .def_property("text", &LineEditModel::getText, &LineEditModel::setText)
                .def("observeText", &LineEditModel::observeText)
                .def("clearText", &LineEditModel::clearText)
                .def_property("readOnly", &LineEditModel::isReadOnly, &LineEditModel::setReadOnly)
                .def("observeReadOnly", &LineEditModel::observeReadOnly)
                .def_property("cursor", &LineEditModel::getCursor, &LineEditModel::setCursor)
                .def("observeCursor", &LineEditModel::observeCursor)
                .def_property("selection", &LineEditModel::getSelection, &LineEditModel::setSelection)
                .def("observeSelection", &LineEditModel::observeSelection)
                .def("selectAll", &LineEditModel::selectAll)
                .def("clearSelection", &LineEditModel::clearSelection)
                .def("undo", &LineEditModel::undo)
                .def("redo", &LineEditModel::redo)
                .def("cut", &LineEditModel::cut)
                .def("copy", &LineEditModel::copy)
                .def("paste", &LineEditModel::paste)
                .def("input", &LineEditModel::input, py::arg("text"))
                .def("key", &LineEditModel::key, py::arg("key"), py::arg("modifiers") = 0)
                .def_property("regex", &LineEditModel::getRegex, &LineEditModel::setRegex);
        }
    }
}
