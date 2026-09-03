// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/ItemButton.h>
#include <ftk/UI/ItemButtonList.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void itemButton(py::module_& m)
        {
            py::class_<ItemButton, IButton, std::shared_ptr<ItemButton> >(m, "ItemButton")
                .def(
                    py::init(&ItemButton::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("widget", &ItemButton::getWidget, &ItemButton::setWidget)
                .def("setFocusCallback", &ItemButton::setFocusCallback)
                .def("setDragDropDataCallback", &ItemButton::setDragDropDataCallback)
                .def("setDragDropCursorCallback", &ItemButton::setDragDropCursorCallback);

            py::class_<ItemControls, IMouseWidget, std::shared_ptr<ItemControls> >(m, "ItemControls")
                .def(
                    py::init(&ItemControls::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("widget", &ItemControls::getWidget, &ItemControls::setWidget);
        }

        void itemButtonList(py::module_& m)
        {
            py::class_<ItemButtonList, VerticalLayout, std::shared_ptr<ItemButtonList> >(m, "ItemButtonList")
                .def(
                    py::init(&ItemButtonList::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("current", &ItemButtonList::getCurrent, &ItemButtonList::setCurrent)
                .def("setCurrentCallback", &ItemButtonList::setCurrentCallback)
                .def("setActivateCallback", &ItemButtonList::setActivateCallback)
                .def("setDeleteCallback", &ItemButtonList::setDeleteCallback);
        }
    }
}
