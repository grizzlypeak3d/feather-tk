// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Image.h>

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void image(py::module_& m)
        {
            py::enum_<ImageType>(m, "ImageType")
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
                .value("YUV_444P_U16", ImageType::YUV_444P_U16)
                .value("ARGB_4444_Premult", ImageType::ARGB_4444_Premult);
            FTK_ENUM_BIND(m, ImageType);

            m.def("getChannelCount", &getChannelCount);
            m.def("getBitDepth", &getBitDepth);

            py::enum_<VideoLevels>(m, "VideoLevels")
                .value("FullRange", VideoLevels::FullRange)
                .value("LegalRange", VideoLevels::LegalRange);
            FTK_ENUM_BIND(m, VideoLevels);

            py::enum_<YUVCoefficients>(m, "YUVCoefficients")
                .value("REC709", YUVCoefficients::REC709)
                .value("BT2020", YUVCoefficients::BT2020);
            FTK_ENUM_BIND(m, YUVCoefficients);

            m.def("getYUVCoefficients", &getYUVCoefficients);

            py::class_<ImageMirror>(m, "ImageMirror")
                .def(py::init<>())
                .def(py::init<bool, bool>())
                .def_readwrite("x", &ImageMirror::x)
                .def_readwrite("y", &ImageMirror::y)
                .def(py::self == py::self)
                .def(py::self != py::self);

            py::class_<ImageLayout>(m, "ImageLayout")
                .def(py::init<>())
                .def(py::init<const ImageMirror&, int, Endian>())
                .def_readwrite("mirror", &ImageLayout::mirror)
                .def_readwrite("alignment", &ImageLayout::alignment)
                .def_readwrite("endian", &ImageLayout::endian)
                .def(py::self == py::self)
                .def(py::self != py::self);

            py::class_<ImageInfo>(m, "ImageInfo")
                .def(py::init<>())
                .def(py::init<const Size2I&, ImageType>())
                .def(py::init<int, int, ImageType>())
                .def_readwrite("name", &ImageInfo::name)
                .def_readwrite("size", &ImageInfo::size)
                .def_readwrite("type", &ImageInfo::type)
                .def_readwrite("pixelAspectRatio", &ImageInfo::pixelAspectRatio)
                .def_readwrite("videoLevels", &ImageInfo::videoLevels)
                .def_readwrite("yuvCoefficients", &ImageInfo::yuvCoefficients)
                .def_readwrite("layout", &ImageInfo::layout)
                .def_property_readonly("valid", &ImageInfo::isValid)
                .def_property_readonly("aspect", &ImageInfo::getAspect)
                .def_property_readonly("byteCount", &ImageInfo::getByteCount)
                .def(py::self == py::self)
                .def(py::self != py::self);

            py::class_<ImageTags>(m, "ImageTags");

            py::class_<Image, std::shared_ptr<Image> >(m, "Image")
                .def(py::init(py::overload_cast<const ImageInfo&>(&Image::create)))
                .def(py::init(py::overload_cast<const Size2I&, ImageType>(&Image::create)))
                .def(py::init(py::overload_cast<int, int, ImageType>(&Image::create)))
                .def_property_readonly("info", &Image::getInfo)
                .def_property_readonly("size", &Image::getSize)
                .def_property_readonly("width", &Image::getWidth)
                .def_property_readonly("height", &Image::getHeight)
                .def_property_readonly("aspect", &Image::getAspect)
                .def_property_readonly("type", &Image::getType)
                .def_property_readonly("valid", &Image::isValid)
                .def_property("tags", &Image::getTags, &Image::setTags)
                .def_property_readonly("byteCount", &Image::getByteCount)
                .def("zero", &Image::zero);
        }
    }
}
