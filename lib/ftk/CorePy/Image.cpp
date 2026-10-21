// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Format.h>
#include <ftk/Core/Image.h>
#include <ftk/Core/Memory.h>

#include <nanobind/nanobind.h>
#include <nanobind/ndarray.h>
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
        namespace
        {
            //! The pixels in place: an array of (height, width, channels)
            //! over the image's own memory, which keeps the image alive for
            //! as long as the array is. Writing to it writes the image.
            //!
            //! As the image is seen, whatever its layout: row zero is the
            //! top and column zero the left. A mirrored image -- a reader
            //! that stores rows bottom up, as OpenGL does -- is walked
            //! backwards with negative strides rather than shown the wrong
            //! way up, which a script comparing pixels would not notice.
            //!
            //! Refused rather than approximated for what has no such shape:
            //! RGB_U10 packs three channels into 32 bits, the YUV types are
            //! planes of different sizes, and a texture image's pixels are
            //! on the GPU. An array that looked right for those would be
            //! wrong without saying so.
            template<typename... Ts>
            nb::ndarray<Ts...> getPixels(nb::handle self)
            {
                Image& image = nb::cast<Image&>(self);
                const ImageInfo& info = image.getInfo();
                if (!image.isValid() || !image.getData())
                {
                    throw std::runtime_error(
                        "The image has no pixels in memory: it is empty, or "
                        "its pixels are a texture.");
                }
                size_t bytes = 0;
                bool isFloat = false;
                switch (info.type)
                {
                case ImageType::L_U8:
                case ImageType::LA_U8:
                case ImageType::RGB_U8:
                case ImageType::RGBA_U8:
                    bytes = 1;
                    break;
                case ImageType::L_U16:
                case ImageType::LA_U16:
                case ImageType::RGB_U16:
                case ImageType::RGBA_U16:
                    bytes = 2;
                    break;
                case ImageType::L_U32:
                case ImageType::LA_U32:
                case ImageType::RGB_U32:
                case ImageType::RGBA_U32:
                    bytes = 4;
                    break;
                case ImageType::L_F16:
                case ImageType::LA_F16:
                case ImageType::RGB_F16:
                case ImageType::RGBA_F16:
                    bytes = 2;
                    isFloat = true;
                    break;
                case ImageType::L_F32:
                case ImageType::LA_F32:
                case ImageType::RGB_F32:
                case ImageType::RGBA_F32:
                    bytes = 4;
                    isFloat = true;
                    break;
                default:
                    throw std::runtime_error(Format(
                        "{0} images have no pixel array: their channels are "
                        "packed or in planes.").arg(to_string(info.type)));
                }
                if (bytes > 1 && info.layout.endian != getEndian())
                {
                    throw std::runtime_error(
                        "The image's bytes are in the other endian order.");
                }
                const size_t channels = getChannelCount(info.type);
                const size_t rowBytes = getAlignedByteCount(
                    info.size.w * channels * bytes,
                    info.layout.alignment);
                if (rowBytes % bytes != 0)
                {
                    throw std::runtime_error(
                        "The image's rows are not a whole number of channels apart.");
                }
                const size_t shape[3] =
                {
                    static_cast<size_t>(info.size.h),
                    static_cast<size_t>(info.size.w),
                    channels
                };
                // In elements, not bytes.
                int64_t strides[3] =
                {
                    static_cast<int64_t>(rowBytes / bytes),
                    static_cast<int64_t>(channels),
                    1
                };
                uint8_t* data = image.getData();
                if (info.layout.mirror.y && info.size.h > 0)
                {
                    data += (info.size.h - 1) * rowBytes;
                    strides[0] = -strides[0];
                }
                if (info.layout.mirror.x && info.size.w > 0)
                {
                    data += (info.size.w - 1) * channels * bytes;
                    strides[1] = -strides[1];
                }
                nb::dlpack::dtype dtype;
                dtype.code = static_cast<uint8_t>(isFloat ?
                    nb::dlpack::dtype_code::Float :
                    nb::dlpack::dtype_code::UInt);
                dtype.bits = static_cast<uint8_t>(bytes * 8);
                dtype.lanes = 1;
                return nb::ndarray<Ts...>(
                    data,
                    3,
                    shape,
                    self,
                    strides,
                    dtype);
            }
        }

        void image(nb::module_& m)
        {
            // Hand-written rather than FTK_ENUM_PY: the structured enumerator
            // names (L_U8, RGBA_F16) are the Python API, and the labels
            // ("L U8") would flatten them. The enum contiguity test catches
            // a value this list falls behind on.
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
                .value("YUV_444P_U16", ImageType::YUV_444P_U16)
                .value("YUV_420SP_U8", ImageType::YUV_420SP_U8)
                .value("YUV_420SP_U16", ImageType::YUV_420SP_U16);
            FTK_ENUM_BIND(m, ImageType);
            // A viewport's color buffer is observed as one.
            observable<ImageType>(m, "ImageType");

            m.def("getChannelCount", &getChannelCount);
            m.def("getBitDepth", &getBitDepth);

            FTK_ENUM_PY(m, VideoLevels);
            FTK_ENUM_BIND(m, VideoLevels);

            FTK_ENUM_PY(m, YUVCoefficients);
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
                .def("zero", &Image::zero)
                // A memoryview, so nothing beyond Python is needed to read
                // it, and anything taking the buffer protocol can.
                .def_prop_ro(
                    "data",
                    [](nb::handle self) { return getPixels<nb::memview>(self); },
                    "The pixels as a memoryview of (height, width, channels) "
                    "over the image's own memory, row zero at the top whatever "
                    "the layout's mirror. Writing to it writes the image.")
                // What numpy.asarray(image) asks for; numpy is only imported
                // when numpy is the one asking.
                .def(
                    "__array__",
                    [](nb::handle self, nb::handle dtype, nb::handle copy)
                    {
                        nb::object out = nb::cast(getPixels<nb::numpy>(self));
                        // As numpy's protocol asks: a dtype is converted to,
                        // which copies, and copy=False forbids the copy.
                        if (!dtype.is_none() && !out.attr("dtype").equal(dtype))
                        {
                            if (!copy.is_none() && !nb::cast<bool>(copy))
                            {
                                throw nb::value_error(
                                    "Converting the image's pixels to another "
                                    "dtype needs a copy.");
                            }
                            return out.attr("astype")(dtype);
                        }
                        if (!copy.is_none() && nb::cast<bool>(copy))
                        {
                            return out.attr("copy")();
                        }
                        return out;
                    },
                    nb::arg("dtype") = nb::none(),
                    nb::arg("copy") = nb::none());
        }
    }
}
