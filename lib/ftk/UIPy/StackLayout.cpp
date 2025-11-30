// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/StackLayout.h>

#include <ftk/UI/StackLayout.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void stackLayout(py::module_& m)
        {
            py::class_<StackLayout, IWidget, std::shared_ptr<StackLayout> >(m, "StackLayout")
                .def(
                    py::init(&StackLayout::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("currentIndex", &StackLayout::getCurrentIndex, &StackLayout::setCurrentIndex)
                .def("nextIndex", &StackLayout::nextIndex)
                .def_property_readonly("observeHasNextIndex", &StackLayout::observeHasNextIndex)
                .def("prevIndex", &StackLayout::prevIndex)
                .def_property_readonly("observeHasPrevIndex", &StackLayout::observeHasPrevIndex)
                .def("firstIndex", &StackLayout::firstIndex)
                .def("lastIndex", &StackLayout::lastIndex)
                .def("setCurrentWidget", &StackLayout::setCurrentWidget)
                .def_property("marginRole", &StackLayout::getMarginRole, &StackLayout::setMarginRole)
                .def("clear", &StackLayout::clear);
        }
    }
}
