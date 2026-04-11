// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/SVGWidget.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void svgWidget(py::module_& m)
        {
            py::class_<SvgWidget, IWidget, std::shared_ptr<SvgWidget> >(m, "SvgWidget")
                .def(
                    py::init(&SvgWidget::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def_property("svgData",
                    &SvgWidget::getSVGData,
                    &SvgWidget::setSVGData)
                .def_property("svgWidth",
                    &SvgWidget::getSVGWidth,
                    &SvgWidget::setSVGWidth)
                .def_property("marginRole",
                    &SvgWidget::getMarginRole,
                    &SvgWidget::setMarginRole);
        }
    }
}
