// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/FontSystem.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Error.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LRUCache.h>
#include <ftk/Core/LogSystem.h>
#include <ftk/Core/String.h>

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include <algorithm>
#include <codecvt>
#include <limits>
#include <locale>
#include <map>

namespace ftk_resource
{
    extern std::vector<uint8_t> NotoSansBold;
    extern std::vector<uint8_t> NotoMonoRegular;
    extern std::vector<uint8_t> NotoSansRegular;
}

namespace ftk
{
    FTK_ENUM_IMPL(
        Font,
        "Regular",
        "Bold",
        "Mono");

    const std::string& getFont(Font value)
    {
        static const std::array<std::string, static_cast<size_t>(Font::Count)> data =
        {
            "NotoSans-Regular",
            "NotoSans-Bold",
            "NotoMono-Regular"
        };
        return data[static_cast<size_t>(value)];
    }

    namespace
    {
#if defined(_WINDOWS)
        //! \bug https://social.msdn.microsoft.com/Forums/vstudio/en-US/8f40dcd8-c67f-4eba-9134-a19b9178e481/vs-2015-rc-linker-stdcodecvt-error?forum=vcgeneral
        typedef unsigned int ftk_char_t;
#else // _WINDOWS
        typedef char32_t ftk_char_t;
#endif // _WINDOWS
    }

    struct FontSystem::Private
    {
        std::shared_ptr<Glyph> getGlyph(uint32_t code, const FontInfo&);
        void measure(
            const std::basic_string<ftk_char_t>& utf32,
            const FontInfo&,
            int maxLineWidth,
            Size2I&,
            std::vector<Box2I>* = nullptr);

        std::map<std::string, std::vector<uint8_t> > fontData;
        FT_Library ftLibrary = nullptr;
        std::map<std::string, FT_Face> ftFaces;
        std::wstring_convert<std::codecvt_utf8<ftk_char_t>, ftk_char_t> utf32Convert;
        ImageType imageType = ImageType::L_U8;
        LRUCache<GlyphInfo, std::shared_ptr<Glyph> > glyphCache;
    };

    FontSystem::FontSystem(const std::shared_ptr<Context>& context) :
        ISystem(context, "ftk::FontSystem"),
        _p(new Private)
    {
        FTK_P();

        p.fontData[getFont(Font::Regular)] = ftk_resource::NotoSansRegular;
        p.fontData[getFont(Font::Bold)] = ftk_resource::NotoSansBold;
        p.fontData[getFont(Font::Mono)] = ftk_resource::NotoMonoRegular;

#if defined(FTK_API_GLES_2)
        //! \bug Some GLES 2 implementations (Pi Zero W) only support RGBA?
        p.imageType = ImageType::RGBA_U8;
#endif // FTK_API_GLES_2
        try
        {
            FT_Error ftError = FT_Init_FreeType(&p.ftLibrary);
            if (ftError)
            {
                throw std::runtime_error("FreeType cannot be initialized");
            }

            for (const auto& i : p.fontData)
            {
                ftError = FT_New_Memory_Face(
                    p.ftLibrary,
                    i.second.data(),
                    i.second.size(),
                    0,
                    &p.ftFaces[i.first]);
                if (ftError)
                {
                    throw std::runtime_error(Format("Cannot create font: \"{0}\"").arg(i.first));
                }
            }
        }
        catch (const std::exception& e)
        {
            if (auto context = _context.lock())
            {
                auto logSystem = context->getSystem<LogSystem>();
                logSystem->print("ftk::FontSystem", e.what(), LogType::Error);
            }
        }
    }

    FontSystem::~FontSystem()
    {
        FTK_P();
        if (p.ftLibrary)
        {
            for (const auto& i : p.ftFaces)
            {
                FT_Done_Face(i.second);
            }
            FT_Done_FreeType(p.ftLibrary);
        }
    }

    std::shared_ptr<FontSystem> FontSystem::create(const std::shared_ptr<Context>& context)
    {
        return std::shared_ptr<FontSystem>(new FontSystem(context));
    }

    void FontSystem::addFont(const std::string& name, const uint8_t* data, size_t size)
    {
        FTK_P();
        p.fontData[name] = std::vector<uint8_t>(size);
        memcpy(p.fontData[name].data(), data, size);
        FT_Error ftError = FT_New_Memory_Face(
            p.ftLibrary,
            p.fontData[name].data(),
            size,
            0,
            &p.ftFaces[name]);
        if (ftError)
        {
            throw std::runtime_error(Format("Cannot create font: \"{0}\"").arg(name));
        }
    }

    size_t FontSystem::getGlyphCacheSize() const
    {
        return _p->glyphCache.getSize();
    }

