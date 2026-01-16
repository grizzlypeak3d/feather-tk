// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Image.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/String.h>

#include <atomic>
#include <array>
#include <cstring>
#include <sstream>

namespace ftk
{
    FTK_ENUM_IMPL(
        ImageType,
        "None",
        "L U8",
        "L U16",
        "L U32",
        "L F16",
        "L F32",
        "LA U8",
        "LA U16",
        "LA U32",
        "LA F16",
        "LA F32",
        "RGB U8",
        "RGB U10",
        "RGB U16",
        "RGB U32",
        "RGB F16",
        "RGB F32",
        "RGBA U8",
        "RGBA U16",
        "RGBA U32",
        "RGBA F16",
        "RGBA F32",
        "YUV 420P U8",
        "YUV 422P U8",
        "YUV 444P U8",
        "YUV 420P U16",
        "YUV 422P U16",
        "YUV 444P U16",
        "ARGB 4444 Premult");

    int getChannelCount(ImageType value)
    {
        const std::array<int, static_cast<size_t>(ImageType::Count)> values =
        {
            0,
            1, 1, 1, 1, 1,
            2, 2, 2, 2, 2,
            3, 3, 3, 3, 3, 3,
            4, 4, 4, 4, 4,
            3, 3, 3,
            3, 3, 3,
            4
        };
        return values[static_cast<size_t>(value)];
    }

    int getBitDepth(ImageType value)
    {
        const std::array<int, static_cast<size_t>(ImageType::Count)> values =
        {
            0,
            8, 16, 32, 16, 32,
            8, 16, 32, 16, 32,
            8, 10, 16, 32, 16, 32,
            8, 16, 32, 16, 32,
            8, 8, 8,
            16, 16, 16,
            4
        };
        return values[static_cast<size_t>(value)];
    }

    FTK_ENUM_IMPL(
        YUVCoefficients,
        "REC709",
        "BT2020");

    V4F getYUVCoefficients(YUVCoefficients value)
    {
        //! References:
        //! * https://www.itu.int/rec/R-REC-BT.709
        //! * https://www.itu.int/rec/R-REC-BT.2020
        //! * https://gist.github.com/yohhoy/dafa5a47dade85d8b40625261af3776a
        //!
        //!     Y  = a * R + b * G + c * B
        //!     Cb = (B - Y) / d
        //!     Cr = (R - Y) / e
        //!
        //!     R = Y + e * Cr
        //!     G = Y - (a * e / b) * Cr - (c * d / b) * Cb
        //!     B = Y + d * Cb
        //!
        //!        BT.601   BT.709   BT.2020
        //!     ----------------------------
        //!     a  0.299    0.2126   0.2627
        //!     b  0.587    0.7152   0.6780
        //!     c  0.114    0.0722   0.0593
        //!     d  1.772    1.8556   1.8814
        //!     e  1.402    1.5748   1.4746
        //!
        const std::array<V4F, static_cast<size_t>(YUVCoefficients::Count)> data =
        {
            V4F(1.5748, 0.468124273, 0.187324273, 1.8556),
            V4F(1.4746, 0.571353127, 0.164553127, 1.8814)
        };
        return data[static_cast<size_t>(value)];
    }

    FTK_ENUM_IMPL(
        VideoLevels,
        "Full Range",
        "Legal Range");

