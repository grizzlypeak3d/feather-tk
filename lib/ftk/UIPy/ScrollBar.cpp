// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/ScrollBar.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void scrollBar(py::module_& m)
        {
            py::class_<ScrollBar, IWidget, std::shared_ptr<ScrollBar> >(m, "ScrollBar")
                .def(
                    py::init(&ScrollBar::create),
                    py::arg("context"),
                    py::arg("orientation"),
                    py::arg("parent") = nullptr)
                .def_property("scrollSize",
                    &ScrollBar::getScrollSize,
                    &ScrollBar::setScrollSize)
                .def_property("scrollPos",
                    &ScrollBar::getScrollPos,
                    &ScrollBar::setScrollPos)
                .def("setScrollPosCallback", &ScrollBar::setScrollPosCallback);
        }
    }
}
