// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/ObservableValue.h>

namespace ftk
{
    namespace python
    {
        template<typename T>
        inline void observableValue(pybind11::module_& m, const std::string& type)
        {
            std::string name = "ValueObserver" + type;
            pybind11::class_<ValueObserver<T>, std::shared_ptr<ValueObserver<T> > >(m, name.c_str())
                .def(
                    pybind11::init(&ValueObserver<T>::create),
                    pybind11::arg("observableValue"),
                    pybind11::arg("callback"),
                    pybind11::arg("action") = ObserverAction::Trigger);
            name = "IObservableValue" + type;
            pybind11::class_<IObservableValue<T>, std::shared_ptr<IObservableValue<T> > >(m, name.c_str());
            name = "ObservableValue" + type;
            pybind11::class_<ObservableValue<T>, IObservableValue<T>, std::shared_ptr<ObservableValue<T> > >(m, name.c_str())
                .def(pybind11::init(pybind11::overload_cast<const T&>(&ObservableValue<T>::create)))
                .def("setAlways", &ObservableValue<T>::setAlways)
                .def("setIfChanged", &ObservableValue<T>::setIfChanged)
                .def("get", &ObservableValue<T>::get);
        }
    }
}
