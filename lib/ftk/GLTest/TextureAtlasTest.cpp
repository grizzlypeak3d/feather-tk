// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GLTest/TextureAtlasTest.h>

#include <ftk/GL/TextureAtlas.h>
#include <ftk/GL/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

using namespace ftk::gl;

namespace ftk
{
    namespace gl_test
    {
        TextureAtlasTest::TextureAtlasTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::gl_test::TextureAtlasTest")
        {}

        TextureAtlasTest::~TextureAtlasTest()
        {}

        std::shared_ptr<TextureAtlasTest> TextureAtlasTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<TextureAtlasTest>(new TextureAtlasTest(context));
        }
        
        namespace
        {
            std::shared_ptr<Window> createWindow(
                const std::shared_ptr<Context>& context)
            {
                return Window::create(
                    context,
                    "TextureAtlasTest",
                    Size2I(100, 100),
                    static_cast<int>(WindowOptions::MakeCurrent));
            }
            
            std::string format(const TextureAtlasItem& item)
            {
                return Format(
                    "Item: size={0}, u={1}-{2}, v={3}-{4}").
                    arg(item.size).
                    arg(item.u.min()).
                    arg(item.u.max()).
                    arg(item.v.min()).
                    arg(item.v.max());
            }
        }
        
        void TextureAtlasTest::run()
        {
            {
                auto window = createWindow(_context);
                
                auto atlas = TextureAtlas::create(
                    1024,
                    ImageType::L_U8,
                    ImageFilter::Linear,
                    0);
                FTK_ASSERT(1024 == atlas->getSize());
                FTK_ASSERT(ImageType::L_U8 == atlas->getType());
                FTK_ASSERT(atlas->getTexture());
                
                for (size_t i = 0; i < 10; ++i)
                {
                    auto image = Image::create(512, 512, ImageType::L_U8);
                    TextureAtlasItem item;
                    FTK_ASSERT(atlas->addItem(image, item));
                    FTK_ASSERT(atlas->getItem(item.id, item));
                    _print(format(item));
                    _print(Format("Percentage: {0}").arg(atlas->getPercentageUsed()));
                }
            }
        }
    }
}

