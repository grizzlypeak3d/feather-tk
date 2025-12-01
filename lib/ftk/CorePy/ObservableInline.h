// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Observable.h>

namespace ftk
{
    namespace python
    {
        template<typename T>
        inline void observable(pybind11::module_& m, const std::string& type)
        {
            std::string name = type + "Observer";
            pybind11::class_<Observer<T>, std::shared_ptr<Observer<T> > >(m, name.c_str())
                .def(
                    pybind11::init(&Observer<T>::create),
                    pybind11::arg("observable"),
                    pybind11::arg("callback"),
                    pybind11::arg("action") = ObserverAction::Trigger);
            name = "IObservable" + type;
            pybind11::class_<IObservable<T>, std::shared_ptr<IObservable<T> > >(m, name.c_str());
            name = "Observable" + type;
            pybind11::class_<Observable<T>, IObservable<T>, std::shared_ptr<Observable<T> > >(m, name.c_str())
                .def(pybind11::init(pybind11::overload_cast<const T&>(&Observable<T>::create)))
                .def("setAlways", &Observable<T>::setAlways)
                .def("setIfChanged", &Observable<T>::setIfChanged)
                .def("get", &Observable<T>::get);
        }
    }
}