    size_t ImageInfo::getByteCount() const
    {
        std::size_t out = 0;
        const size_t w = size.w;
        const size_t h = size.h;
        const size_t alignment = layout.alignment;
        switch (type)
        {
        case ImageType::L_U8:     out = getAlignedByteCount(w, alignment) * h; break;
        case ImageType::L_U16:    out = getAlignedByteCount(w * 2, alignment) * h; break;
        case ImageType::L_U32:    out = getAlignedByteCount(w * 4, alignment) * h; break;
        case ImageType::L_F16:    out = getAlignedByteCount(w * 2, alignment) * h; break;
        case ImageType::L_F32:    out = getAlignedByteCount(w * 4, alignment) * h; break;

        case ImageType::LA_U8:    out = getAlignedByteCount(w * 2, alignment) * h; break;
        case ImageType::LA_U16:   out = getAlignedByteCount(w * 2 * 2, alignment) * h; break;
        case ImageType::LA_U32:   out = getAlignedByteCount(w * 2 * 4, alignment) * h; break;
        case ImageType::LA_F16:   out = getAlignedByteCount(w * 2 * 2, alignment) * h; break;
        case ImageType::LA_F32:   out = getAlignedByteCount(w * 2 * 4, alignment) * h; break;

        case ImageType::RGB_U8:   out = getAlignedByteCount(w * 3, alignment) * h; break;
        case ImageType::RGB_U10:  out = getAlignedByteCount(w * 4, alignment) * h; break;
        case ImageType::RGB_U16:  out = getAlignedByteCount(w * 3 * 2, alignment) * h; break;
        case ImageType::RGB_U32:  out = getAlignedByteCount(w * 3 * 4, alignment) * h; break;
        case ImageType::RGB_F16:  out = getAlignedByteCount(w * 3 * 2, alignment) * h; break;
        case ImageType::RGB_F32:  out = getAlignedByteCount(w * 3 * 4, alignment) * h; break;

        case ImageType::RGBA_U8:  out = getAlignedByteCount(w * 4, alignment) * h; break;
        case ImageType::RGBA_U16: out = getAlignedByteCount(w * 4 * 2, alignment) * h; break;
        case ImageType::RGBA_U32: out = getAlignedByteCount(w * 4 * 4, alignment) * h; break;
        case ImageType::RGBA_F16: out = getAlignedByteCount(w * 4 * 2, alignment) * h; break;
        case ImageType::RGBA_F32: out = getAlignedByteCount(w * 4 * 4, alignment) * h; break;

            //! \todo Is YUV data aligned?
        case ImageType::YUV_420P_U8:  out = w * h + (w / 2 * h / 2) + (w / 2 * h / 2); break;
        case ImageType::YUV_422P_U8:  out = w * h + (w / 2 * h) + (w / 2 * h); break;
        case ImageType::YUV_444P_U8:  out = w * h * 3; break;
        case ImageType::YUV_420P_U16: out = (w * h + (w / 2 * h / 2) + (w / 2 * h / 2)) * 2; break;
        case ImageType::YUV_422P_U16: out = (w * h + (w / 2 * h) + (w / 2 * h)) * 2; break;
        case ImageType::YUV_444P_U16: out = (w * h * 3) * 2; break;

        case ImageType::ARGB_4444_Premult: out = w * h * 4 * 2; break;

        default: break;
        }
        return out;
    }

    std::string getLabel(const ImageInfo& info)
    {
        return ftk::Format("{0}x{1}:{2} {3}").
            arg(info.size.w).
            arg(info.size.h).
            arg(info.getAspect(), 2).
            arg(info.type);
    }

    namespace
    {
        std::atomic<size_t> objectCount = 0;
        std::atomic<size_t> totalByteCount = 0;
    }

    Image::Image(const ImageInfo& info, uint8_t* externalData) :
        _info(info),
        _byteCount(info.getByteCount()),
        _externalData(externalData)
    {
        ++objectCount;
        totalByteCount += _byteCount;

        if (externalData)
        {
            _data = externalData;
        }
        else
        {
            // Allocate a bit of extra space since FFmpeg sws_scale()
            // can read past the end.
            _data = new uint8_t[_byteCount + 16];
        }
    }

    Image::~Image()
    {
        if (!_externalData)
        {
            delete[] _data;
        }

        --objectCount;
        totalByteCount -= _byteCount;
    }

    std::shared_ptr<Image> Image::create(const ImageInfo& info)
    {
        return std::shared_ptr<Image>(new Image(info));
    }

    std::shared_ptr<Image> Image::create(const ImageInfo& info, uint8_t* externalData)
    {
        return std::shared_ptr<Image>(new Image(info, externalData));
    }

    std::shared_ptr<Image> Image::create(const Size2I& size, ImageType type)
    {
        return std::shared_ptr<Image>(new Image(ImageInfo(size, type)));
    }

    std::shared_ptr<Image> Image::create(int w, int h, ImageType type)
    {
        return std::shared_ptr<Image>(new Image(ImageInfo(w, h, type)));
    }

    void Image::setTags(const ImageTags& tags)
    {
        _tags = tags;
    }

    void Image::zero()
    {
        memset(_data, 0, _byteCount);
    }

    size_t Image::getObjectCount()
    {
        return objectCount;
    }

    size_t Image::getTotalByteCount()
    {
        return totalByteCount;
    }

    void to_json(nlohmann::json& json, const ImageMirror& in)
    {
        json["X"] = in.x;
        json["Y"] = in.y;
    }

    void from_json(const nlohmann::json& json, ImageMirror& out)
    {
        json.at("X").get_to(out.x);
        json.at("Y").get_to(out.y);
    }
}
