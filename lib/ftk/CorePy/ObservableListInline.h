// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/ObservableList.h>

namespace ftk
{
    namespace python
    {
        template<typename T>
        inline void observableList(pybind11::module_& m, const std::string& type)
        {
            std::string name = type + "ListObserver";
            pybind11::class_<ListObserver<T>, std::shared_ptr<ListObserver<T> > >(m, name.c_str())
                .def(
                    pybind11::init(&ListObserver<T>::create),
                    pybind11::arg("observableList"),
                    pybind11::arg("callback"),
                    pybind11::arg("action") = ObserverAction::Trigger);
            name = "IObservable" + type + "List";
            pybind11::class_<IObservableList<T>, std::shared_ptr<IObservableList<T> > >(m, name.c_str());
            name = "Observable" + type + "List";
            pybind11::class_<ObservableList<T>, IObservableList<T>, std::shared_ptr<ObservableList<T> > >(m, name.c_str())
                .def(pybind11::init(pybind11::overload_cast<const std::vector<T>&>(&ObservableList<T>::create)))
                .def("setAlways", &ObservableList<T>::setAlways)
                .def("setIfChanged", &ObservableList<T>::setIfChanged)
                .def("clear", &ObservableList<T>::clear)
                .def("setItem", &ObservableList<T>::setItem, pybind11::arg("index"), pybind11::arg("value"))
                .def("setItemOnlyIfChanged", &ObservableList<T>::setItemOnlyIfChanged, pybind11::arg("index"), pybind11::arg("value"))
                .def("pushBack", pybind11::overload_cast<const T&>(&ObservableList<T>::pushBack))
                .def("pushBack", pybind11::overload_cast<const std::vector<T>&>(&ObservableList<T>::pushBack))
                .def("insertItem", &ObservableList<T>::insertItem, pybind11::arg("index"), pybind11::arg("value"))
                .def("insertItems", &ObservableList<T>::insertItems, pybind11::arg("index"), pybind11::arg("list"))
                .def("removeItem", &ObservableList<T>::removeItem, pybind11::arg("index"))
                .def("removeItems", &ObservableList<T>::removeItems, pybind11::arg("start"), pybind11::arg("end"))
                .def("replaceItems", &ObservableList<T>::replaceItems, pybind11::arg("start"), pybind11::arg("end"), pybind11::arg("list"))
                .def("get", &ObservableList<T>::get)
                .def("getSize", &ObservableList<T>::getSize)
                .def("isEmpty", &ObservableList<T>::isEmpty)
                .def("getItem", &ObservableList<T>::getItem)
                .def("contains", &ObservableList<T>::contains)
                .def("indexOf", &ObservableList<T>::indexOf);
        }
    }
}
