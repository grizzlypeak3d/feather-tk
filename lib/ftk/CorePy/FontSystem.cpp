// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/FontSystem.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void fontSystem(py::module_& m)
        {
            py::class_<FontInfo>(m, "FontInfo")
                .def(py::init<>())
                .def(py::init<const std::string&, int>())
                .def_readwrite("family", &FontInfo::family)
                .def_readwrite("size", &FontInfo::size)
                .def(py::self == py::self)
                .def(py::self != py::self)
                .def(py::self < py::self);

            py::class_<FontMetrics>(m, "FontMetrics")
                .def_readwrite("ascender", &FontMetrics::ascender)
                .def_readwrite("descender", &FontMetrics::descender)
                .def_readwrite("lineHeight", &FontMetrics::lineHeight);

            py::class_<GlyphInfo>(m, "GlyphInfo")
                .def(py::init<>())
                .def(py::init<uint32_t, const FontInfo&>())
                .def_readwrite("code", &GlyphInfo::code)
                .def_readwrite("fontInfo", &GlyphInfo::fontInfo)
                .def(py::self == py::self)
                .def(py::self != py::self)
                .def(py::self < py::self);

            py::class_<Glyph, std::shared_ptr<Glyph> >(m, "Glyph")
                .def_readwrite("info", &Glyph::info)
                .def_readwrite("image", &Glyph::image)
                .def_readwrite("offset", &Glyph::offset)
                .def_readwrite("advance", &Glyph::advance)
                .def_readwrite("lsbDelta", &Glyph::lsbDelta)
                .def_readwrite("rsbDelta", &Glyph::rsbDelta);

            py::class_<FontSystem, ISystem, std::shared_ptr<FontSystem> >(m, "FontSystem")
                .def(
                    py::init(&FontSystem::create),
                    py::arg("context"))
                .def_property_readonly("glyphCacheSize", &FontSystem::getGlyphCacheSize)
                .def_property_readonly("glyphCachePercentage", &FontSystem::getGlyphCachePercentage)
                .def("getMetrics", &FontSystem::getMetrics, py::arg("fontInfo"))
                .def(
                    "getSize",
                    &FontSystem::getSize,
                    py::arg("text"),
                    py::arg("fontInfo"),
                    py::arg("maxLineWidth") = 0)
                .def(
                    "getBoxes",
                    &FontSystem::getBoxes,
                    py::arg("text"),
                    py::arg("fontInfo"),
                    py::arg("maxLineWidth") = 0)
                .def(
                    "getGlyphs",
                    &FontSystem::getGlyphs,
                    py::arg("text"),
                    py::arg("fontInfo"));
        }
    }
}
