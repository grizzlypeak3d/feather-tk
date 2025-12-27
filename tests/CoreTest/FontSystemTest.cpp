// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <CoreTest/FontSystemTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/FontSystem.h>
#include <ftk/Core/Format.h>

namespace ftk
{
    namespace core_test
    {
        FontSystemTest::FontSystemTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::FontSystemTest")
        {}

        FontSystemTest::~FontSystemTest()
        {}

        std::shared_ptr<FontSystemTest> FontSystemTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<FontSystemTest>(new FontSystemTest(context));
        }
        
        void FontSystemTest::run()
        {
            _info();
            _size();
            _add();
        }

        void FontSystemTest::_info()
        {
            {
                const FontInfo a;
                const FontInfo b(getFont(Font::Bold), 16);
                FTK_ASSERT(a == a);
                FTK_ASSERT(a != b);
            }
            {
                const GlyphInfo a;
                const FontInfo fontInfo(getFont(Font::Bold), 16);
                const GlyphInfo b(1, fontInfo);
                FTK_ASSERT(a == a);
                FTK_ASSERT(a != b);
                FTK_ASSERT(a < b);
            }
        }

        void FontSystemTest::_size()
        {
            if (auto context = _context.lock())
            {
                auto fontSystem = context->getSystem<FontSystem>();
                for (const auto& font : getFontEnums())
                {
                    const FontInfo info(getFont(font), 12);
                    const FontMetrics metrics = fontSystem->getMetrics(info);
                    _print(Format("{0}: ascender={1}, descender={2}, lineHeight={3}").
                        arg(info).
                        arg(metrics.ascender).
                        arg(metrics.descender).
                        arg(metrics.lineHeight));

                    std::string s = "0123";
                    Size2I size = fontSystem->getSize(s, info);
                    _print(Format("{0} size: {1}").arg(s).arg(size));
                    auto boxes = fontSystem->getBoxes(s, info);
                    auto glyphs = fontSystem->getGlyphs(s, info);

                    s = "abcd";
                    size = fontSystem->getSize(s, info);
                    _print(Format("{0} size: {1}").arg(s).arg(size));
                    boxes = fontSystem->getBoxes(s, info);
                    glyphs = fontSystem->getGlyphs(s, info);

                    s = "Hello\nworld";
                    size = fontSystem->getSize(s, info);
                    _print(Format("{0} size: {1}").arg(s).arg(size));
                    boxes = fontSystem->getBoxes(s, info);
                    glyphs = fontSystem->getGlyphs(s, info);

                    size = fontSystem->getSize(s, info, 1);
                    _print(Format("{0} size: {1}").arg(s).arg(size));
                    boxes = fontSystem->getBoxes(s, info, 1);

                    _print(Format("Glyph cache: {0} {1}%").
                        arg(fontSystem->getGlyphCacheSize()).
                        arg(fontSystem->getGlyphCachePercentage()));
                }
            }
        }

        void FontSystemTest::_add()
        {
            if (auto context = _context.lock())
            {
                auto fontSystem = context->getSystem<FontSystem>();
                FTK_ASSERT(!fontSystem->addFont("", nullptr, 0));
            }
        }
    }
}

