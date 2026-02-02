// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <GLTest/TextureTest.h>

#include <ftk/GL/Texture.h>
#include <ftk/GL/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

using namespace ftk::gl;

namespace ftk
{
    namespace gl_test
    {
        TextureTest::TextureTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::gl_test::TextureTest")
        {}

        TextureTest::~TextureTest()
        {}

        std::shared_ptr<TextureTest> TextureTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<TextureTest>(new TextureTest(context));
        }
        
        namespace
        {
            std::shared_ptr<Window> createWindow(
                const std::shared_ptr<Context>& context)
            {
                return Window::create(
                    context,
                    "TextureTest",
                    Size2I(100, 100),
                    static_cast<int>(WindowOptions::MakeCurrent));
            }
        }
        
        void TextureTest::run()
        {
            _members();
            _functions();
            _operators();
        }
        
        void TextureTest::_members()
        {
            if (auto context = _context.lock())
            {
                auto window = createWindow(context);
                
                struct Data
                {
                    ImageInfo info;
                    TextureOptions options;
                };
                std::vector<Data> dataList;
                for (auto imageType : getImageTypeEnums())
                {
                    dataList.push_back({
                        ImageInfo(1920, 1080, imageType),
                        TextureOptions() });
                }
                {
                    TextureOptions options;
                    options.filters.minify = ImageFilter::Nearest;
                    options.filters.magnify = ImageFilter::Nearest;
                    dataList.push_back({
                        ImageInfo(1920, 1080, ImageType::RGBA_U8),
                        options });
                }
                {
                    TextureOptions options;
                    options.pbo = true;
                    dataList.push_back({
                        ImageInfo(1920, 1080, ImageType::RGBA_U8),
                        options });
                }
                for (const auto& data : dataList)
                {
                    try
                    {
                        _print(Format("Texture: size={0} type={1}, minify={2}, magnify={3}, pbo={4}").
                            arg(data.info.size).
                            arg(data.info.type).
                            arg(data.options.filters.minify).
                            arg(data.options.filters.magnify).
                            arg(data.options.pbo));
                        auto texture = Texture::create(data.info, data.options);
                        FTK_ASSERT(data.info == texture->getImageInfo());
                        FTK_ASSERT(data.info.size == texture->getSize());
                        FTK_ASSERT(data.info.size.w == texture->getWidth());
                        FTK_ASSERT(data.info.size.h == texture->getHeight());
                        FTK_ASSERT(texture->getID());
                        auto image = Image::create(data.info);
                        texture->copy(image);
                        texture->copy(image, 0, 0);
                        texture->copy(image->getData(), image->getInfo());
                        texture->bind();
                    }
                    catch (const std::exception& e)
                    {
                        _error(e.what());
                    }
                }
            }
        }
        
        void TextureTest::_functions()
        {
            if (auto context = _context.lock())
            {
                auto window = createWindow(context);
            }
        }
        
        void TextureTest::_operators()
        {
            {
                const TextureOptions a;
                TextureOptions b;
                FTK_ASSERT(a == b);
                b.pbo = true;
                FTK_ASSERT(a != b);
            }
        }
    }
}

