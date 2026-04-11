// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/OverlayLayout.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void overlayLayout(py::module_& m)
        {
            py::class_<OverlayLayout, IWidget, std::shared_ptr<OverlayLayout> >(m, "OverlayLayout")
                .def(
                    py::init(&OverlayLayout::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("marginRole",
                    &OverlayLayout::getMarginRole,
                    &OverlayLayout::setMarginRole);
        }
    }
}
