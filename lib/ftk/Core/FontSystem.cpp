// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/FontSystem.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/Error.h>
#include <ftk/Core/FileIO.h>
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
#include <mutex>

namespace ftk_resource
{
    extern std::vector<uint8_t> NotoSans_Bold;
    extern std::vector<uint8_t> NotoMono_Regular;
    extern std::vector<uint8_t> NotoSans_Regular;
    extern std::vector<uint8_t> NotoSansSymbols2_Regular;
}

namespace ftk
{
    FTK_ENUM_IMPL(
        FontType,
        "Regular",
        "Bold",
        "Mono",
        "Symbols");

    std::string getDefaultFont(FontType value)
    {
        const std::array<std::string, static_cast<size_t>(FontType::Count)> data =
        {
            "NotoSans-Regular",
            "NotoSans-Bold",
            "NotoMono-Regular",
            "NotoSansSymbols2-Regular"
        };
        return data[static_cast<size_t>(value)];
    }

    namespace
    {
        const ImageType imageType =
#if defined(FTK_API_GLES_2)
        //! \bug Some GLES 2 implementations (Pi Zero W) only support RGBA?
            ImageType::RGBA_U8;
#else  // FTK_API_GLES_2
            ImageType::L_U8;
#endif // FTK_API_GLES_2

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

        FT_Library ftLibrary = nullptr;
        std::shared_ptr<ObservableList<std::string> > fonts;
        std::shared_ptr<Observable<size_t> > glyphCacheSize;
        std::shared_ptr<Observable<float> > glyphCachePercentage;

        std::mutex mutex;
        std::map<std::string, std::vector<uint8_t> > fontData;
        std::map<std::string, FT_Face> faces;
        std::wstring_convert<std::codecvt_utf8<ftk_char_t>, ftk_char_t> utf32Convert;
        LRUCache<GlyphInfo, std::shared_ptr<Glyph> > glyphCache;
    };

    FontSystem::FontSystem(const std::shared_ptr<Context>& context) :
        ISystem(context, "ftk::FontSystem"),
        _p(new Private)
    {
        FTK_P();

        p.fontData[getDefaultFont(FontType::Regular)] = ftk_resource::NotoSans_Regular;
        p.fontData[getDefaultFont(FontType::Bold)] = ftk_resource::NotoSans_Bold;
        p.fontData[getDefaultFont(FontType::Mono)] = ftk_resource::NotoMono_Regular;
        p.fontData[getDefaultFont(FontType::Symbols)] = ftk_resource::NotoSansSymbols2_Regular;

        if (FT_Init_FreeType(&p.ftLibrary))
        {
            auto logSystem = context->getSystem<LogSystem>();
            logSystem->print(
                "ftk::FontSystem",
                "FreeType cannot be initialized",
                LogType::Error);
        }

        p.fonts = ObservableList<std::string>::create();
        p.glyphCacheSize = Observable<size_t>::create();
        p.glyphCachePercentage = Observable<float>::create();

        if (p.ftLibrary)
        {
            for (const auto& i : p.fontData)
            {
                if (FT_New_Memory_Face(
                    p.ftLibrary,
                    i.second.data(),
                    i.second.size(),
                    0,
                    &p.faces[i.first]))
                {
                    auto logSystem = context->getSystem<LogSystem>();
                    logSystem->print(
                        "ftk::FontSystem",
                        Format("Cannot create font: \"{0}\"").arg(i.first),
                        LogType::Error);
                    continue;
                }
                p.fonts->pushBack(i.first);
            }
        }
    }

