// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/GLTest/OffscreenBufferTest.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/OffscreenBuffer.h>
#include <ftk/GL/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

#include <cmath>

using namespace ftk::gl;

namespace ftk
{
    namespace gl_test
    {
        OffscreenBufferTest::OffscreenBufferTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::gl_test::OffscreenBufferTest")
        {}

        OffscreenBufferTest::~OffscreenBufferTest()
        {}

        std::shared_ptr<OffscreenBufferTest> OffscreenBufferTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<OffscreenBufferTest>(new OffscreenBufferTest(context));
        }
        
        namespace
        {
            std::shared_ptr<Window> createWindow(
                const std::shared_ptr<Context>& context)
            {
                return Window::create(
                    context,
                    "OffscreenBufferTest",
                    Size2I(100, 100),
                    static_cast<int>(WindowOptions::MakeCurrent));
            }
        }
        
        void OffscreenBufferTest::run()
        {
            _enums();
            _members();
            _functions();
            _operators();
            _read();
        }
        
        void OffscreenBufferTest::_enums()
        {
            FTK_TEST_ENUM(OffscreenDepth);
            FTK_TEST_ENUM(OffscreenStencil);
            FTK_TEST_ENUM(OffscreenSampling);
        }
        
        void OffscreenBufferTest::_members()
        {
            {
                auto window = createWindow(_context);

                // No color stays no color, and eight bit RGBA is rendered
                // to everywhere.
                FTK_CHECK(TextureType::None == getRenderableType(TextureType::None));
                FTK_CHECK(TextureType::RGBA_U8 == getRenderableType(TextureType::RGBA_U8));

                struct Test
                {
                    Size2I size;
                    TextureType type = TextureType::None;
                    OffscreenBufferOptions options;
                };
                std::vector<Test> testData;
                for (auto type : getTextureTypeEnums())
                {
                    testData.push_back({ Size2I(1920, 1080), type, OffscreenBufferOptions()});
                }
                for (auto depth : getOffscreenDepthEnums())
                {
                    OffscreenBufferOptions options;
                    options.depth = depth;
                    testData.push_back({ Size2I(1920, 1080), getOffscreenColorDefault(), options });
                }
                {
                    OffscreenBufferOptions options;
                    options.stencil = OffscreenStencil::_8;
                    testData.push_back({ Size2I(1920, 1080), getOffscreenColorDefault(), options });
                }
                {
                    OffscreenBufferOptions options;
                    options.stencil = OffscreenStencil::_8;
                    options.depth = OffscreenDepth::_16;
                    testData.push_back({ Size2I(1920, 1080), getOffscreenColorDefault(), options });
                }
                {
                    OffscreenBufferOptions options;
                    options.stencil = OffscreenStencil::_8;
                    options.depth = OffscreenDepth::_24;
                    testData.push_back({ Size2I(1920, 1080), getOffscreenColorDefault(), options });
                }
                {
                    OffscreenBufferOptions options;
                    options.stencil = OffscreenStencil::_8;
                    options.depth = OffscreenDepth::_32;
                    testData.push_back({ Size2I(1920, 1080), getOffscreenColorDefault(), options });
                }
                for (auto sampling : getOffscreenSamplingEnums())
                {
                    OffscreenBufferOptions options;
                    options.sampling = sampling;
                    testData.push_back({ Size2I(1920, 1080), getOffscreenColorDefault(), options });
                }
                for (const auto& test : testData)
                {
                    try
                    {
                        _print(Format("Offscreen buffer: size={0}, color={1}, depth={2}, stencil={3}, sampling={4}").
                            arg(test.size).
                            arg(test.type).
                            arg(test.options.depth).
                            arg(test.options.stencil).
                            arg(test.options.sampling));
                        auto offscreen = OffscreenBuffer::create(test.size, test.type, test.options);
                        FTK_CHECK(test.size == offscreen->getSize());
                        FTK_CHECK(test.size.w == offscreen->getWidth());
                        FTK_CHECK(test.size.h == offscreen->getHeight());
                        // What the driver can render to, which on OpenGL ES
                        // is not every type.
                        FTK_CHECK(getRenderableType(test.type) == offscreen->getType());
                        FTK_CHECK(test.options == offscreen->getOptions());
                        FTK_CHECK(offscreen->getID());
                        if (test.type != TextureType::None)
                        {
                            FTK_CHECK(offscreen->getColorID());
                            offscreen->bind();
                        }
                    }
                    catch (const std::exception& e)
                    {
                        _error(e.what());
                    }
                }
            }
        }
        
        void OffscreenBufferTest::_functions()
        {
            {
                auto window = createWindow(_context);

                std::shared_ptr<OffscreenBuffer> buffer;
                Size2I size(1920, 1080);
                bool create = doCreate(buffer, size);
                FTK_CHECK(create);
                buffer = OffscreenBuffer::create(size);

                size = Size2I(1280, 960);
                create = doCreate(buffer, size);
                FTK_CHECK(create);
                buffer = OffscreenBuffer::create(size);
                
                OffscreenBufferOptions options;
                options.depth = offscreenDepthDefault;
                create = doCreate(buffer, size, getOffscreenColorDefault(), options);
                FTK_CHECK(create);
                buffer = OffscreenBuffer::create(size, getOffscreenColorDefault(), options);
            }
        }
        
