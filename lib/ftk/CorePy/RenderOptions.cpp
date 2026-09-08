// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/RenderOptions.h>

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
        void renderOptions(nb::module_& m)
        {
            nb::class_<RenderOptions>(m, "RenderOptions")
                .def(nb::init<>())
                .def_rw("clear", &RenderOptions::clear)
                .def_rw("clearColor", &RenderOptions::clearColor)
                .def_rw("texturePoolByteCount", &RenderOptions::texturePoolByteCount)
                .def_rw("textureCacheByteCount", &RenderOptions::textureCacheByteCount)
                .def_rw("glyphAtlasSize", &RenderOptions::glyphAtlasSize)
                .def_rw("log", &RenderOptions::log)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self);

            nb::class_<LineOptions>(m, "LineOptions")
                .def(nb::init<>())
                .def_rw("width", &LineOptions::width)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self);

            nb::enum_<ChannelDisplay>(m, "ChannelDisplay")
                .value("Color", ChannelDisplay::Color)
                .value("Red", ChannelDisplay::Red)
                .value("Green", ChannelDisplay::Green)
                .value("Blue", ChannelDisplay::Blue)
                .value("Alpha", ChannelDisplay::Alpha);
            FTK_ENUM_BIND(m, ChannelDisplay);

            nb::enum_<InputVideoLevels>(m, "InputVideoLevels")
                .value("FromFile", InputVideoLevels::FromFile)
                .value("FullRange", InputVideoLevels::FullRange)
                .value("LegalRange", InputVideoLevels::LegalRange);
            FTK_ENUM_BIND(m, InputVideoLevels);

            nb::enum_<AlphaBlend>(m, "AlphaBlend")
                .value("_None", AlphaBlend::None)
                .value("Straight", AlphaBlend::Straight)
                .value("Premultiplied", AlphaBlend::Premultiplied);
            FTK_ENUM_BIND(m, AlphaBlend);

            nb::enum_<ImageFilter>(m, "ImageFilter")
                .value("Nearest", ImageFilter::Nearest)
                .value("Linear", ImageFilter::Linear)
                .value("HighQuality", ImageFilter::HighQuality);
            FTK_ENUM_BIND(m, ImageFilter);

            nb::class_<ImageFilters>(m, "ImageFilters")
                .def(nb::init<>())
                .def_rw("minify", &ImageFilters::minify)
                .def_rw("magnify", &ImageFilters::magnify)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self);

            nb::class_<ImageOptions>(m, "ImageOptions")
                .def(nb::init<>())
                .def_rw("channelDisplay", &ImageOptions::channelDisplay)
                .def_rw("videoLevels", &ImageOptions::videoLevels)
                .def_rw("alphaBlend", &ImageOptions::alphaBlend)
                .def_rw("imageFilters", &ImageOptions::imageFilters)
                .def_rw("cache", &ImageOptions::cache)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self);
        }
    }
}
