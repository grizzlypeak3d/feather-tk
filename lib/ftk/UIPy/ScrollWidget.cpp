// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ScrollWidget.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/operators.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void scrollWidget(nb::module_& m)
        {
            nb::class_<ScrollInfo>(m, "ScrollInfo")
                .def(nb::init<>())
                .def_rw("viewport", &ScrollInfo::viewport)
                .def_rw("scrollSize", &ScrollInfo::scrollSize)
                .def_rw("scrollPos", &ScrollInfo::scrollPos)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self);

            nb::class_<ScrollWidget, IWidget>(m, "ScrollWidget")
                .def(
                    nb::new_(&ScrollWidget::create),
                    nb::arg("context"),
                    nb::arg("type") = ScrollType::Both,
                    nb::arg("parent") = nullptr)
                .def_prop_rw("widget", &ScrollWidget::getWidget, &ScrollWidget::setWidget)
                .def_prop_rw("scrollType", &ScrollWidget::getScrollType, &ScrollWidget::setScrollType)
                .def_prop_ro("scrollSize", &ScrollWidget::getScrollSize, nb::rv_policy::copy)
                .def("setScrollSizeCallback", &ScrollWidget::setScrollSizeCallback)
                .def("setScrollPos", &ScrollWidget::setScrollPos, nb::arg("value"), nb::arg("clamp") = true)
                .def_prop_ro("scrollPos", &ScrollWidget::getScrollPos, nb::rv_policy::copy)
                .def("setScrollPosCallback", &ScrollWidget::setScrollPosCallback)
                .def("scrollTo", &ScrollWidget::scrollTo)
                .def_prop_ro("scrollInfo", &ScrollWidget::getScrollInfo)
                .def("setScrollInfoCallback", &ScrollWidget::setScrollInfoCallback)
                .def_prop_rw("areaResizable", &ScrollWidget::isAreaResizable, &ScrollWidget::setAreaResizable)
                .def_prop_rw("scrollBarsVisible", &ScrollWidget::areScrollBarsVisible, &ScrollWidget::setScrollBarsVisible)
                .def_prop_rw("scrollBarsAutoHide", &ScrollWidget::getScrollBarsAutoHide, &ScrollWidget::setScrollBarsAutoHide)
                .def_prop_rw("scrollEventsEnabled", &ScrollWidget::areScrollEventsEnabled, &ScrollWidget::setScrollEventsEnabled)
                .def_prop_rw("dragScroll", &ScrollWidget::hasDragScroll, &ScrollWidget::setDragScroll)
                .def_prop_rw("border", &ScrollWidget::hasBorder, &ScrollWidget::setBorder)
                .def_prop_rw("sizeHintRole", &ScrollWidget::getSizeHintRole, &ScrollWidget::setSizeHintRole)
                .def_prop_rw("marginRole", &ScrollWidget::getMarginRole, &ScrollWidget::setMarginRole)
                .def_prop_rw("viewportWidget", &ScrollWidget::getViewportWidget, &ScrollWidget::setViewportWidget);
        }
    }
}
