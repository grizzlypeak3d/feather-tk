// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/DocumentModel.h>

#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void documentModel(py::module_& m)
        {
            py::class_<IDocument, std::shared_ptr<IDocument> >(m, "IDocument");

            py::class_<DocumentModel, std::shared_ptr<DocumentModel> >(m, "DocumentModel")
                .def(
                    py::init(&DocumentModel::create),
                    py::arg("context"))
                .def("getList", &DocumentModel::getList)
                .def("observeList", &DocumentModel::observeList)
                .def("add", &DocumentModel::add, py::arg("document"))
                .def("observeAdd", &DocumentModel::observeAdd)
                .def("close", &DocumentModel::close, py::arg("index"))
                .def("closeAll", &DocumentModel::closeAll)
                .def("observeClose", &DocumentModel::observeClose)
                .def("observeCloseAll", &DocumentModel::observeCloseAll)
                .def("getCurrent", &DocumentModel::getCurrent)
                .def_property("currentIndex",
                    &DocumentModel::getCurrentIndex,
                    &DocumentModel::setCurrentIndex)
                .def("observeCurrent", &DocumentModel::observeCurrent)
                .def("observeCurrentIndex", &DocumentModel::observeCurrentIndex);
        }
    }
}
