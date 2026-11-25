// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>
#include <ftk/Core/Box.h>
#include <ftk/Core/ISystem.h>
#include <ftk/Core/Image.h>
#include <ftk/Core/Observable.h>
#include <ftk/Core/ObservableList.h>

namespace ftk
{
    class Context;

    //! \name Fonts
    ///@{

    //! Font types.
    enum class FTK_CORE_API_TYPE FontType
    {
        Regular,
        Bold,
        Mono,
        Symbols,

        Count,
        First = Regular
    };
    FTK_ENUM(FTK_CORE_API, FontType);

    //! Get a built-in font.
    FTK_CORE_API std::string getDefaultFont(FontType);

    //! Font information.
    struct FTK_CORE_API_TYPE FontInfo
    {
        FontInfo() = default;
        FontInfo(const std::string&, int size);

        //! The font name: a built-in font (see getDefaultFont()) or one added to
        //! the font system. A name the font system does not have is drawn with
        //! the default regular font.
        std::string name = getDefaultFont(FontType::Regular);
        //! The font size, in pixels.
        int         size = 12;

        bool operator == (const FontInfo&) const = default;
        FTK_CORE_API bool operator < (const FontInfo&) const;
    };

    //! Font metrics.
    struct FTK_CORE_API_TYPE FontMetrics
    {
        //! The distance from the baseline up to the top of the font, in pixels.
        int ascender   = 0;
        //! The distance from the baseline to the bottom of the font, in pixels.
        //! It is below the baseline, so it is usually negative.
        int descender  = 0;
        //! The distance from one baseline to the next, in pixels.
        int lineHeight = 0;
    };

    //! Font glyph information.
    struct FTK_CORE_API_TYPE GlyphInfo
    {
        GlyphInfo() = default;
        GlyphInfo(uint32_t code, const FontInfo&);

        //! The Unicode code point.
        uint32_t code     = 0;
        //! The font.
        FontInfo fontInfo;

        bool operator == (const GlyphInfo&) const = default;
        FTK_CORE_API bool operator < (const GlyphInfo&) const;
    };

    //! Font glyph.
    struct FTK_CORE_API_TYPE Glyph
    {
        //! The code point and font the glyph is for.
        GlyphInfo              info;
        //! The rendered glyph: one channel of 8-bit coverage. Null when no font
        //! has the glyph or it cannot be rendered.
        std::shared_ptr<Image> image;
        //! Where the top left of the image is from the pen position on the
        //! baseline, in pixels, with y up.
        V2I                    offset;
        //! How far the pen moves on to the next glyph, in pixels.
        int                    advance  = 0;
        //! The change hinting made to the left side bearing, in 64ths of a pixel.
        //! Text layout compares it with the previous glyph's rsbDelta and moves
        //! the glyph a pixel when they differ by more than about half a pixel.
        int32_t                lsbDelta = 0;
        //! The change hinting made to the right side bearing, in 64ths of a
        //! pixel.
        int32_t                rsbDelta = 0;
    };

    //! Font system.
    //!
    //! \todo Add text elide functionality.
    //! \todo Add support for gamma correction?
    //! - https://www.freetype.org/freetype2/docs/text-rendering-general.html
    //! \todo Use a text-shaping library.
    class FTK_CORE_API_TYPE FontSystem : public ISystem
    {
        FTK_NON_COPYABLE(FontSystem);

    protected:
        FontSystem(const std::shared_ptr<Context>&);

    public:
        FTK_CORE_API virtual ~FontSystem();

        //! Create a new system.
        FTK_CORE_API static std::shared_ptr<FontSystem> create(
            const std::shared_ptr<Context>&);

        //! \name Fonts
        ///@{

        //! Get the names of the fonts.
        FTK_CORE_API std::vector<std::string> getFonts() const;
        //! Observe the names of the fonts.
        FTK_CORE_API std::shared_ptr<IObservableList<std::string> > observeFonts() const;
        //! Add a font from a file, under the given name.
        FTK_CORE_API bool addFont(const std::string& name, const std::string& fileName);
        //! Add a font from data in memory, which is copied, under the given name.
        //! Returns whether the font could be read.
        FTK_CORE_API bool addFont(const std::string& name, const uint8_t*, size_t);
        //! Remove a font.
        FTK_CORE_API void removeFont(const std::string&);

        //! Register an already-added font (see addFont) as a fallback for
        //! glyphs missing from the requested font. Fallbacks are consulted in
        //! registration order. Useful for scripts the bundled fonts do not
        //! cover.
        FTK_CORE_API void addFallbackFont(const std::string& name);

        ///@}

        //! \name Information
        ///@{

        //! Get the number of glyphs in the cache. It is updated once a second.
        FTK_CORE_API size_t getGlyphCacheSize() const;
        //! Get how full the glyph cache is, as a percentage of its maximum. It is
        //! updated once a second.
        FTK_CORE_API float getGlyphCachePercentage() const;
        //! Observe the number of glyphs in the cache.
        FTK_CORE_API std::shared_ptr<IObservable<size_t> > observeGlyphCacheSize() const;
        //! Observe how full the glyph cache is, as a percentage of its maximum.
        FTK_CORE_API std::shared_ptr<IObservable<float> > observeGlyphCachePercentage() const;

        ///@}

        //! \name Measure
        ///@{

        //! Get font metrics.
        FTK_CORE_API FontMetrics getMetrics(const FontInfo&);

        //! Get the size of the given string.
        FTK_CORE_API Size2I getSize(
            const std::string&,
            const FontInfo&,
            int maxLineWidth = 0);

        //! Get the character boxes for the given string.
        FTK_CORE_API std::vector<Box2I> getBoxes(
            const std::string&,
            const FontInfo&,
            int maxLineWidth = 0);

        ///@}

        //! \name Glyphs
        ///@{

        //! Get the glyphs for the given string.
        FTK_CORE_API std::vector<std::shared_ptr<Glyph> > getGlyphs(
            const std::string&,
            const FontInfo&);

        ///@}

        FTK_CORE_API void tick() override;
        FTK_CORE_API std::chrono::milliseconds getTickTime() const override;

    private:
        FTK_PRIVATE();
    };

    FTK_CORE_API void to_json(nlohmann::json&, const FontInfo&);

    FTK_CORE_API void from_json(const nlohmann::json&, FontInfo&);

    FTK_CORE_API std::ostream& operator << (std::ostream&, const FontInfo&);

    ///@}
}

namespace std
{
    template<>
    struct hash<ftk::FontInfo>
    {
        size_t operator()(const ftk::FontInfo& v) const noexcept
        {
            const size_t seed = std::hash<std::string>{}(v.name);
            return ftk::hashCombine(seed, std::hash<int>{}(v.size));
        }
    };

    template<>
    struct hash<ftk::GlyphInfo>
    {
        size_t operator()(const ftk::GlyphInfo& v) const noexcept
        {
            const size_t seed = std::hash<uint32_t>{}(v.code);
            return ftk::hashCombine(seed, std::hash<ftk::FontInfo>{}(v.fontInfo));
        }
    };
}

#include <ftk/Core/FontSystemInline.h>

