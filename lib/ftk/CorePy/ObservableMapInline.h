// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/ObservableMap.h>

namespace ftk
{
    namespace python
    {
        template<typename T, typename U>
        inline void observableMap(nanobind::module_& m, const std::string& type)
        {
            std::string name = type + "MapObserver";
            nanobind::class_<MapObserver<T, U>>(m, name.c_str())
                .def(
                    nanobind::new_(&MapObserver<T, U>::create),
                    nanobind::arg("observableMap"),
                    nanobind::arg("callback"),
                    nanobind::arg("action") = ObserverAction::Trigger);
            name = "IObservable" + type + "Map";
            nanobind::class_<IObservableMap<T, U>>(m, name.c_str());
            name = "Observable" + type + "Map";
            nanobind::class_<ObservableMap<T, U>, IObservableMap<T, U>>(m, name.c_str())
                .def(nanobind::new_(nanobind::overload_cast<const std::map<T, U>&>(&ObservableMap<T, U>::create)))
                .def("setAlways", &ObservableMap<T, U>::setAlways)
                .def("setIfChanged", &ObservableMap<T, U>::setIfChanged)
                .def("get", &ObservableMap<T, U>::get);
        }
    }
}
