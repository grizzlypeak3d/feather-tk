// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <GLTest/RenderTest.h>

#include <ftk/GL/OffscreenBuffer.h>
#include <ftk/GL/Render.h>
#include <ftk/GL/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/FontSystem.h>

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
            
            std::shared_ptr<OffscreenBuffer> createBuffer(const Size2I& size)
            {
                OffscreenBufferOptions options;
                options.color = offscreenColorDefault;
                return OffscreenBuffer::create(size, options);
            }
        }
        
        void RenderTest::run()
        {
            auto context = _context.lock();
            auto logSystem = context->getLogSystem();
            auto fontSystem = context->getSystem<FontSystem>();
            {
                auto window = createWindow(context);
                Size2I size(1920, 1080);
                auto buffer = createBuffer(size);
                OffscreenBufferBinding bufferBinding(buffer);

                auto render = Render::create(logSystem, fontSystem);
                render->begin(size);
                FTK_ASSERT(render->getRenderSize() == size);

                size = Size2I(1280, 960);
                render->setRenderSize(size);
                FTK_ASSERT(render->getRenderSize() == size);
                
                Box2I viewport(0, 0, 100, 100);
                render->setViewport(viewport);
                FTK_ASSERT(render->getViewport() == viewport);
                
                render->setClipRectEnabled(true);
                FTK_ASSERT(render->getClipRectEnabled());
                Box2I clipRect(0, 0, 50, 50);
                render->setClipRect(clipRect);
                FTK_ASSERT(clipRect == render->getClipRect());
                render->setClipRectEnabled(false);
                
                M44F transform = perspective(60.F, 1.F, .1F, 10000.F);
                render->setTransform(transform);
                FTK_ASSERT(transform == render->getTransform());

                render->end();
            }
            if (auto context = _context.lock())
            {
                auto window = createWindow(context);
                Size2I size(1920, 1080);
                auto buffer = createBuffer(size);
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
                auto fontSystem = context->getSystem<FontSystem>();
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
        }
    }
}

