// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/FontSystem.h>

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
        void fontSystem(nb::module_& m)
        {
            nb::enum_<FontType>(m, "FontType")
                .value("Regular", FontType::Regular)
                .value("Bold", FontType::Bold)
                .value("Mono", FontType::Mono)
                .value("Symbols", FontType::Symbols);
            FTK_ENUM_BIND(m, FontType);

            nb::class_<FontInfo>(m, "FontInfo")
                .def(nb::init<>())
                .def(nb::init<std::string, int>())
                .def_rw("name", &FontInfo::name)
                .def_rw("size", &FontInfo::size)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self)
                .def(nb::self < nb::self);

            nb::class_<FontMetrics>(m, "FontMetrics")
                .def_rw("ascender", &FontMetrics::ascender)
                .def_rw("descender", &FontMetrics::descender)
                .def_rw("lineHeight", &FontMetrics::lineHeight);

            nb::class_<GlyphInfo>(m, "GlyphInfo")
                .def(nb::init<>())
                .def(nb::init<uint32_t, const FontInfo&>())
                .def_rw("code", &GlyphInfo::code)
                .def_rw("fontInfo", &GlyphInfo::fontInfo)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self)
                .def(nb::self < nb::self);

            nb::class_<Glyph>(m, "Glyph")
                .def_rw("info", &Glyph::info)
                .def_rw("image", &Glyph::image)
                .def_rw("offset", &Glyph::offset)
                .def_rw("advance", &Glyph::advance)
                .def_rw("lsbDelta", &Glyph::lsbDelta)
                .def_rw("rsbDelta", &Glyph::rsbDelta);

            nb::class_<FontSystem, ISystem>(m, "FontSystem")
                .def(
                    nb::new_(&FontSystem::create),
                    nb::arg("context"))
                .def_prop_ro("fonts", &FontSystem::getFonts)
                .def(
                    "addFont",
                    nb::overload_cast<const std::string&, const std::string&>(&FontSystem::addFont),
                    nb::arg("name"),
                    nb::arg("fileName"))
                .def_prop_ro("glyphCacheSize", &FontSystem::getGlyphCacheSize)
                .def("observeGlyphCacheSize", &FontSystem::observeGlyphCacheSize)
                .def_prop_ro("glyphCachePercentage", &FontSystem::getGlyphCachePercentage)
                .def("getMetrics", &FontSystem::getMetrics, nb::arg("fontInfo"))
                .def(
                    "getSize",
                    &FontSystem::getSize,
                    nb::arg("text"),
                    nb::arg("fontInfo"),
                    nb::arg("maxLineWidth") = 0)
                .def(
                    "getBoxes",
                    &FontSystem::getBoxes,
                    nb::arg("text"),
                    nb::arg("fontInfo"),
                    nb::arg("maxLineWidth") = 0)
                .def(
                    "getGlyphs",
                    &FontSystem::getGlyphs,
                    nb::arg("text"),
                    nb::arg("fontInfo"));
        }
    }
}
