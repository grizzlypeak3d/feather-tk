// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/IRender.h>

#include <nanobind/nanobind.h>
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
        void iRender(nb::module_& m)
        {
            nb::class_<IRender>(m, "IRender")
                .def(
                    "begin",
                    &IRender::begin,
                    nb::arg("size"),
                    nb::arg("options"))
                .def("end", &IRender::end)
                .def_prop_rw("renderSize", &IRender::getRenderSize, &IRender::setRenderSize)
                .def_prop_rw("viewport", &IRender::getViewport, &IRender::setViewport)
                .def("clearViewport", &IRender::clearViewport)
                .def_prop_rw("clipRectEnabled", &IRender::getClipRectEnabled, &IRender::setClipRectEnabled)
                .def_prop_rw("clipRect", &IRender::getClipRect, &IRender::setClipRect)
                .def(
                    "drawRect",
                    nb::overload_cast<const Box2F&, const Color4F&>(&IRender::drawRect),
                    nb::arg("rect"),
                    nb::arg("color") = Color4F(1.F, 1.F, 1.F, 1.F))
                .def(
                    "drawRect",
                    nb::overload_cast<const Box2I&, const Color4F&>(&IRender::drawRect),
                    nb::arg("rect"),
                    nb::arg("color") = Color4F(1.F, 1.F, 1.F, 1.F))
                .def(
                    "drawText",
                    nb::overload_cast<
                    const std::vector<std::shared_ptr<Glyph> >&,
                    const FontMetrics&,
                    const V2F&,
                    const Color4F&>(&IRender::drawText),
                    nb::arg("glyphs"),
                    nb::arg("fontMetrics"),
                    nb::arg("position"),
                    nb::arg("color") = Color4F(1.F, 1.F, 1.F, 1.F))
                .def(
                    "drawText",
                    nb::overload_cast<
                    const std::vector<std::shared_ptr<Glyph> >&,
                    const FontMetrics&,
                    const V2I&,
                    const Color4F&>(&IRender::drawText),
                    nb::arg("glyphs"),
                    nb::arg("fontMetrics"),
                    nb::arg("position"),
                    nb::arg("color") = Color4F(1.F, 1.F, 1.F, 1.F));
        }
    }
}
