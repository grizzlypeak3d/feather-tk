// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Memory.h>
#include <ftk/Core/Range.h>
#include <ftk/Core/Util.h>
#include <ftk/Core/Vector.h>

#include <iostream>
#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace ftk
{
    //! \name Images
    ///@{

    //! Image types.
    enum class FTK_API_TYPE ImageType
    {
        None,

        L_U8,
        L_U16,
        L_U32,
        L_F16,
        L_F32,

        LA_U8,
        LA_U16,
        LA_U32,
        LA_F16,
        LA_F32,

        RGB_U8,
        RGB_U10,
        RGB_U16,
        RGB_U32,
        RGB_F16,
        RGB_F32,

        RGBA_U8,
        RGBA_U16,
        RGBA_U32,
        RGBA_F16,
        RGBA_F32,

        YUV_420P_U8,
        YUV_422P_U8,
        YUV_444P_U8,

        YUV_420P_U16,
        YUV_422P_U16,
        YUV_444P_U16,

        ARGB_4444_Premult,

        Count,
        First = None
    };
    FTK_ENUM(ImageType);

    //! Get the number of channels for the given image type.
    FTK_API int getChannelCount(ImageType);

    //! Get the bit-depth for the given image type.
    FTK_API int getBitDepth(ImageType);

    //! Video levels.
    enum class FTK_API_TYPE VideoLevels
    {
        FullRange,
        LegalRange,

        Count,
        First = FullRange
    };
    FTK_ENUM(VideoLevels);

    //! YUV coefficients.
    enum class FTK_API_TYPE YUVCoefficients
    {
        REC709,
        BT2020,

        Count,
        First = REC709
    };
    FTK_ENUM(YUVCoefficients);

    //! Get YUV coefficients.
    FTK_API V4F getYUVCoefficients(YUVCoefficients);

    //! Image mirroring.
    struct FTK_API_TYPE ImageMirror
    {
        ImageMirror() = default;
        constexpr ImageMirror(bool x, bool y);

        bool x = false;
        bool y = false;

        constexpr bool operator == (const ImageMirror&) const;
        constexpr bool operator != (const ImageMirror&) const;
    };

    //! Image data layout.
    struct FTK_API_TYPE ImageLayout
    {
        ImageLayout() = default;
        ImageLayout(
            const ImageMirror& mirror,
            int                alignment = 1,
            Endian             endian    = getEndian());

        ImageMirror mirror;
        int         alignment = 1;
        Endian      endian    = getEndian();

        constexpr bool operator == (const ImageLayout&) const;
        constexpr bool operator != (const ImageLayout&) const;
    };

    //! Image information.
    struct FTK_API_TYPE ImageInfo
    {
        ImageInfo() = default;
        ImageInfo(const Size2I&, ImageType);
        ImageInfo(int w, int h, ImageType);

        std::string     name             = "Default";
        Size2I          size;
        ImageType       type             = ImageType::None;
        float           pixelAspectRatio = 1.F;
        VideoLevels     videoLevels      = VideoLevels::FullRange;
        YUVCoefficients yuvCoefficients  = YUVCoefficients::REC709;
        ImageLayout     layout;

        //! Is the information valid?
        bool isValid() const;
                
        //! Get the aspect ratio.
        float getAspect() const;

        //! Get the number of bytes used to store an image.
        size_t getByteCount() const;

        bool operator == (const ImageInfo&) const;
        bool operator != (const ImageInfo&) const;
    };

    //! Image tags.
    typedef std::map<std::string, std::string> ImageTags;

    //! Image.
    class FTK_API_TYPE Image : public std::enable_shared_from_this<Image>
    {
        FTK_NON_COPYABLE(Image);

    protected:
        Image(const ImageInfo&, uint8_t* externalData = nullptr);

    public:
        FTK_API ~Image();

        //! Create a new image.
        FTK_API static std::shared_ptr<Image> create(const ImageInfo&);

        //! Create a new image.
        FTK_API static std::shared_ptr<Image> create(const ImageInfo&, uint8_t* externalData);

        //! Create a new image.
        FTK_API static std::shared_ptr<Image> create(const Size2I&, ImageType);

        //! Create a new image.
        FTK_API static std::shared_ptr<Image> create(int w, int h, ImageType);

        //! Get the image information.
        const ImageInfo& getInfo() const;
            
        //! Get the image size.
        const Size2I& getSize() const;

        //! Get the image width.
        int getWidth() const;

        //! Get the image height.
        int getHeight() const;

        //! Get the aspect ratio.
        float getAspect() const;

        //! Get the image type.
        ImageType getType() const;

        //! Is the image valid?
        bool isValid() const;

        //! Get the image tags.
        const ImageTags& getTags() const;

        //! Set the image tags.
        FTK_API void setTags(const ImageTags&);

        //! Get the number of bytes used to store the image data.
        size_t getByteCount() const;

        //! Get the image data.
        const uint8_t* getData() const;

        //! Get the image data.
        uint8_t* getData();

        //! Zero the image data.
        FTK_API void zero();

    private:
        ImageInfo _info;
        ImageTags _tags;
        size_t    _byteCount    = 0;
        uint8_t*  _data         = nullptr;
        bool      _externalData = false;
    };

    FTK_API void to_json(nlohmann::json&, const ImageMirror&);

    FTK_API void from_json(const nlohmann::json&, ImageMirror&);

    ///@}
}

#include <ftk/Core/ImageInline.h>

