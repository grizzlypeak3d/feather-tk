// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/DocumentModel.h>

#include <ftk/CorePy/Bindings.h>
#include <ftk/Core/Context.h>

#include <nanobind/stl/function.h>
#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        namespace
        {
            // Lets Python classes derive their own document types; the
            // interface is only an identity, so there is nothing to
            // forward.
            class PyIDocument : public IDocument
            {
            public:
                virtual ~PyIDocument()
                {}
            };
        }

        void documentModel(nb::module_& m)
        {
            nb::class_<IDocument, PyIDocument>(m, "IDocument")
                .def(nb::init<>());

            // The add and close observables carry weak pointers, which
            // do not cross into Python; observing the list covers the
            // same ground.
            ftk::python::observable<std::shared_ptr<IDocument> >(m, "IDocument");
            ftk::python::observableList<std::shared_ptr<IDocument> >(m, "IDocument");

            nb::class_<DocumentModel>(m, "DocumentModel")
                .def(
                    nb::new_(&DocumentModel::create),
                    nb::arg("context"))
                .def("getList", &DocumentModel::getList)
                .def("observeList", &DocumentModel::observeList)
                .def("add", &DocumentModel::add, nb::arg("document"))
                .def("observeAdd", &DocumentModel::observeAdd)
                .def("close", &DocumentModel::close, nb::arg("index"))
                .def("closeAll", &DocumentModel::closeAll)
                .def("observeClose", &DocumentModel::observeClose)
                .def("observeCloseAll", &DocumentModel::observeCloseAll)
                .def("getCurrent", &DocumentModel::getCurrent)
                .def_prop_rw("currentIndex",
                    &DocumentModel::getCurrentIndex,
                    &DocumentModel::setCurrentIndex)
                .def("observeCurrent", &DocumentModel::observeCurrent)
                .def("observeCurrentIndex", &DocumentModel::observeCurrentIndex);
        }
    }
}
