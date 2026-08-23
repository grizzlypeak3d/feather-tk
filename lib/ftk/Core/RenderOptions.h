// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>
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
    struct FTK_CORE_API_TYPE RenderOptions
    {
        //! Clear the canvas before rendering.
        bool clear = true;

        //! Clear color.
        Color4F clearColor = Color4F(0.F, 0.F, 0.F, 0.F);

        //! Texture pool byte count (enough for two 4K RGBA 16-bit frames).
        size_t texturePoolByteCount = 4096 * 4096 * 4 * 2 * 2;

        //! Texture cache byte count.
        size_t textureCacheByteCount = megabyte * 100;

        //! Glyph texture atlas size.
        int glyphAtlasSize =
#if defined(FTK_API_GLES_2)
            2048;
#else // FTK_API_GLES_2
            4096;
#endif // FTK_API_GLES_2

        //! Enable logging.
        bool log = true;

        FTK_CORE_API bool operator == (const RenderOptions&) const;
        FTK_CORE_API bool operator != (const RenderOptions&) const;
    };

    //! Line drawing options.
    struct FTK_CORE_API_TYPE LineOptions
    {
        float width = 1.F;

        FTK_CORE_API bool operator == (const LineOptions&) const;
        FTK_CORE_API bool operator != (const LineOptions&) const;
    };

    //! Channel display.
    enum class FTK_CORE_API_TYPE ChannelDisplay
    {
        Color,
        Red,
        Green,
        Blue,
        Alpha,

        Count,
        First = Color
    };
    FTK_ENUM(FTK_CORE_API, ChannelDisplay);

    //! Input video levels.
    enum class FTK_CORE_API_TYPE InputVideoLevels
    {
        FromFile,
        FullRange,
        LegalRange,

        Count,
        First = FromFile
    };
    FTK_ENUM(FTK_CORE_API, InputVideoLevels);

    //! Alpha channel blending.
    //!
    //! References:
    //! - https://microsoft.github.io/Win2D/html/PremultipliedAlpha.htm
    enum class FTK_CORE_API_TYPE AlphaBlend
    {
        None,
        Straight,
        Premultiplied,

        Count,
        First = None
    };
    FTK_ENUM(FTK_CORE_API, AlphaBlend);

    //! Image filtering.
    enum class FTK_CORE_API_TYPE ImageFilter
    {
        Nearest,
        Linear,
        //! A separable two pass resample with a wide kernel, rather than the
        //! four texels Linear reads. What it is for is minification: scaling
        //! an image down far enough that Linear misses most of it, which is
        //! what a thumbnail does and what the view does when it is zoomed out.
        HighQuality,

        Count,
        First = Nearest
    };
    FTK_ENUM(FTK_CORE_API, ImageFilter);

    //! Image filters.
    struct FTK_CORE_API_TYPE ImageFilters
    {
        ImageFilter minify  = ImageFilter::Linear;
        ImageFilter magnify = ImageFilter::Linear;

        FTK_CORE_API bool operator == (const ImageFilters&) const;
        FTK_CORE_API bool operator != (const ImageFilters&) const;
    };

    //! Image options.
    struct FTK_CORE_API_TYPE ImageOptions
    {
        ChannelDisplay   channelDisplay = ChannelDisplay::Color;
        InputVideoLevels videoLevels    = InputVideoLevels::FromFile;
        AlphaBlend       alphaBlend     = AlphaBlend::Straight;
        ImageFilters     imageFilters;
        bool             cache          = true;

        FTK_CORE_API bool operator == (const ImageOptions&) const;
        FTK_CORE_API bool operator != (const ImageOptions&) const;
    };
        
    FTK_CORE_API void to_json(nlohmann::json&, const ImageFilters&);
    FTK_CORE_API void to_json(nlohmann::json&, const ImageOptions&);

    FTK_CORE_API void from_json(const nlohmann::json&, ImageFilters&);
    FTK_CORE_API void from_json(const nlohmann::json&, ImageOptions&);

    ///@}
}

#include <ftk/Core/RenderOptionsInline.h>

