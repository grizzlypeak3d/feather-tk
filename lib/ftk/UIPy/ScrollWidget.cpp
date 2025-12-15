// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ScrollWidget.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void scrollWidget(py::module_& m)
        {
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
                .def_property("scrollPos", &ScrollWidget::getScrollPos, &ScrollWidget::setScrollPos)
                .def("scrollTo", &ScrollWidget::scrollTo)
                .def("setScrollPosCallback", &ScrollWidget::setScrollPosCallback)
                .def_property("areaResizable", &ScrollWidget::isAreaResizable, &ScrollWidget::setAreaResizable)
                .def_property("scrollBarsVisible", &ScrollWidget::areScrollBarsVisible, &ScrollWidget::setScrollBarsVisible)
                .def_property("scrollBarsAutoHide", &ScrollWidget::getScrollBarsAutoHide, &ScrollWidget::setScrollBarsAutoHide)
                .def("setScrollEventsEnabled", &ScrollWidget::setScrollEventsEnabled)
                .def_property_readonly("viewport", &ScrollWidget::getViewport)
                .def_property_readonly("setViewportCallback", &ScrollWidget::setViewportCallback)
                .def_property("border", &ScrollWidget::hasBorder, &ScrollWidget::setBorder);
        }
    }
}
