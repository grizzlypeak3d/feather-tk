// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/FontSystemTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/FontSystem.h>

#include <ftk_resource/NotoMono-Regular.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
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
                const FontInfo b(getDefaultFont(FontType::Bold), 16);
                FTK_CHECK(a == a);
                FTK_CHECK(a != b);
            }
            {
                const GlyphInfo a;
                const FontInfo fontInfo(getDefaultFont(FontType::Bold), 16);
                const GlyphInfo b(1, fontInfo);
                FTK_CHECK(a == a);
                FTK_CHECK(a != b);
                FTK_CHECK(a < b);
            }
        }

        void FontSystemTest::_size()
        {
            {
                auto fontSystem = _context->getSystem<FontSystem>();
                for (const auto font : getFontTypeEnums())
                {
                    const FontInfo info(getDefaultFont(font), 12);
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
            {
                auto fontSystem = _context->getSystem<FontSystem>();
                FTK_CHECK(!fontSystem->addFont("", nullptr, 0));
            }
            {
                // Added from memory and from a file, added again under the
                // same name, and removed: each leaves the name once in the
                // list, and nothing reads a face whose data is gone.
                auto fontSystem = _context->getSystem<FontSystem>();
                const auto& data = ftk_resource::NotoMono_Regular;
                FTK_CHECK(fontSystem->addFont("TestMono", data.data(), data.size()));
                FTK_CHECK(fontSystem->addFont("TestMono", data.data(), data.size()));
                const auto fonts = fontSystem->getFonts();
                FTK_CHECK(1 == std::count(fonts.begin(), fonts.end(), "TestMono"));
                const FontInfo info("TestMono", 16);
                FTK_CHECK(fontSystem->getSize("Mono", info).w > 0);
                fontSystem->removeFont("TestMono");
                const auto removed = fontSystem->getFonts();
                FTK_CHECK(0 == std::count(removed.begin(), removed.end(), "TestMono"));
                FTK_CHECK(fontSystem->getSize("Mono", info).w > 0);

                const std::filesystem::path path =
                    std::filesystem::temp_directory_path() / "ftk-FontSystemTest.ttf";
                {
                    std::ofstream file(path, std::ios::binary);
                    file.write(reinterpret_cast<const char*>(data.data()), data.size());
                }
                FTK_CHECK(fontSystem->addFont("TestMonoFile", path.string()));
                fontSystem->removeFont("TestMonoFile");
                std::filesystem::remove(path);
            }
        }
    }
}

