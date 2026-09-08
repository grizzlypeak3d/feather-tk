// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/ScrollArea.h>

#include <nanobind/nanobind.h>
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
        void scrollArea(nb::module_& m)
        {
            nb::enum_<ScrollType>(m, "ScrollType")
                .value("Horizontal", ScrollType::Horizontal)
                .value("Vertical", ScrollType::Vertical)
                .value("Both", ScrollType::Both)
                .value("Menu", ScrollType::Menu);
            FTK_ENUM_BIND(m, ScrollType);

            nb::class_<ScrollArea, IWidget>(m, "ScrollArea")
                .def(
                    nb::new_(&ScrollArea::create),
                    nb::arg("context"),
                    nb::arg("type") = ScrollType::Both,
                    nb::arg("parent") = nullptr)
                .def_prop_rw("scrollType", &ScrollArea::getScrollType, &ScrollArea::setScrollType)
                .def_prop_ro("scrollSize", &ScrollArea::getScrollSize, nb::rv_policy::copy)
                .def("setScrollSizeCallback", &ScrollArea::setScrollSizeCallback)
                .def_prop_rw("scrollPos", &ScrollArea::getScrollPos, &ScrollArea::setScrollPos, nb::rv_policy::copy)
                .def("setScrollPosCallback", &ScrollArea::setScrollPosCallback)
                .def("scrollTo", &ScrollArea::scrollTo)
                .def_prop_rw("areaResizable", &ScrollArea::isAreaResizable, &ScrollArea::setAreaResizable)
                .def_prop_rw("sizeHintRole", &ScrollArea::getSizeHintRole, &ScrollArea::setSizeHintRole);
        }
    }
}
