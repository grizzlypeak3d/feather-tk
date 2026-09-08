// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/SVGWidget.h>

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
        void svgWidget(nb::module_& m)
        {
            nb::class_<SvgWidget, IWidget>(m, "SvgWidget")
                .def(
                    nb::new_(&SvgWidget::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def_prop_rw("svgData",
                    &SvgWidget::getSVGData,
                    &SvgWidget::setSVGData)
                .def_prop_rw("svgWidth",
                    &SvgWidget::getSVGWidth,
                    &SvgWidget::setSVGWidth)
                .def_prop_rw("marginRole",
                    &SvgWidget::getMarginRole,
                    &SvgWidget::setMarginRole);
        }
    }
}
