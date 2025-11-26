// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/IRender.h>

#include <ftk/Core/IRender.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void iRender(py::module_& m)
        {
            py::class_<RenderOptions>(m, "RenderOptions")
                .def_readwrite("clear", &RenderOptions::clear)
                .def_readwrite("clearColor", &RenderOptions::clearColor)
                .def_readwrite("textureCacheByteCount", &RenderOptions::textureCacheByteCount)
                .def_readwrite("log", &RenderOptions::log)
                .def(py::self == py::self)
                .def(py::self != py::self);

            py::class_<IRender, std::shared_ptr<IRender> >(m, "IRender")
                .def(
                    "begin",
                    &IRender::begin,
                    py::arg("size"),
                    py::arg("options"))
                .def("end", &IRender::end)
                .def_property("renderSize", &IRender::getRenderSize, &IRender::setRenderSize)
                .def_property("viewport", &IRender::getViewport, &IRender::setViewport)
                .def("clearViewport", &IRender::clearViewport)
                .def_property("clipRectEnabled", &IRender::getClipRectEnabled, &IRender::setClipRectEnabled)
                .def_property("clipRect", &IRender::getClipRect, &IRender::setClipRect)
                .def(
                    "drawRect",
                    py::overload_cast<const Box2F&, const Color4F&>(&IRender::drawRect),
                    py::arg("rect"),
                    py::arg("color") = Color4F(1.F, 1.F, 1.F, 1.F))
                .def(
                    "drawRect",
                    py::overload_cast<const Box2I&, const Color4F&>(&IRender::drawRect),
                    py::arg("rect"),
                    py::arg("color") = Color4F(1.F, 1.F, 1.F, 1.F))
                .def(
                    "drawText",
                    py::overload_cast<
                    const std::vector<std::shared_ptr<Glyph> >&,
                    const FontMetrics&,
                    const V2F&,
                    const Color4F&>(&IRender::drawText),
                    py::arg("glyphs"),
                    py::arg("fontMetrics"),
                    py::arg("position"),
                    py::arg("color") = Color4F(1.F, 1.F, 1.F, 1.F))
                .def(
                    "drawText",
                    py::overload_cast<
                    const std::vector<std::shared_ptr<Glyph> >&,
                    const FontMetrics&,
                    const V2I&,
                    const Color4F&>(&IRender::drawText),
                    py::arg("glyphs"),
                    py::arg("fontMetrics"),
                    py::arg("position"),
                    py::arg("color") = Color4F(1.F, 1.F, 1.F, 1.F));
        }
    }
}
