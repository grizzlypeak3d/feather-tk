// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Image.h>

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
        void image(nb::module_& m)
        {
            nb::enum_<ImageType>(m, "ImageType")
                .value("_None", ImageType::None)
                .value("L_U8", ImageType::L_U8)
                .value("L_U16", ImageType::L_U16)
                .value("L_U32", ImageType::L_U32)
                .value("L_F16", ImageType::L_F16)
                .value("L_F32", ImageType::L_F32)
                .value("LA_U8", ImageType::LA_U8)
                .value("LA_U16", ImageType::LA_U16)
                .value("LA_U32", ImageType::LA_U32)
                .value("LA_F16", ImageType::LA_F16)
                .value("LA_F32", ImageType::LA_F32)
                .value("RGB_U8", ImageType::RGB_U8)
                .value("RGB_U10", ImageType::RGB_U10)
                .value("RGB_U16", ImageType::RGB_U16)
                .value("RGB_U32", ImageType::RGB_U32)
                .value("RGB_F16", ImageType::RGB_F16)
                .value("RGB_F32", ImageType::RGB_F32)
                .value("RGBA_U8", ImageType::RGBA_U8)
                .value("RGBA_U16", ImageType::RGBA_U16)
                .value("RGBA_U32", ImageType::RGBA_U32)
                .value("RGBA_F16", ImageType::RGBA_F16)
                .value("RGBA_F32", ImageType::RGBA_F32)
                .value("YUV_420P_U8", ImageType::YUV_420P_U8)
                .value("YUV_422P_U8", ImageType::YUV_422P_U8)
                .value("YUV_444P_U8", ImageType::YUV_444P_U8)
                .value("YUV_420P_U16", ImageType::YUV_420P_U16)
                .value("YUV_422P_U16", ImageType::YUV_422P_U16)
                .value("YUV_444P_U16", ImageType::YUV_444P_U16);
            FTK_ENUM_BIND(m, ImageType);

            m.def("getChannelCount", &getChannelCount);
            m.def("getBitDepth", &getBitDepth);

            nb::enum_<VideoLevels>(m, "VideoLevels")
                .value("FullRange", VideoLevels::FullRange)
                .value("LegalRange", VideoLevels::LegalRange);
            FTK_ENUM_BIND(m, VideoLevels);

            nb::enum_<YUVCoefficients>(m, "YUVCoefficients")
                .value("REC709", YUVCoefficients::REC709)
                .value("BT2020", YUVCoefficients::BT2020);
            FTK_ENUM_BIND(m, YUVCoefficients);

            m.def("getYUVCoefficients", &getYUVCoefficients);

            nb::class_<ImageMirror>(m, "ImageMirror")
                .def(nb::init<>())
                .def(nb::init<bool, bool>())
                .def_rw("x", &ImageMirror::x)
                .def_rw("y", &ImageMirror::y)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self);

            nb::class_<ImageLayout>(m, "ImageLayout")
                .def(nb::init<>())
                .def(nb::init<const ImageMirror&, int, Endian>())
                .def_rw("mirror", &ImageLayout::mirror)
                .def_rw("alignment", &ImageLayout::alignment)
                .def_rw("endian", &ImageLayout::endian)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self);

            nb::class_<ImageInfo>(m, "ImageInfo")
                .def(nb::init<>())
                .def(nb::init<const Size2I&, ImageType>())
                .def(nb::init<int, int, ImageType>())
                .def_rw("name", &ImageInfo::name)
                .def_rw("size", &ImageInfo::size)
                .def_rw("type", &ImageInfo::type)
                .def_rw("pixelAspectRatio", &ImageInfo::pixelAspectRatio)
                .def_rw("videoLevels", &ImageInfo::videoLevels)
                .def_rw("yuvCoefficients", &ImageInfo::yuvCoefficients)
                .def_rw("layout", &ImageInfo::layout)
                .def_prop_ro("valid", &ImageInfo::isValid)
                .def_prop_ro("aspect", &ImageInfo::getAspect)
                .def_prop_ro("byteCount", &ImageInfo::getByteCount)
                .def(nb::self == nb::self)
                .def(nb::self != nb::self);


            nb::class_<Image>(m, "Image")
                .def(nb::new_(nb::overload_cast<const ImageInfo&>(&Image::create)))
                .def(nb::new_(nb::overload_cast<const Size2I&, ImageType>(&Image::create)))
                .def(nb::new_(nb::overload_cast<int, int, ImageType>(&Image::create)))
                .def_prop_ro("info", &Image::getInfo, nb::rv_policy::copy)
                .def_prop_ro("size", &Image::getSize, nb::rv_policy::copy)
                .def_prop_ro("width", &Image::getWidth)
                .def_prop_ro("height", &Image::getHeight)
                .def_prop_ro("aspect", &Image::getAspect)
                .def_prop_ro("type", &Image::getType)
                .def_prop_ro("valid", &Image::isValid)
                .def_prop_rw("tags", &Image::getTags, &Image::setTags, nb::rv_policy::copy)
                .def_prop_ro("byteCount", &Image::getByteCount)
                .def("zero", &Image::zero);
        }
    }
}
