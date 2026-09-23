// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/DocumentModel.h>

#include <ftk/CorePy/Bindings.h>
#include <ftk/Core/Context.h>

#include <ftk/CorePy/Function.h>
#include <ftk/CorePy/WeakPtr.h>
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
            // The weak pointers the model hands out: what observes them
            // and what reads them, but not Observable<T>, which compares
            // values with == and a weak pointer has no ==. Named as
            // ftk::python::observable() would name them.
            nb::class_<IObservable<std::weak_ptr<IDocument> > >(m, "IObservableWeakIDocument")
                .def("get", &IObservable<std::weak_ptr<IDocument> >::get)
                .def("getObserversCount", &IObservable<std::weak_ptr<IDocument> >::getObserversCount);
            nb::class_<Observer<std::weak_ptr<IDocument> > >(m, "WeakIDocumentObserver")
                .def(
                    nb::new_(&Observer<std::weak_ptr<IDocument> >::create),
                    nb::arg("observable"),
                    nb::arg("callback"),
                    nb::arg("action") = ObserverAction::Trigger);
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
