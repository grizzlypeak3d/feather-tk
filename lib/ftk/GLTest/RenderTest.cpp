// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GLTest/RenderTest.h>

#include <ftk/GL/OffscreenBuffer.h>
#include <ftk/GL/Render.h>
#include <ftk/GL/Texture.h>
#include <ftk/GL/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/FontSystem.h>

#include <array>
#include <cstring>

using namespace ftk::gl;

namespace ftk
{
    namespace gl_test
    {
        RenderTest::RenderTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::gl_test::RenderTest")
        {}

        RenderTest::~RenderTest()
        {}

        std::shared_ptr<RenderTest> RenderTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<RenderTest>(new RenderTest(context));
        }
        
        namespace
        {
            std::shared_ptr<Window> createWindow(
                const std::shared_ptr<Context>& context)
            {
                return Window::create(
                    context,
                    "RenderTest",
                    Size2I(100, 100),
                    static_cast<int>(WindowOptions::MakeCurrent));
            }
        }
        
        void RenderTest::run()
        {
            auto logSystem = _context->getLogSystem();
            auto fontSystem = _context->getSystem<FontSystem>();
            {
                auto window = createWindow(_context);
                Size2I size(1920, 1080);
                auto buffer = OffscreenBuffer::create(size);
                OffscreenBufferBinding bufferBinding(buffer);

                auto render = Render::create(logSystem, fontSystem);
                render->begin(size);
                FTK_CHECK(render->getRenderSize() == size);

                size = Size2I(1280, 960);
                render->setRenderSize(size);
                FTK_CHECK(render->getRenderSize() == size);
                
                Box2I viewport(0, 0, 100, 100);
                render->setViewport(viewport);
                FTK_CHECK(render->getViewport() == viewport);
                
                render->setClipRectEnabled(true);
                FTK_CHECK(render->getClipRectEnabled());
                Box2I clipRect(0, 0, 50, 50);
                render->setClipRect(clipRect);
                FTK_CHECK(clipRect == render->getClipRect());
                render->setClipRectEnabled(false);
                
                M44F transform = perspective(60.F, 1.F, .1F, 10000.F);
                render->setTransform(transform);
                FTK_CHECK(transform == render->getTransform());

                render->end();
            }
            {
                auto window = createWindow(_context);
                Size2I size(1920, 1080);
                auto buffer = OffscreenBuffer::create(size);
                OffscreenBufferBinding bufferBinding(buffer);

                auto render = Render::create(logSystem, fontSystem);
                render->begin(size);
                
                Box2F box(0, 0, size.w, size.h);
                render->drawRect(box, Color4F(1.F, 0.F, 0.F, 1.F));

                const auto& min = box.min;
                const auto& max = box.max;
                render->drawLine(min, max, Color4F(0.F, 1.F, 0.F, 1.F));

                {
                    TriMesh2F mesh;
                    mesh.v.push_back(V2F(min.x, min.y));
                    mesh.v.push_back(V2F(max.x, min.y));
                    mesh.v.push_back(V2F(max.x, max.y));
                    mesh.v.push_back(V2F(min.x, max.y));
                    Triangle2 triangle;
                    triangle.v[0].v = 1;
                    triangle.v[1].v = 3;
                    triangle.v[2].v = 2;
                    mesh.triangles.push_back(triangle);
                    triangle.v[0].v = 3;
                    triangle.v[1].v = 1;
                    triangle.v[2].v = 4;
                    mesh.triangles.push_back(triangle);
                    render->drawMesh(mesh);
                }
                
                {
                    TriMesh2F mesh;
                    mesh.v.push_back(V2F(min.x, min.y));
                    mesh.v.push_back(V2F(max.x, min.y));
                    mesh.v.push_back(V2F(max.x, max.y));
                    mesh.v.push_back(V2F(min.x, max.y));
                    mesh.c.push_back(V4F(0.F, 0.F, 0.F, 1.F));
                    mesh.c.push_back(V4F(0.F, 0.F, 0.F, 1.F));
                    mesh.c.push_back(V4F(1.F, 1.F, 1.F, 1.F));
                    mesh.c.push_back(V4F(1.F, 1.F, 1.F, 1.F));
                    Triangle2 triangle;
                    triangle.v[0].v = 1;
                    triangle.v[1].v = 3;
                    triangle.v[2].v = 2;
                    triangle.v[0].c = 1;
                    triangle.v[1].c = 3;
                    triangle.v[2].c = 2;
                    mesh.triangles.push_back(triangle);
                    triangle.v[0].v = 3;
                    triangle.v[1].v = 1;
                    triangle.v[2].v = 4;
                    triangle.v[0].c = 3;
                    triangle.v[1].c = 1;
                    triangle.v[2].c = 4;
                    mesh.triangles.push_back(triangle);
                    render->drawColorMesh(mesh);
                }
                
                std::string text = "Hello world";
                auto fontSystem = _context->getSystem<FontSystem>();
                FontInfo fontInfo;
                auto fontMetrics = fontSystem->getMetrics(fontInfo);
                auto glyphs = fontSystem->getGlyphs(text, fontInfo);
                render->drawText(glyphs, fontMetrics, V2F(100.F, 100.F));
                
                std::vector<ImageOptions> imageOptionsList;
                for (auto i : getInputVideoLevelsEnums())
                {
                    ImageOptions imageOptions;
                    imageOptions.videoLevels = i;
                    imageOptionsList.push_back(imageOptions);
                }
                for (auto i : getAlphaBlendEnums())
                {
                    ImageOptions imageOptions;
                    imageOptions.alphaBlend = i;
                    imageOptionsList.push_back(imageOptions);
                }
                {
                    ImageOptions imageOptions;
                    imageOptions.cache = false;
                    imageOptionsList.push_back(imageOptions);
                }
                {
                    ImageOptions imageOptions;
                    imageOptions.imageFilters.minify = ImageFilter::Nearest;
                    imageOptions.imageFilters.magnify = ImageFilter::Nearest;
                    imageOptionsList.push_back(imageOptions);
                }
                for (const auto& imageSize : { Size2I(64, 64), Size2I(2048, 2048) })
                {
                    for (auto imageType : getImageTypeEnums())
                    {
                        auto image = Image::create(imageSize, imageType);
                        for (const auto& imageOptions : imageOptionsList)
                        {
                            try
                            {
                                render->drawImage(
                                    image,
                                    Box2F(200.F, 200.F, 256.F, 256.F),
                                    Color4F(1.F, 1.F, 1.F, 1.F),
                                    imageOptions);
                            }
                            catch (const std::exception&)
                            {}
                        }
                    }
                }

                render->end();
            }
            {
                // An image whose pixels are a texture draws where it is:
                // a two row texture, green under red, lands in the buffer
                // as it is, and flipped when the image says it is top down.
                auto window = createWindow(_context);
                const Size2I size(16, 16);
                auto buffer = OffscreenBuffer::create(size);
                OffscreenBufferBinding bufferBinding(buffer);
                auto render = Render::create(logSystem, fontSystem);

                const ImageInfo info(2, 2, ImageType::RGBA_U8);
                auto pixels = Image::create(info);
                const uint8_t rows[2][4] = { { 0, 255, 0, 255 }, { 255, 0, 0, 255 } };
                for (int y = 0; y < 2; ++y)
                {
                    for (int x = 0; x < 2; ++x)
                    {
                        memcpy(pixels->getData() + (y * 2 + x) * 4, rows[y], 4);
                    }
                }
                auto texture = Texture::create(info);
                texture->copy(pixels);

                size_t released = 0;
                const auto draw = [&](bool mirrorY)
                {
                    ImageInfo imageInfo = info;
                    imageInfo.layout.mirror.y = mirrorY;
                    auto image = Image::create(
                        imageInfo,
                        texture->getID(),
                        [&released](uintptr_t) { ++released; });
                    render->begin(size);
                    render->clearViewport(Color4F(0.F, 0.F, 0.F, 1.F));
                    ImageOptions imageOptions;
                    imageOptions.imageFilters.minify = ImageFilter::Nearest;
                    imageOptions.imageFilters.magnify = ImageFilter::Nearest;
                    render->drawImage(
                        image,
                        Box2F(0.F, 0.F, 16.F, 16.F),
                        Color4F(1.F, 1.F, 1.F, 1.F),
                        imageOptions);
                    render->end();
                    auto out = buffer->read(ImageInfo(size, ImageType::RGBA_U8));
                    const uint8_t* d = out->getData();
                    const auto at = [d](int x, int y)
                    {
                        const uint8_t* q = d + (y * 16 + x) * 4;
                        return std::array<uint8_t, 3>{ q[0], q[1], q[2] };
                    };
                    return std::make_pair(at(8, 2), at(8, 13));
                };
                const auto plain = draw(false);
                const auto flipped = draw(true);
                FTK_CHECK(plain.first != plain.second);
                FTK_CHECK(plain.first == flipped.second);
                FTK_CHECK(plain.second == flipped.first);
                const std::array<uint8_t, 3> green{ 0, 255, 0 };
                const std::array<uint8_t, 3> red{ 255, 0, 0 };
                FTK_CHECK(plain.first == green || plain.first == red);
                // The draws' texture cache entries hold the images; the
                // renderer going releases them, and the texture is still
                // the owner's.
                render.reset();
                FTK_CHECK(2 == released);
                FTK_CHECK(texture->copy(pixels));
            }
            {
                // Textures used again, from the pool or the cache, take the
                // filters they are drawn with: a black and a white texel
                // magnified, with a hard edge between them and then a ramp.
                auto window = createWindow(_context);
                const Size2I size(16, 16);
                auto buffer = OffscreenBuffer::create(size);
                OffscreenBufferBinding bufferBinding(buffer);
                auto render = Render::create(logSystem, fontSystem);

                auto image = Image::create(2, 1, ImageType::L_U8);
                image->getData()[0] = 0;
                image->getData()[1] = 255;
                const auto draw = [&](ImageFilter filter, bool cache)
                {
                    render->begin(size);
                    render->clearViewport(Color4F(0.F, 0.F, 0.F, 1.F));
                    ImageOptions imageOptions;
                    imageOptions.imageFilters.minify = filter;
                    imageOptions.imageFilters.magnify = filter;
                    imageOptions.cache = cache;
                    render->drawImage(
                        image,
                        Box2F(0.F, 0.F, 16.F, 16.F),
                        Color4F(1.F, 1.F, 1.F, 1.F),
                        imageOptions);
                    render->end();
                    auto out = buffer->read(ImageInfo(size, ImageType::RGBA_U8));
                    return out->getData()[(8 * 16 + 7) * 4];
                };
                for (bool cache : { false, true })
                {
                    FTK_CHECK(0 == draw(ImageFilter::Nearest, cache));
                    const uint8_t linear = draw(ImageFilter::Linear, cache);
                    FTK_CHECK(linear > 0 && linear < 255);
                    FTK_CHECK(0 == draw(ImageFilter::Nearest, cache));
                }

                // The cache copies an image in once: pixels changed in place
                // are drawn when the cache is cleared.
                const size_t count = Render::getTextureCacheCount();
                image->getData()[0] = 255;
                FTK_CHECK(0 == draw(ImageFilter::Nearest, true));
                FTK_CHECK(count == Render::getTextureCacheCount());
                render->clearTextureCache();
                FTK_CHECK(count - 1 == Render::getTextureCacheCount());
                FTK_CHECK(255 == draw(ImageFilter::Nearest, true));
                FTK_CHECK(255 == draw(ImageFilter::Nearest, false));
            }
        }
    }
}
