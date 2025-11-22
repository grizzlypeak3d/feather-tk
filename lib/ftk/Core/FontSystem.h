// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Box.h>
#include <ftk/Core/ISystem.h>
#include <ftk/Core/Image.h>

namespace ftk
{
    class Context;

    //! \name Fonts
    ///@{

    //! Built-in fonts.
    enum class Font
    {
        Regular,
        Bold,
        Mono,

        Count,
        First = Regular
    };
    FTK_ENUM(Font);

    //! Get a built-in font.
    const std::string& getFont(Font);

    //! Font information.
    struct FontInfo
    {
        FontInfo() = default;
        FontInfo(const std::string& family, int size);

        std::string family = getFont(Font::Regular);
        int         size   = 12;

        bool operator == (const FontInfo&) const;
        bool operator != (const FontInfo&) const;
        bool operator < (const FontInfo&) const;
    };

    //! Font metrics.
    struct FontMetrics
    {
        int ascender   = 0;
        int descender  = 0;
        int lineHeight = 0;
    };

    //! Font glyph information.
    struct GlyphInfo
    {
        GlyphInfo() = default;
        GlyphInfo(uint32_t code, const FontInfo&);

        uint32_t code     = 0;
        FontInfo fontInfo;

        bool operator == (const GlyphInfo&) const;
        bool operator != (const GlyphInfo&) const;
        bool operator < (const GlyphInfo&) const;
    };

    //! Font glyph.
    struct Glyph
    {
        GlyphInfo              info;
        std::shared_ptr<Image> image;
        V2I                    offset;
        int                    advance  = 0;
        int32_t                lsbDelta = 0;
        int32_t                rsbDelta = 0;
    };

    //! Font system.
    //!
    //! \todo Add text elide functionality.
    //! \todo Add support for gamma correction?
    //! - https://www.freetype.org/freetype2/docs/text-rendering-general.html
    class FontSystem : public ISystem
    {
        FTK_NON_COPYABLE(FontSystem);

    protected:
        FontSystem(const std::shared_ptr<Context>&);

    public:
        virtual ~FontSystem();

        //! Create a new system.
        static std::shared_ptr<FontSystem> create(const std::shared_ptr<Context>&);

        //! Add a font.
        void addFont(const std::string& name, const uint8_t*, size_t);

        //! \name Information
        ///@{

        //! Get the glyph cache size.
        size_t getGlyphCacheSize() const;

        //! Get the percentage of the glyph cache in use.
        float getGlyphCachePercentage() const;

        ///@}

        //! \name Measure
        ///@{

        //! Get font metrics.
        FontMetrics getMetrics(const FontInfo&);

        //! Get the size of the given string.
        Size2I getSize(
            const std::string&,
            const FontInfo&,
            int maxLineWidth = 0);

        //! Get the character boxes for the given string.
        std::vector<Box2I> getBoxes(
            const std::string&,
            const FontInfo&,
            int maxLineWidth = 0);

        ///@}

        //! \name Glyphs
        ///@{

        //! Get the glyphs for the given string.
        std::vector<std::shared_ptr<Glyph> > getGlyphs(
            const std::string&,
            const FontInfo&);

        ///@}

    private:
        FTK_PRIVATE();
    };

    void to_json(nlohmann::json&, const FontInfo&);

    void from_json(const nlohmann::json&, FontInfo&);

    std::ostream& operator << (std::ostream&, const FontInfo&);

    ///@}
}

#include <ftk/Core/FontSystemInline.h>

