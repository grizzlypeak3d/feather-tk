// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/RenderOptions.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void renderOptions(py::module_& m)
        {
            py::class_<RenderOptions>(m, "RenderOptions")
                .def(py::init<>())
                .def_readwrite("clear", &RenderOptions::clear)
                .def_readwrite("clearColor", &RenderOptions::clearColor)
                .def_readwrite("texturePoolByteCount", &RenderOptions::texturePoolByteCount)
                .def_readwrite("textureCacheByteCount", &RenderOptions::textureCacheByteCount)
                .def_readwrite("glyphAtlasSize", &RenderOptions::glyphAtlasSize)
                .def_readwrite("log", &RenderOptions::log)
                .def(py::self == py::self)
                .def(py::self != py::self);

            py::class_<LineOptions>(m, "LineOptions")
                .def(py::init<>())
                .def_readwrite("width", &LineOptions::width)
                .def(py::self == py::self)
                .def(py::self != py::self);

            py::enum_<ChannelDisplay>(m, "ChannelDisplay")
                .value("Color", ChannelDisplay::Color)
                .value("Red", ChannelDisplay::Red)
                .value("Green", ChannelDisplay::Green)
                .value("Blue", ChannelDisplay::Blue)
                .value("Alpha", ChannelDisplay::Alpha);
            FTK_ENUM_BIND(m, ChannelDisplay);

            py::enum_<InputVideoLevels>(m, "InputVideoLevels")
                .value("FromFile", InputVideoLevels::FromFile)
                .value("FullRange", InputVideoLevels::FullRange)
                .value("LegalRange", InputVideoLevels::LegalRange);
            FTK_ENUM_BIND(m, InputVideoLevels);

            py::enum_<AlphaBlend>(m, "AlphaBlend")
                .value("_None", AlphaBlend::None)
                .value("Straight", AlphaBlend::Straight)
                .value("Premultiplied", AlphaBlend::Premultiplied);
            FTK_ENUM_BIND(m, AlphaBlend);

            py::enum_<ImageFilter>(m, "ImageFilter")
                .value("Nearest", ImageFilter::Nearest)
                .value("Linear", ImageFilter::Linear)
                .value("HighQuality", ImageFilter::HighQuality);
            FTK_ENUM_BIND(m, ImageFilter);

            py::class_<ImageFilters>(m, "ImageFilters")
                .def(py::init<>())
                .def_readwrite("minify", &ImageFilters::minify)
                .def_readwrite("magnify", &ImageFilters::magnify)
                .def(py::self == py::self)
                .def(py::self != py::self);

            py::class_<ImageOptions>(m, "ImageOptions")
                .def(py::init<>())
                .def_readwrite("channelDisplay", &ImageOptions::channelDisplay)
                .def_readwrite("videoLevels", &ImageOptions::videoLevels)
                .def_readwrite("alphaBlend", &ImageOptions::alphaBlend)
                .def_readwrite("imageFilters", &ImageOptions::imageFilters)
                .def_readwrite("cache", &ImageOptions::cache)
                .def(py::self == py::self)
                .def(py::self != py::self);
        }
    }
}
