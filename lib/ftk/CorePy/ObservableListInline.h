// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/ObservableList.h>

namespace ftk
{
    namespace python
    {
        template<typename T>
        inline void observableList(nanobind::module_& m, const std::string& type)
        {
            std::string name = type + "ListObserver";
            nanobind::class_<ListObserver<T>>(m, name.c_str())
                .def(
                    nanobind::new_(&ListObserver<T>::create),
                    nanobind::arg("observableList"),
                    nanobind::arg("callback"),
                    nanobind::arg("action") = ObserverAction::Trigger);
            name = "IObservable" + type + "List";
            nanobind::class_<IObservableList<T>>(m, name.c_str());
            name = "Observable" + type + "List";
            nanobind::class_<ObservableList<T>, IObservableList<T>>(m, name.c_str())
                .def(nanobind::new_(nanobind::overload_cast<const std::vector<T>&>(&ObservableList<T>::create)))
                .def("setAlways", &ObservableList<T>::setAlways)
                .def("setIfChanged", &ObservableList<T>::setIfChanged)
                .def("clear", &ObservableList<T>::clear)
                .def("setItem", &ObservableList<T>::setItem, nanobind::arg("index"), nanobind::arg("value"))
                .def("setItemOnlyIfChanged", &ObservableList<T>::setItemOnlyIfChanged, nanobind::arg("index"), nanobind::arg("value"))
                .def("pushBack", nanobind::overload_cast<const T&>(&ObservableList<T>::pushBack))
                .def("pushBack", nanobind::overload_cast<const std::vector<T>&>(&ObservableList<T>::pushBack))
                .def("insertItem", &ObservableList<T>::insertItem, nanobind::arg("index"), nanobind::arg("value"))
                .def("insertItems", &ObservableList<T>::insertItems, nanobind::arg("index"), nanobind::arg("list"))
                .def("removeItem", &ObservableList<T>::removeItem, nanobind::arg("index"))
                .def("removeItems", &ObservableList<T>::removeItems, nanobind::arg("start"), nanobind::arg("end"))
                .def("replaceItems", &ObservableList<T>::replaceItems, nanobind::arg("start"), nanobind::arg("end"), nanobind::arg("list"))
                .def("get", &ObservableList<T>::get)
                .def("getSize", &ObservableList<T>::getSize)
                .def("isEmpty", &ObservableList<T>::isEmpty)
                .def("getItem", &ObservableList<T>::getItem)
                .def("contains", &ObservableList<T>::contains)
                .def("indexOf", &ObservableList<T>::indexOf);
        }
    }
}