    FontSystem::~FontSystem()
    {
        FTK_P();
        if (p.ftLibrary)
        {
            for (const auto& i : p.faces)
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

    std::vector<std::string> FontSystem::getFonts() const
    {
        return _p->fonts->get();
    }

    std::shared_ptr<IObservableList<std::string> > FontSystem::observeFonts() const
    {
        return _p->fonts;
    }

    bool FontSystem::addFont(const std::string& name, const std::string& fileName)
    {
        bool out = false;
        try
        {
            auto io = FileIO::create(fileName, FileMode::Read);
            std::vector<uint8_t> data(io->getSize());
            io->read(data.data(), io->getSize());
            addFont(name, data.data(), data.size());
        }
        catch (const std::exception&)
        {}
        return out;
    }

    bool FontSystem::addFont(const std::string& name, const uint8_t* data, size_t size)
    {
        FTK_P();
        bool out = false;
        {
            std::unique_lock<std::mutex> lock(p.mutex);
            p.fontData[name].resize(size);
            memcpy(p.fontData[name].data(), data, size);
            if (0 == FT_New_Memory_Face(
                p.ftLibrary,
                p.fontData[name].data(),
                size,
                0,
                &p.faces[name]))
            {
                out = true;
                p.fonts->pushBack(name);
            }
        }
        return out;
    }

    void FontSystem::removeFont(const std::string& name)
    {
        FTK_P();
        {
            std::unique_lock<std::mutex> lock(p.mutex);
            auto i = p.fontData.find(name);
            if (i != p.fontData.end())
            {
                p.fontData.erase(i);
            }
        }
        const size_t j = p.fonts->indexOf(name);
        if (j != ObservableListInvalidIndex)
        {
            p.fonts->removeItem(j);
        }
    }

    size_t FontSystem::getGlyphCacheSize() const
    {
        return _p->glyphCacheSize->get();
    }

    float FontSystem::getGlyphCachePercentage() const
    {
        return _p->glyphCachePercentage->get();
    }

    std::shared_ptr<IObservable<size_t> > FontSystem::observeGlyphCacheSize() const
    {
        return _p->glyphCacheSize;
    }

    std::shared_ptr<IObservable<float> > FontSystem::observeGlyphCachePercentage() const
    {
        return _p->glyphCachePercentage;
    }

    FontMetrics FontSystem::getMetrics(const FontInfo& info)
    {
        FTK_P();
        FontMetrics out;
        {
            std::unique_lock<std::mutex> lock(p.mutex);
            const auto faceIt = p.faces.find(info.name);
            if (faceIt != p.faces.end())
            {
                FT_Set_Pixel_Sizes(faceIt->second, 0, info.size);
                out.ascender = faceIt->second->size->metrics.ascender / 64;
                out.descender = faceIt->second->size->metrics.descender / 64;
                out.lineHeight = faceIt->second->size->metrics.height / 64;
            }
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
        {
            std::unique_lock<std::mutex> lock(p.mutex);
            const auto utf32 = p.utf32Convert.from_bytes(text);
            p.measure(utf32, fontInfo, maxLineWidth, out);
        }
        return out;
    }

    std::vector<Box2I> FontSystem::getBoxes(
        const std::string& text,
        const FontInfo& fontInfo,
        int maxLineWidth)
    {
        FTK_P();
        std::vector<Box2I> out;
        {
            std::unique_lock<std::mutex> lock(p.mutex);
            const auto utf32 = p.utf32Convert.from_bytes(text);
            Size2I size;
            p.measure(utf32, fontInfo, maxLineWidth, size, &out);
        }
        return out;
    }

    std::vector<std::shared_ptr<Glyph> > FontSystem::getGlyphs(
        const std::string& text,
        const FontInfo& fontInfo)
    {
        FTK_P();
        std::vector<std::shared_ptr<Glyph> > out;
        {
            std::unique_lock<std::mutex> lock(p.mutex);
            const auto utf32 = p.utf32Convert.from_bytes(text);
            for (const auto& i : utf32)
            {
                out.push_back(p.getGlyph(i, fontInfo));
            }
        }
        return out;
    }

    std::shared_ptr<Glyph> FontSystem::Private::getGlyph(uint32_t code, const FontInfo& fontInfo)
    {
        std::shared_ptr<Glyph> out;
        do
        {
            if (glyphCache.get(GlyphInfo(code, fontInfo), out))
            {
                break;
            }

            out = std::make_shared<Glyph>();
            out->info = GlyphInfo(code, fontInfo);

            const auto faceIt = faces.find(fontInfo.name);
            if (faceIt == faces.end())
            {
                break;
            }
            if (FT_Set_Pixel_Sizes(
                faceIt->second,
                0,
                static_cast<int>(fontInfo.size)))
            {
                break;
            }
            const FT_UInt ftGlyphIndex = FT_Get_Char_Index(faceIt->second, code);
            if (!ftGlyphIndex)
            {
                break;
            }
            if (FT_Load_Glyph(faceIt->second, ftGlyphIndex, FT_LOAD_FORCE_AUTOHINT))
            {
                break;
            }
            const FT_Render_Mode renderMode = FT_RENDER_MODE_NORMAL;
            const uint8_t renderModeChannels = 1;
            if (FT_Render_Glyph(faceIt->second->glyph, renderMode))
            {
                break;
            }

            const FT_Bitmap ftBitmap = faceIt->second->glyph->bitmap;
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
            out->offset = V2I(faceIt->second->glyph->bitmap_left, faceIt->second->glyph->bitmap_top);
            out->advance = faceIt->second->glyph->advance.x / 64;
            out->lsbDelta = faceIt->second->glyph->lsb_delta;
            out->rsbDelta = faceIt->second->glyph->rsb_delta;

            glyphCache.add(out->info, out);

        } while (0);

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
        const auto faceIt = faces.find(fontInfo.name);
        if (faceIt == faces.end())
        {
            return;
        }
        if (FT_Set_Pixel_Sizes(
            faceIt->second,
            0,
            static_cast<int>(fontInfo.size)))
        {
            return;
        }

        const int h = faceIt->second->size->metrics.height / 64;
        V2I pos(0, h);
        auto textLineIt = utf32.end();
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
                if (textLineIt != utf32.end())
                {
                    utf32It = textLineIt;
                    textLineIt = utf32.end();
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
                    textLineIt = utf32It;
                    textLineX = pos.x;
                }
                pos.x += x;
            }
        }

        size.w = std::max(size.w, pos.x);
        size.h = pos.y;
    }

    void FontSystem::tick()
    {
        FTK_P();
        size_t glyphCacheSize = 0;
        float glyphCachePercentage = 0.F;
        {
            std::unique_lock<std::mutex> lock(p.mutex);
            glyphCacheSize = p.glyphCache.getCount();
            glyphCachePercentage = p.glyphCache.getPercentage();
        }
        p.glyphCacheSize->setIfChanged(glyphCacheSize);
        p.glyphCachePercentage->setIfChanged(glyphCachePercentage);
    }

    std::chrono::milliseconds FontSystem::getTickTime() const
    {
        return std::chrono::seconds(1);
    }

    void to_json(nlohmann::json& json, const FontInfo& value)
    {
        json["Name"] = value.name;
        json["Size"] = value.size;
    }

    void from_json(const nlohmann::json& json, FontInfo& value)
    {
        json.at("Name").get_to(value.name);
        json.at("Size").get_to(value.size);
    }

    std::ostream& operator << (std::ostream& os, const FontInfo& value)
    {
        os << value.name << ":" << value.size;
        return os;
    }
}