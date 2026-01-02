// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

namespace ftk
{
    constexpr ImageFlip::ImageFlip(bool x, bool y) :
        x(x),
        y(y)
    {}

    constexpr bool ImageFlip::operator == (const ImageFlip& other) const
    {
        return other.x == x && other.y == y;
    }

    constexpr bool ImageFlip::operator != (const ImageFlip& other) const
    {
        return !(other == *this);
    }

    inline ImageLayout::ImageLayout(
        const ImageFlip& flip,
        int alignment,
        Endian endian) :
        flip(flip),
        alignment(alignment),
        endian(endian)
    {}

    constexpr bool ImageLayout::operator == (const ImageLayout & other) const
    {
        return
            other.flip == flip &&
            other.alignment == alignment &&
            other.endian == endian;
    }

    constexpr bool ImageLayout::operator != (const ImageLayout & other) const
    {
        return !(other == *this);
    }

    inline ImageInfo::ImageInfo(const Size2I& size, ImageType type) :
        size(size),
        type(type)
    {}
        
    inline ImageInfo::ImageInfo(int w, int h, ImageType type) :
        size(w, h),
        type(type)
    {}

    inline bool ImageInfo::isValid() const
    {
        return size.isValid() && type != ImageType::None;
    }

    inline float ImageInfo::getAspect() const
    {
        return size.h > 0 ? (size.w / static_cast<float>(size.h) * pixelAspectRatio) : 0;
    }
        
    inline bool ImageInfo::operator == (const ImageInfo& other) const
    {
        return
            name == other.name &&
            size == other.size &&
            type == other.type &&
            pixelAspectRatio == other.pixelAspectRatio &&
            videoLevels == other.videoLevels &&
            yuvCoefficients == other.yuvCoefficients &&
            layout == other.layout;
    }
        
    inline bool ImageInfo::operator != (const ImageInfo& other) const
    {
        return !(*this == other);
    }

    inline const ImageInfo& Image::getInfo() const
    {
        return _info;
    }

    inline const Size2I& Image::getSize() const
    {
        return _info.size;
    }

    inline int Image::getWidth() const
    {
        return _info.size.w;
    }

    inline int Image::getHeight() const
    {
        return _info.size.h;
    }

    inline float Image::getAspect() const
    {
        return _info.getAspect();
    }

    inline ImageType Image::getType() const
    {
        return _info.type;
    }

    inline bool Image::isValid() const
    {
        return _info.isValid();
    }

    inline const ImageTags& Image::getTags() const
    {
        return _tags;
    }

    inline size_t Image::getByteCount() const
    {
        return _byteCount;
    }

    inline const uint8_t* Image::getData() const
    {
        return _data;
    }

    inline uint8_t* Image::getData()
    {
        return _data;
    }
}
