// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ScrollWidget.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void scrollWidget(py::module_& m)
        {
            py::class_<ScrollInfo>(m, "ScrollInfo")
                .def(py::init<>())
                .def_readwrite("viewport", &ScrollInfo::viewport)
                .def_readwrite("scrollSize", &ScrollInfo::scrollSize)
                .def_readwrite("scrollPos", &ScrollInfo::scrollPos)
                .def(py::self == py::self)
                .def(py::self != py::self);

            py::class_<ScrollWidget, IWidget, std::shared_ptr<ScrollWidget> >(m, "ScrollWidget")
                .def(
                    py::init(&ScrollWidget::create),
                    py::arg("context"),
                    py::arg("type") = ScrollType::Both,
                    py::arg("parent") = nullptr)
                .def_property("widget", &ScrollWidget::getWidget, &ScrollWidget::setWidget)
                .def_property("scrollType", &ScrollWidget::getScrollType, &ScrollWidget::setScrollType)
                .def_property_readonly("scrollSize", &ScrollWidget::getScrollSize)
                .def("setScrollSizeCallback", &ScrollWidget::setScrollSizeCallback)
                .def("setScrollPos", &ScrollWidget::setScrollPos, py::arg("value"), py::arg("clamp") = true)
                .def_property_readonly("scrollPos", &ScrollWidget::getScrollPos)
                .def("setScrollPosCallback", &ScrollWidget::setScrollPosCallback)
                .def("scrollTo", &ScrollWidget::scrollTo)
                .def_property_readonly("scrollInfo", &ScrollWidget::getScrollInfo)
                .def("setScrollInfoCallback", &ScrollWidget::setScrollInfoCallback)
                .def_property("areaResizable", &ScrollWidget::isAreaResizable, &ScrollWidget::setAreaResizable)
                .def_property("scrollBarsVisible", &ScrollWidget::areScrollBarsVisible, &ScrollWidget::setScrollBarsVisible)
                .def_property("scrollBarsAutoHide", &ScrollWidget::getScrollBarsAutoHide, &ScrollWidget::setScrollBarsAutoHide)
                .def_property("scrollEventsEnabled", &ScrollWidget::areScrollEventsEnabled, &ScrollWidget::setScrollEventsEnabled)
                .def_property("border", &ScrollWidget::hasBorder, &ScrollWidget::setBorder)
                .def_property("sizeHintRole", &ScrollWidget::getSizeHintRole, &ScrollWidget::setSizeHintRole)
                .def_property("marginRole", &ScrollWidget::getMarginRole, &ScrollWidget::setMarginRole)
                .def_property("viewportWidget", &ScrollWidget::getViewportWidget, &ScrollWidget::setViewportWidget);
        }
    }
}
