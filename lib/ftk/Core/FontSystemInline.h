// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

namespace ftk
{
    inline FontInfo::FontInfo(FontType type, int size) :
        type(type),
        size(size)
    {}

    inline bool FontInfo::operator == (const FontInfo & other) const
    {
        return type == other.type && size == other.size;
    }

    inline bool FontInfo::operator != (const FontInfo& other) const
    {
        return !(*this == other);
    }

    inline bool FontInfo::operator < (const FontInfo& other) const
    {
        return std::tie(type, size) < std::tie(other.type, other.size);
    }

    inline GlyphInfo::GlyphInfo(uint32_t code, const FontInfo& fontInfo) :
        code(code),
        fontInfo(fontInfo)
    {}

    inline bool GlyphInfo::operator == (const GlyphInfo & other) const
    {
        return code == other.code && fontInfo == other.fontInfo;
    }

    inline bool GlyphInfo::operator != (const GlyphInfo& other) const
    {
        return !(*this == other);
    }

    inline bool GlyphInfo::operator < (const GlyphInfo& other) const
    {
        return std::tie(code, fontInfo) < std::tie(other.code, other.fontInfo);
    }
}
