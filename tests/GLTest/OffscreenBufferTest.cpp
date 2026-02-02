// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <GLTest/OffscreenBufferTest.h>

#include <ftk/GL/OffscreenBuffer.h>
#include <ftk/GL/Window.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>

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
        }
        
        void OffscreenBufferTest::_enums()
        {
            FTK_TEST_ENUM(OffscreenDepth);
            FTK_TEST_ENUM(OffscreenStencil);
            FTK_TEST_ENUM(OffscreenSampling);
        }
        
        void OffscreenBufferTest::_members()
        {
            if (auto context = _context.lock())
            {
                auto window = createWindow(context);

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
                    testData.push_back({ Size2I(1920, 1080), offscreenColorDefault, options });
                }
                {
                    OffscreenBufferOptions options;
                    options.stencil = OffscreenStencil::_8;
                    testData.push_back({ Size2I(1920, 1080), offscreenColorDefault, options });
                }
                {
                    OffscreenBufferOptions options;
                    options.stencil = OffscreenStencil::_8;
                    options.depth = OffscreenDepth::_16;
                    testData.push_back({ Size2I(1920, 1080), offscreenColorDefault, options });
                }
                {
                    OffscreenBufferOptions options;
                    options.stencil = OffscreenStencil::_8;
                    options.depth = OffscreenDepth::_24;
                    testData.push_back({ Size2I(1920, 1080), offscreenColorDefault, options });
                }
                {
                    OffscreenBufferOptions options;
                    options.stencil = OffscreenStencil::_8;
                    options.depth = OffscreenDepth::_32;
                    testData.push_back({ Size2I(1920, 1080), offscreenColorDefault, options });
                }
                for (auto sampling : getOffscreenSamplingEnums())
                {
                    OffscreenBufferOptions options;
                    options.sampling = sampling;
                    testData.push_back({ Size2I(1920, 1080), offscreenColorDefault, options });
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
                        FTK_ASSERT(test.size == offscreen->getSize());
                        FTK_ASSERT(test.size.w == offscreen->getWidth());
                        FTK_ASSERT(test.size.h == offscreen->getHeight());
                        FTK_ASSERT(test.type == offscreen->getType());
                        FTK_ASSERT(test.options == offscreen->getOptions());
                        FTK_ASSERT(offscreen->getID());
                        if (test.type != TextureType::None)
                        {
                            FTK_ASSERT(offscreen->getColorID());
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
            if (auto context = _context.lock())
            {
                auto window = createWindow(context);

                std::shared_ptr<OffscreenBuffer> buffer;
                Size2I size(1920, 1080);
                bool create = doCreate(buffer, size);
                FTK_ASSERT(create);
                buffer = OffscreenBuffer::create(size);

                size = Size2I(1280, 960);
                create = doCreate(buffer, size);
                FTK_ASSERT(create);
                buffer = OffscreenBuffer::create(size);
                
                OffscreenBufferOptions options;
                options.depth = offscreenDepthDefault;
                create = doCreate(buffer, size, offscreenColorDefault, options);
                FTK_ASSERT(create);
                buffer = OffscreenBuffer::create(size, offscreenColorDefault, options);
            }
        }
        
        void OffscreenBufferTest::_operators()
        {
            const OffscreenBufferOptions a;
            OffscreenBufferOptions b;
            FTK_ASSERT(a == b);
            b.depth = offscreenDepthDefault;
            FTK_ASSERT(a != b);
        }
    }
}

