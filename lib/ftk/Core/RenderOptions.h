// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Color.h>
#include <ftk/Core/Image.h>
#include <ftk/Core/Memory.h>

#include <iostream>
#include <string>
#include <vector>

namespace ftk
{
    //! \name Rendering
    ///@{

    //! Render options.
    struct FTK_API_TYPE RenderOptions
    {
        //! Clear the canvas before rendering.
        bool clear = true;

        //! Clear color.
        Color4F clearColor = Color4F(0.F, 0.F, 0.F, 0.F);

        //! Texture cache byte count.
        size_t textureCacheByteCount = gigabyte / 4;

        //! Glyph texture atlas size.
        int glyphAtlasSize =
#if defined(FTK_API_GLES_2)
            2048;
#else // FTK_API_GLES_2
            4096;
#endif // FTK_API_GLES_2

        //! Enable logging.
        bool log = true;

        bool operator == (const RenderOptions&) const;
        bool operator != (const RenderOptions&) const;
    };

    //! Line drawing options.
    struct FTK_API_TYPE LineOptions
    {
        float width = 1.F;

        bool operator == (const LineOptions&) const;
        bool operator != (const LineOptions&) const;
    };

    //! Channel display.
    enum class FTK_API_TYPE ChannelDisplay
    {
        Color,
        Red,
        Green,
        Blue,
        Alpha,

        Count,
        First = Color
    };
    FTK_ENUM(ChannelDisplay);

    //! Input video levels.
    enum class FTK_API_TYPE InputVideoLevels
    {
        FromFile,
        FullRange,
        LegalRange,

        Count,
        First = FromFile
    };
    FTK_ENUM(InputVideoLevels);

    //! Alpha channel blending.
    //!
    //! References:
    //! - https://microsoft.github.io/Win2D/html/PremultipliedAlpha.htm
    enum class FTK_API_TYPE AlphaBlend
    {
        None,
        Straight,
        Premultiplied,

        Count,
        First = None
    };
    FTK_ENUM(AlphaBlend);

    //! Image filtering.
    enum class FTK_API_TYPE ImageFilter
    {
        Nearest,
        Linear,

        Count,
        First = Nearest
    };
    FTK_ENUM(ImageFilter);

    //! Image filters.
    struct FTK_API_TYPE ImageFilters
    {
        ImageFilter minify  = ImageFilter::Linear;
        ImageFilter magnify = ImageFilter::Linear;

        bool operator == (const ImageFilters&) const;
        bool operator != (const ImageFilters&) const;
    };

    //! Image options.
    struct FTK_API_TYPE ImageOptions
    {
        ChannelDisplay   channelDisplay = ChannelDisplay::Color;
        InputVideoLevels videoLevels    = InputVideoLevels::FromFile;
        AlphaBlend       alphaBlend     = AlphaBlend::Straight;
        ImageFilters     imageFilters;
        bool             cache          = true;

        bool operator == (const ImageOptions&) const;
        bool operator != (const ImageOptions&) const;
    };
        
    FTK_API void to_json(nlohmann::json&, const ImageFilters&);
    FTK_API void to_json(nlohmann::json&, const ImageOptions&);

    FTK_API void from_json(const nlohmann::json&, ImageFilters&);
    FTK_API void from_json(const nlohmann::json&, ImageOptions&);

    ///@}
}

#include <ftk/Core/RenderOptionsInline.h>

