// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Observable.h>

namespace ftk
{
    namespace python
    {
        template<typename T>
        struct IsSharedPtr : std::false_type {};
        template<typename T>
        struct IsSharedPtr<std::shared_ptr<T> > : std::true_type {};

        template<typename T>
        inline void observable(nanobind::module_& m, const std::string& type)
        {
            std::string name = type + "Observer";
            nanobind::class_<Observer<T>>(m, name.c_str())
                .def(
                    nanobind::new_(&Observer<T>::create),
                    nanobind::arg("observable"),
                    nanobind::arg("callback"),
                    nanobind::arg("action") = ObserverAction::Trigger);
            name = "IObservable" + type;
            nanobind::class_<IObservable<T>>(m, name.c_str())
                .def("get", &IObservable<T>::get)
                .def("getObserversCount", &IObservable<T>::getObserversCount);
            name = "Observable" + type;
            auto cls = nanobind::class_<Observable<T>, IObservable<T>>(m, name.c_str());
            // A shared_ptr observable holds "nothing yet" as a null pointer,
            // which Python spells None -- the argument has to allow it.
            if constexpr (IsSharedPtr<T>::value)
            {
                cls.def(
                    nanobind::new_(nanobind::overload_cast<const T&>(&Observable<T>::create)),
                    nanobind::arg("value").none());
                cls.def("setAlways", &Observable<T>::setAlways, nanobind::arg("value").none());
                cls.def("setIfChanged", &Observable<T>::setIfChanged, nanobind::arg("value").none());
            }
            else
            {
                cls.def(nanobind::new_(nanobind::overload_cast<const T&>(&Observable<T>::create)));
                cls.def("setAlways", &Observable<T>::setAlways);
                cls.def("setIfChanged", &Observable<T>::setIfChanged);
            }
            cls.def("get", &Observable<T>::get);
        }
    }
}