        void OffscreenBufferTest::_operators()
        {
            const OffscreenBufferOptions a;
            OffscreenBufferOptions b;
            FTK_CHECK(a == b);
            b.depth = offscreenDepthDefault;
            FTK_CHECK(a != b);
        }

        void OffscreenBufferTest::_read()
        {
            auto window = createWindow(_context);
            auto buffer = OffscreenBuffer::create(Size2I(4, 4), TextureType::RGBA_F32);
            {
                // A color past one, as a float buffer holds it.
                OffscreenBufferBinding binding(buffer);
                glClearColor(.25F, .5F, 1.5F, 1.F);
                glClear(GL_COLOR_BUFFER_BIT);
            }
            // A pixel comes back as it is, from any corner.
            const Color4F pixel = buffer->getPixel(V2I(3, 0));
            FTK_CHECK(std::abs(pixel.r - .25F) < .001F);
            FTK_CHECK(std::abs(pixel.g - .5F) < .001F);
            FTK_CHECK(std::abs(pixel.b - 1.5F) < .001F);
            FTK_CHECK(std::abs(pixel.a - 1.F) < .001F);
            // Read as eight bits, it is clamped and scaled, every row alike.
            FTK_CHECK(OffscreenBuffer::canRead(ImageType::RGBA_U8));
            auto image = buffer->read(ImageInfo(Size2I(4, 4), ImageType::RGBA_U8));
            FTK_CHECK(image.get());
            if (image)
            {
                const uint8_t* d = image->getData();
                for (int i = 0; i < 4 * 4; ++i)
                {
                    FTK_CHECK(std::abs(static_cast<int>(d[i * 4 + 0]) - 64) <= 1);
                    FTK_CHECK(std::abs(static_cast<int>(d[i * 4 + 1]) - 128) <= 1);
                    FTK_CHECK(255 == d[i * 4 + 2]);
                    FTK_CHECK(255 == d[i * 4 + 3]);
                }
            }
            // Read as half floats, the value past one is kept.
            if (OffscreenBuffer::canRead(ImageType::RGBA_F16))
            {
                auto half = buffer->read(ImageInfo(Size2I(4, 4), ImageType::RGBA_F16));
                FTK_CHECK(half.get());
                if (half)
                {
                    const uint16_t* d = reinterpret_cast<const uint16_t*>(half->getData());
                    for (int i = 0; i < 4 * 4; ++i)
                    {
                        FTK_CHECK(0x3400 == d[i * 4 + 0]); // .25
                        FTK_CHECK(0x3800 == d[i * 4 + 1]); // .5
                        FTK_CHECK(0x3E00 == d[i * 4 + 2]); // 1.5
                        FTK_CHECK(0x3C00 == d[i * 4 + 3]); // 1
                    }
                }
            }

            // An image is given out again once nothing else holds it, and
            // not while something does.
            {
                const ImageInfo info(Size2I(4, 4), ImageType::RGBA_U8);
                auto first = buffer->read(info);
                auto second = buffer->read(info);
                FTK_CHECK(first && second && first != second);
                const Image* ptr = first.get();
                first.reset();
                auto third = buffer->read(info);
                FTK_CHECK(third.get() == ptr);
                FTK_CHECK(third != second);
                // Another description is not given one of these. (A size,
                // since not every type reads back on OpenGL ES.)
                auto other = buffer->read(ImageInfo(Size2I(2, 2), ImageType::RGBA_U8));
                FTK_CHECK(other && other != second && other != third);
            }

            // Read into an image of one's own.
            {
                auto own = Image::create(Size2I(4, 4), ImageType::RGBA_U8);
                FTK_CHECK(buffer->read(own));
                const uint8_t* d = own->getData();
                FTK_CHECK(std::abs(static_cast<int>(d[0]) - 64) <= 1);
                FTK_CHECK(std::abs(static_cast<int>(d[1]) - 128) <= 1);
                FTK_CHECK(255 == d[2]);
                FTK_CHECK(255 == d[3]);
                FTK_CHECK(!buffer->read(std::shared_ptr<Image>()));
                FTK_CHECK(!buffer->read(Image::create(Size2I(4, 4), ImageType::YUV_420P_U8)));
            }

            // The frame buffer binding is as it was.
            {
                GLint fbo = -1;
                glGetIntegerv(GL_FRAMEBUFFER_BINDING, &fbo);
                FTK_CHECK(0 == fbo);
            }

            // A type that cannot be read gives nothing.
            FTK_CHECK(!OffscreenBuffer::canRead(ImageType::YUV_420P_U8));
            FTK_CHECK(!buffer->read(ImageInfo(Size2I(4, 4), ImageType::YUV_420P_U8)));
        }
    }
}
