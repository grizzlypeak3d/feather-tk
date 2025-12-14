// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/ScrollArea.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void scrollArea(py::module_& m)
        {
            py::enum_<ScrollType>(m, "ScrollType")
                .value("Horizontal", ScrollType::Horizontal)
                .value("Vertical", ScrollType::Vertical)
                .value("Both", ScrollType::Both)
                .value("Menu", ScrollType::Menu);
            FTK_ENUM_BIND(m, ScrollType);

            py::class_<ScrollArea, IWidget, std::shared_ptr<ScrollArea> >(m, "ScrollArea")
                .def(
                    py::init(&ScrollArea::create),
                    py::arg("context"),
                    py::arg("type") = ScrollType::Both,
                    py::arg("parent") = nullptr)
                .def_property("scrollType", &ScrollArea::getScrollType, &ScrollArea::setScrollType)
                .def_property_readonly("scrollSize", &ScrollArea::getScrollSize)
                .def("setScrollSizeCallback", &ScrollArea::setScrollSizeCallback)
                .def_property("scrollPos", &ScrollArea::getScrollPos, &ScrollArea::setScrollPos)
                .def("setScrollPosCallback", &ScrollArea::setScrollPosCallback)
                .def("scrollTo", &ScrollArea::scrollTo)
                .def_property("areaResizable", &ScrollArea::isAreaResizable, &ScrollArea::setAreaResizable)
                .def_property("sizeHintRole", &ScrollArea::getSizeHintRole, &ScrollArea::setSizeHintRole);
        }
    }
}
