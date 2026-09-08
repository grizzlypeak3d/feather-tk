// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ScrollBar.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/function.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void scrollBar(nb::module_& m)
        {
            nb::class_<ScrollBar, IWidget>(m, "ScrollBar")
                .def(
                    nb::new_(&ScrollBar::create),
                    nb::arg("context"),
                    nb::arg("orientation"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("scrollSize",
                    &ScrollBar::getScrollSize,
                    &ScrollBar::setScrollSize)
                .def_prop_rw("scrollPos",
                    &ScrollBar::getScrollPos,
                    &ScrollBar::setScrollPos)
                .def("setScrollPosCallback", &ScrollBar::setScrollPosCallback);
        }
    }
}