    float FontSystem::getGlyphCachePercentage() const
    {
        return _p->glyphCache.getPercentage();
    }

    FontMetrics FontSystem::getMetrics(const FontInfo& info)
    {
        FTK_P();
        FontMetrics out;
        const auto ftFaceIt = p.ftFaces.find(info.family);
        if (ftFaceIt != p.ftFaces.end())
        {
            FT_Error ftError = FT_Set_Pixel_Sizes(ftFaceIt->second, 0, info.size);
            out.ascender = ftFaceIt->second->size->metrics.ascender / 64;
            out.descender = ftFaceIt->second->size->metrics.descender / 64;
            out.lineHeight = ftFaceIt->second->size->metrics.height / 64;
        }
        return out;
    }

    Size2I FontSystem::getSize(
        const std::string& text,
        const FontInfo& fontInfo,
        int maxLineWidth)
    {
        FTK_P();
        Size2I out;
        try
        {
            const auto utf32 = p.utf32Convert.from_bytes(text);
            p.measure(utf32, fontInfo, maxLineWidth, out);
        }
        catch (const std::exception&)
        {}
        return out;
    }

    std::vector<Box2I> FontSystem::getBoxes(
        const std::string& text,
        const FontInfo& fontInfo,
        int maxLineWidth)
    {
        FTK_P();
        std::vector<Box2I> out;
        try
        {
            const auto utf32 = p.utf32Convert.from_bytes(text);
            Size2I size;
            p.measure(utf32, fontInfo, maxLineWidth, size, &out);
        }
        catch (const std::exception&)
        {}
        return out;
    }

    std::vector<std::shared_ptr<Glyph> > FontSystem::getGlyphs(
        const std::string& text,
        const FontInfo& fontInfo)
    {
        FTK_P();
        std::vector<std::shared_ptr<Glyph> > out;
        try
        {
            const auto utf32 = p.utf32Convert.from_bytes(text);
            for (const auto& i : utf32)
            {
                out.push_back(p.getGlyph(i, fontInfo));
            }
        }
        catch (const std::exception&)
        {}
        return out;
    }

    std::shared_ptr<Glyph> FontSystem::Private::getGlyph(uint32_t code, const FontInfo& fontInfo)
    {
        std::shared_ptr<Glyph> out;
        if (!glyphCache.get(GlyphInfo(code, fontInfo), out))
        {
            out = std::make_shared<Glyph>();
            out->info = GlyphInfo(code, fontInfo);

            const auto ftFaceIt = ftFaces.find(fontInfo.family);
            if (ftFaceIt != ftFaces.end())
            {
                FT_Error ftError = FT_Set_Pixel_Sizes(
                    ftFaceIt->second,
                    0,
                    static_cast<int>(fontInfo.size));
                if (ftError)
                {
                    throw std::runtime_error(
                        Format("Cannot set pixel sizes: \"{0}\"").arg(fontInfo.family));
                }
                if (auto ftGlyphIndex = FT_Get_Char_Index(ftFaceIt->second, code))
                {
                    ftError = FT_Load_Glyph(ftFaceIt->second, ftGlyphIndex, FT_LOAD_FORCE_AUTOHINT);
                    if (ftError)
                    {
                        throw std::runtime_error(
                            Format("Cannot load glyph: \"{0}\"").arg(fontInfo.family));
                    }
                    FT_Render_Mode renderMode = FT_RENDER_MODE_NORMAL;
                    uint8_t renderModeChannels = 1;
                    ftError = FT_Render_Glyph(ftFaceIt->second->glyph, renderMode);
                    if (ftError)
                    {
                        throw std::runtime_error(
                            Format("Cannot render glyph: \"{0}\"").arg(fontInfo.family));
                    }

                    auto ftBitmap = ftFaceIt->second->glyph->bitmap;
                    const ImageInfo imageInfo(ftBitmap.width, ftBitmap.rows, imageType);
                    out->image = Image::create(imageInfo);
                    for (size_t y = 0; y < ftBitmap.rows; ++y)
                    {
                        const int channelCount = getChannelCount(imageInfo.type);
                        uint8_t* dataP = out->image->getData() + y * imageInfo.size.w * channelCount;
                        const unsigned char* bitmapP = ftBitmap.buffer + y * ftBitmap.pitch;
                        switch (channelCount)
                        {
                        case 1:
                            memcpy(dataP, bitmapP, imageInfo.size.w);
                            break;
                        case 2:
                            for (size_t x = 0; x < imageInfo.size.w; ++x)
                            {
                                dataP[x * 2 + 0] = bitmapP[x];
                                dataP[x * 2 + 1] = bitmapP[x];
                            }
                            break;
                        case 3:
                            for (size_t x = 0; x < imageInfo.size.w; ++x)
                            {
                                dataP[x * 3 + 0] = bitmapP[x];
                                dataP[x * 3 + 1] = bitmapP[x];
                                dataP[x * 3 + 2] = bitmapP[x];
                            }
                            break;
                        case 4:
                            for (size_t x = 0; x < imageInfo.size.w; ++x)
                            {
                                dataP[x * 4 + 0] = bitmapP[x];
                                dataP[x * 4 + 1] = bitmapP[x];
                                dataP[x * 4 + 2] = bitmapP[x];
                                dataP[x * 4 + 3] = bitmapP[x];
                            }
                            break;
                        default: break;
                        }
                    }
                    out->offset = V2I(ftFaceIt->second->glyph->bitmap_left, ftFaceIt->second->glyph->bitmap_top);
                    out->advance = ftFaceIt->second->glyph->advance.x / 64;
                    out->lsbDelta = ftFaceIt->second->glyph->lsb_delta;
                    out->rsbDelta = ftFaceIt->second->glyph->rsb_delta;
                }
            }

            glyphCache.add(out->info, out);
        }
        return out;
    }

