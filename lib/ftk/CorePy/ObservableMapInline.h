// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/ObservableMap.h>

namespace ftk
{
    namespace python
    {
        template<typename T, typename U>
        inline void observableMap(pybind11::module_& m, const std::string& type)
        {
            std::string name = "MapObserver" + type;
            pybind11::class_<MapObserver<T, U>, std::shared_ptr<MapObserver<T, U> > >(m, name.c_str())
                .def(
                    pybind11::init(&MapObserver<T, U>::create),
                    pybind11::arg("observableMap"),
                    pybind11::arg("callback"),
                    pybind11::arg("action") = ObserverAction::Trigger);
            name = "IObservableMap" + type;
            pybind11::class_<IObservableMap<T, U>, std::shared_ptr<IObservableMap<T, U> > >(m, name.c_str());
            name = "ObservableMap" + type;
            pybind11::class_<ObservableMap<T, U>, IObservableMap<T, U>, std::shared_ptr<ObservableMap<T, U> > >(m, name.c_str())
                .def(pybind11::init(pybind11::overload_cast<const std::map<T, U>&>(&ObservableMap<T, U>::create)))
                .def("setAlways", &ObservableMap<T, U>::setAlways)
                .def("setIfChanged", &ObservableMap<T, U>::setIfChanged)
                .def("get", &ObservableMap<T, U>::get);
        }
    }
}