    namespace
    {
        constexpr bool isSpace(ftk_char_t c)
        {
            return ' ' == c || '\t' == c;
        }
    }

    void FontSystem::Private::measure(
        const std::basic_string<ftk_char_t>& utf32,
        const FontInfo& fontInfo,
        int maxLineWidth,
        Size2I& size,
        std::vector<Box2I>* glyphGeom)
    {
        const auto ftFaceIt = ftFaces.find(fontInfo.family);
        if (ftFaceIt != ftFaces.end())
        {
            V2I pos;
            FT_Error ftError = FT_Set_Pixel_Sizes(
                ftFaceIt->second,
                0,
                static_cast<int>(fontInfo.size));
            if (ftError)
            {
                throw std::runtime_error(
                    Format("Cannot set pixel sizes: \"{0}\"").arg(fontInfo.family));
            }

            const int h = ftFaceIt->second->size->metrics.height / 64;
            pos.y = h;
            auto textLine = utf32.end();
            int textLineX = 0;
            int32_t rsbDeltaPrev = 0;
            for (auto utf32It = utf32.begin(); utf32It != utf32.end(); ++utf32It)
            {
                const auto glyph = getGlyph(*utf32It, fontInfo);

                if (glyphGeom)
                {
                    Box2I box;
                    if (glyph)
                    {
                        box = Box2I(
                            pos.x,
                            pos.y - h,
                            glyph->advance,
                            h);
                    }
                    glyphGeom->push_back(box);
                }

                int32_t x = 0;
                V2I posAndSize;
                if (glyph)
                {
                    x = glyph->advance;
                    if (rsbDeltaPrev - glyph->lsbDelta > 32)
                    {
                        x -= 1;
                    }
                    else if (rsbDeltaPrev - glyph->lsbDelta < -31)
                    {
                        x += 1;
                    }
                    rsbDeltaPrev = glyph->rsbDelta;
                }
                else
                {
                    rsbDeltaPrev = 0;
                }

                if ('\n' == *utf32It)
                {
                    size.w = std::max(size.w, pos.x);
                    pos.x = 0;
                    pos.y += h;
                    rsbDeltaPrev = 0;
                    auto crIt = utf32It + 1;
                    if (crIt != utf32.end() && '\r' == *crIt)
                    {
                        ++utf32It;
                    }
                }
                else if (
                    maxLineWidth > 0 &&
                    pos.x > 0 &&
                    pos.x + (!isSpace(*utf32It) ? x : 0) >= maxLineWidth)
                {
                    if (textLine != utf32.end())
                    {
                        utf32It = textLine;
                        textLine = utf32.end();
                        size.w = std::max(size.w, textLineX);
                        pos.x = 0;
                        pos.y += h;
                    }
                    else
                    {
                        size.w = std::max(size.w, pos.x);
                        pos.x = x;
                        pos.y += h;
                    }
                    rsbDeltaPrev = 0;
                }
                else
                {
                    if (isSpace(*utf32It) && utf32It != utf32.begin())
                    {
                        textLine = utf32It;
                        textLineX = pos.x;
                    }
                    pos.x += x;
                }
            }
            size.w = std::max(size.w, pos.x);
            size.h = pos.y;
        }
    }

    void to_json(nlohmann::json& json, const FontInfo& value)
    {
        json["Family"] = value.family;
        json["Size"] = value.size;
    }

    void from_json(const nlohmann::json& json, FontInfo& value)
    {
        json.at("Family").get_to(value.family);
        json.at("Size").get_to(value.size);
    }

    std::ostream& operator << (std::ostream& os, const FontInfo& value)
    {
        os << value.family << ":" << value.size;
        return os;
    }
}