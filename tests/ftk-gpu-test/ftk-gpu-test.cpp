// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

// The same scene through the OpenGL renderer and the GPU renderer, read
// back and compared. Both are IRender, so the scene is drawn by one
// function that does not know which it has.
//
// ftk-gpu-test [output directory]

#include <ftk/GPU/OffscreenBuffer.h>
#include <ftk/GPU/Present.h>
#include <ftk/GPU/Render.h>
#include <ftk/GPU/Shader.h>
#include <ftk/GPU/System.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/Init.h>
#include <ftk/GL/OffscreenBuffer.h>
#include <ftk/GL/Render.h>
#include <ftk/GL/Window.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/FontSystem.h>
#include <ftk/Core/ImageIO.h>
#include <ftk/Core/LogSystem.h>

#include <SDL3/SDL.h>

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <iostream>

using namespace ftk;

namespace
{
    const Size2I size(800, 480);

    std::shared_ptr<Image> gradient(int w, int h, ImageType type, float phase)
    {
        auto out = Image::create(w, h, type);
        const int channels = getChannelCount(type);
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                const float v[4] =
                {
                    x / static_cast<float>(w - 1),
                    y / static_cast<float>(h - 1),
                    phase,
                    // A hole in the middle, to be seen through.
                    std::hypot(x - w / 2.F, y - h / 2.F) < w / 4.F ? .25F : 1.F
                };
                const float l[2] = { (v[0] + v[1]) / 2.F, v[3] };
                const float* c = channels < 3 ? l : v;
                const size_t i = static_cast<size_t>(y) * w + x;
                switch (type)
                {
                case ImageType::L_U8:
                case ImageType::LA_U8:
                case ImageType::RGB_U8:
                case ImageType::RGBA_U8:
                    for (int k = 0; k < channels; ++k)
                    {
                        out->getData()[i * channels + k] = static_cast<uint8_t>(c[k] * 255.F + .5F);
                    }
                    break;
                case ImageType::RGB_U16:
                case ImageType::RGBA_U16:
                    for (int k = 0; k < channels; ++k)
                    {
                        reinterpret_cast<uint16_t*>(out->getData())[i * channels + k] =
                            static_cast<uint16_t>(c[k] * 65535.F + .5F);
                    }
                    break;
                case ImageType::RGB_F32:
                case ImageType::RGBA_F32:
                    for (int k = 0; k < channels; ++k)
                    {
                        reinterpret_cast<float*>(out->getData())[i * channels + k] = c[k];
                    }
                    break;
                default: break;
                }
            }
        }
        return out;
    }

    std::shared_ptr<Image> yuv(int w, int h)
    {
        auto out = Image::create(w, h, ImageType::YUV_420P_U8);
        uint8_t* p = out->getData();
        for (int y = 0; y < h; ++y)
        {
            for (int x = 0; x < w; ++x)
            {
                *p++ = static_cast<uint8_t>(16 + 219 * x / (w - 1));
            }
        }
        for (int plane = 0; plane < 2; ++plane)
        {
            for (int y = 0; y < h / 2; ++y)
            {
                for (int x = 0; x < w / 2; ++x)
                {
                    *p++ = static_cast<uint8_t>(plane ?
                        (64 + 128 * y / (h / 2 - 1)) :
                        (192 - 128 * x / (w / 2 - 1)));
                }
            }
        }
        return out;
    }

    // Enough of a half float for the values here: positive and in range.
    uint16_t toHalf(float value)
    {
        uint32_t bits = 0;
        std::memcpy(&bits, &value, 4);
        const int exponent = static_cast<int>((bits >> 23) & 0xFF) - 127 + 15;
        if (exponent <= 0)
        {
            return 0;
        }
        return static_cast<uint16_t>((exponent << 10) | ((bits >> 13) & 0x3FF));
    }

    // Half float RGB with a plane for each channel, which is how the
    // OpenEXR reader hands over most of what it reads: three single channel
    // textures, and nothing to widen to four channels.
    std::shared_ptr<Image> planes(int w, int h)
    {
        auto out = Image::create(w, h, ImageType::RGB_F16_P);
        uint16_t* p = reinterpret_cast<uint16_t*>(out->getData());
        for (int plane = 0; plane < 3; ++plane)
        {
            for (int y = 0; y < h; ++y)
            {
                for (int x = 0; x < w; ++x)
                {
                    const float v[3] =
                    {
                        x / static_cast<float>(w - 1),
                        y / static_cast<float>(h - 1),
                        .25F
                    };
                    *p++ = toHalf(v[plane]);
                }
            }
        }
        return out;
    }

    struct Scene
    {
        std::vector<std::shared_ptr<Glyph> > title;
        std::vector<std::shared_ptr<Glyph> > body;
        FontMetrics titleMetrics;
        FontMetrics bodyMetrics;
        std::vector<std::pair<std::string, std::shared_ptr<Image> > > images;
        std::shared_ptr<Image> poolA;
        std::shared_ptr<Image> poolB;
        std::shared_ptr<Image> video;
        std::shared_ptr<Image> planes;
    };

    Scene createScene(const std::shared_ptr<FontSystem>& fontSystem)
    {
        Scene out;
        const FontInfo titleFont(getDefaultFont(FontType::Bold), 28);
        const FontInfo bodyFont(getDefaultFont(FontType::Regular), 14);
        out.title = fontSystem->getGlyphs("One scene, two renderers", titleFont);
        out.titleMetrics = fontSystem->getMetrics(titleFont);
        out.body = fontSystem->getGlyphs(
            "The quick brown fox jumps over the lazy dog. 0123456789\n"
            "Rectangles, lines, meshes, text and images, drawn through IRender.",
            bodyFont);
        out.bodyMetrics = fontSystem->getMetrics(bodyFont);
        for (const ImageType type :
            {
                ImageType::L_U8,
                ImageType::LA_U8,
                ImageType::RGB_U8,
                ImageType::RGBA_U8,
                ImageType::RGB_U16,
                ImageType::RGBA_U16,
                ImageType::RGB_F32,
                ImageType::RGBA_F32
            })
        {
            std::stringstream ss;
            ss << type;
            out.images.push_back(std::make_pair(ss.str(), gradient(96, 64, type, .5F)));
        }
        out.poolA = gradient(96, 64, ImageType::RGB_U8, 0.F);
        out.poolB = gradient(96, 64, ImageType::RGB_U8, 1.F);
        out.video = yuv(96, 64);
        out.planes = planes(96, 64);
        return out;
    }

    void draw(const std::shared_ptr<IRender>& render, const Scene& scene)
    {
        render->drawRect(Box2F(0, 0, size.w, size.h), Color4F(.14F, .15F, .17F));

        // Text.
        render->drawText(scene.title, scene.titleMetrics, V2F(20, 14), Color4F(1.F, 1.F, 1.F));
        render->drawText(scene.body, scene.bodyMetrics, V2F(20, 56), Color4F(.8F, .82F, .85F));

        // Rectangles over one another, and through one another.
        render->drawRect(Box2F(20, 110, 120, 80), Color4F(.85F, .25F, .2F));
        render->drawRect(Box2F(70, 140, 120, 80), Color4F(.2F, .6F, .9F, .6F));
        render->drawRects(
            { Box2F(210, 110, 30, 30), Box2F(250, 110, 30, 30), Box2F(290, 110, 30, 30) },
            Color4F(.95F, .8F, .2F));

        // Lines.
        std::vector<std::pair<V2F, V2F> > lines;
        for (int i = 0; i < 8; ++i)
        {
            lines.push_back(std::make_pair(V2F(210, 160 + i * 8), V2F(320, 150 + i * 10)));
        }
        render->drawLines(lines, Color4F(.6F, .9F, .6F), LineOptions{ 2.F });

        // Meshes.
        {
            TriMesh2F mesh;
            mesh.v.push_back(V2F(350, 220));
            mesh.v.push_back(V2F(410, 110));
            mesh.v.push_back(V2F(470, 220));
            mesh.triangles.push_back({ 1, 3, 2 });
            render->drawMesh(mesh, Color4F(.7F, .4F, .9F));
        }
        {
            TriMesh2F mesh;
            mesh.v.push_back(V2F(500, 110));
            mesh.v.push_back(V2F(780, 110));
            mesh.v.push_back(V2F(780, 220));
            mesh.v.push_back(V2F(500, 220));
            mesh.c.push_back(V4F(1.F, 0.F, 0.F, 1.F));
            mesh.c.push_back(V4F(0.F, 1.F, 0.F, 1.F));
            mesh.c.push_back(V4F(0.F, 0.F, 1.F, 1.F));
            mesh.c.push_back(V4F(1.F, 1.F, 1.F, 1.F));
            mesh.triangles.push_back({ Vertex2(1, 0, 1), Vertex2(3, 0, 3), Vertex2(2, 0, 2) });
            mesh.triangles.push_back({ Vertex2(3, 0, 3), Vertex2(1, 0, 1), Vertex2(4, 0, 4) });
            // And one that winds the other way, which neither draws.
            mesh.triangles.push_back({ Vertex2(1, 0, 1), Vertex2(2, 0, 2), Vertex2(3, 0, 3) });
            render->drawColorMesh(mesh);
        }

        // Images of each type, over a checker so the alpha shows.
        int x = 20;
        for (const auto& i : scene.images)
        {
            render->drawRect(Box2F(x, 250, 48, 64), Color4F(.5F, .5F, .5F));
            render->drawRect(Box2F(x + 48, 250, 48, 64), Color4F(.3F, .3F, .3F));
            render->drawImage(i.second, Box2F(x, 250, 96, 64));
            x += 96;
        }

        // Two images through one pooled texture in the same frame: each
        // has to come out as itself.
        ImageOptions pooled;
        pooled.cache = false;
        render->drawImage(scene.poolA, Box2F(20, 340, 96, 64), Color4F(1.F, 1.F, 1.F), pooled);
        render->drawImage(scene.poolB, Box2F(126, 340, 96, 64), Color4F(1.F, 1.F, 1.F), pooled);

        // Video, enlarged, and a channel of it.
        render->drawImage(scene.video, Box2F(240, 340, 192, 128));
        ImageOptions red;
        red.channelDisplay = ChannelDisplay::Red;
        render->drawImage(scene.video, Box2F(442, 340, 96, 64), Color4F(1.F, 1.F, 1.F), red);

        render->drawImage(scene.planes, Box2F(442, 410, 96, 64));

        // Clipping.
        render->setClipRectEnabled(true);
        render->setClipRect(Box2I(560, 340, 150, 60));
        render->drawRect(Box2F(540, 320, 300, 200), Color4F(.2F, .3F, .2F));
        render->drawText(scene.title, scene.titleMetrics, V2F(545, 350), Color4F(1.F, .9F, .5F));
        render->setClipRectEnabled(false);
    }

    void write(
        const std::shared_ptr<Context>& context,
        const std::filesystem::path& path,
        const std::shared_ptr<Image>& image)
    {
        auto io = context->getSystem<ImageIO>();
        if (auto writer = io->write(path, image->getInfo()))
        {
            writer->write(image);
        }
    }
}

namespace
{
    // Two image files against one another: screenshots of an application
    // taken with each renderer.
    int compare(
        const std::shared_ptr<Context>& context,
        const std::filesystem::path& a,
        const std::filesystem::path& b,
        const std::filesystem::path& diff)
    {
        auto io = context->getSystem<ImageIO>();
        const auto imageA = io->read(a)->read();
        const auto imageB = io->read(b)->read();
        if (imageA->getInfo().size != imageB->getInfo().size ||
            imageA->getType() != imageB->getType())
        {
            std::cout << "Different sizes or types" << std::endl;
            return 1;
        }
        size_t differing = 0;
        size_t far = 0;
        int max = 0;
        const size_t count = imageA->getByteCount();
        auto diffImage = Image::create(imageA->getInfo());
        const int channels = getChannelCount(imageA->getType());
        for (size_t i = 0; i < count; ++i)
        {
            const int d = std::abs(
                static_cast<int>(imageA->getData()[i]) -
                static_cast<int>(imageB->getData()[i]));
            max = std::max(max, d);
            differing += d > 2;
            far += d > 32;
            diffImage->getData()[i] = 4 == channels && 3 == (i % 4) ?
                255 :
                static_cast<uint8_t>(std::min(255, d * 8));
        }
        if (!diff.empty())
        {
            if (auto writer = io->write(diff, diffImage->getInfo()))
            {
                writer->write(diffImage);
            }
        }
        std::cout << a.filename().string() << ": " << imageA->getInfo().size <<
            ", max " << max <<
            ", off by more than 2: " << 100.0 * differing / count << "%" <<
            ", by more than 32: " << far << std::endl;
        return 0 == far ? 0 : 1;
    }
}

namespace
{
    // What goes to a window, for each kind of swapchain, checked against
    // the same arithmetic done here.
    bool present(const std::shared_ptr<gpu::System>& system)
    {
        const Size2I size(16, 16);
        const float in[3] = { .5F, 1.F, 2.F };
        const float sdrWhiteLevel = 203.F / 80.F;

        auto source = gpu::OffscreenBuffer::create(system, size, gpu::BufferType::RGBA_F32);
        auto render = gpu::Render::create(system, nullptr, nullptr);
        render->setTarget(source);
        RenderOptions options;
        options.clearColor = Color4F(in[0], in[1], in[2], 1.F);
        render->begin(size, options);
        render->end();

        const auto toLinear = [](float v)
        {
            return v <= .04045F ? v / 12.92F : std::pow((v + .055F) / 1.055F, 2.4F);
        };
        const auto toPQ = [](float nits)
        {
            const float m1 = .1593017578125F;
            const float m2 = 78.84375F;
            const float c1 = .8359375F;
            const float c2 = 18.8515625F;
            const float c3 = 18.6875F;
            const float y = std::pow(std::min(std::max(nits / 10000.F, 0.F), 1.F), m1);
            return std::pow((c1 + c2 * y) / (1.F + c3 * y), m2);
        };
        const float l[3] = { toLinear(in[0]), toLinear(in[1]), toLinear(in[2]) };
        const float m[3][3] =
        {
            { .627404F, .329283F, .043313F },
            { .069097F, .919540F, .011362F },
            { .016391F, .088013F, .895595F }
        };

        bool out = true;
        auto presenter = gpu::Present::create(system);
        for (const auto composition :
            {
                gpu::Composition::SDR,
                gpu::Composition::HDRExtendedLinear,
                gpu::Composition::HDR10
            })
        {
            float expected[3] = { in[0], in[1], in[2] };
            for (int i = 0; i < 3; ++i)
            {
                if (gpu::Composition::HDRExtendedLinear == composition)
                {
                    expected[i] = l[i] * sdrWhiteLevel;
                }
                else if (gpu::Composition::HDR10 == composition)
                {
                    expected[i] = toPQ(
                        (m[i][0] * l[0] + m[i][1] * l[1] + m[i][2] * l[2]) * sdrWhiteLevel * 80.F);
                }
            }
            auto destination = gpu::OffscreenBuffer::create(system, size, gpu::BufferType::RGBA_F32);
            SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(system->getDevice());
            presenter->draw(
                cmd,
                source->getTexture(),
                destination->getTexture(),
                static_cast<int>(SDL_GPU_TEXTUREFORMAT_R32G32B32A32_FLOAT),
                composition,
                sdrWhiteLevel);
            SDL_SubmitGPUCommandBuffer(cmd);
            const auto image = destination->read();
            const float* p = reinterpret_cast<const float*>(image->getData()) + (8 * 16 + 8) * 4;
            float max = 0.F;
            for (int i = 0; i < 3; ++i)
            {
                max = std::max(max, std::fabs(p[i] - expected[i]));
            }
            std::cout << "Present " << gpu::getLabel(composition) << ": " <<
                p[0] << " " << p[1] << " " << p[2] << ", expected " <<
                expected[0] << " " << expected[1] << " " << expected[2] << std::endl;
            out &= max < .001F;
        }
        return out;
    }
}

namespace
{
    // The same, into a real swapchain of each kind: a window that is never
    // shown. Nothing can be read back from one, so this is whether it can
    // be done at all, with the API's validation watching.
    bool swapchain(const std::shared_ptr<gpu::System>& system)
    {
        bool out = true;
        SDL_GPUDevice* device = system->getDevice();
        SDL_Window* window = SDL_CreateWindow(
            "ftk-gpu-test",
            320,
            240,
            SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY |
                ("vulkan" == system->getDriver() ? SDL_WINDOW_VULKAN : 0));
        if (!window || !SDL_ClaimWindowForGPUDevice(device, window))
        {
            std::cout << "Swapchain: " << SDL_GetError() << std::endl;
            return false;
        }
        const Size2I size(320, 240);
        auto source = gpu::OffscreenBuffer::create(system, size, gpu::BufferType::RGBA_F16);
        auto render = gpu::Render::create(system, nullptr, nullptr);
        render->setTarget(source);
        render->begin(size);
        render->drawRect(Box2F(0, 0, 160, 240), Color4F(2.F, 2.F, 2.F));
        render->end();
        auto presenter = gpu::Present::create(system);
        for (const auto composition :
            {
                gpu::Composition::SDR,
                gpu::Composition::HDRExtendedLinear,
                gpu::Composition::HDR10
            })
        {
            const auto got = gpu::setComposition(system, window, composition);
            SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
            SDL_GPUTexture* texture = nullptr;
            Uint32 w = 0;
            Uint32 h = 0;
            const bool acquired =
                SDL_WaitAndAcquireGPUSwapchainTexture(cmd, window, &texture, &w, &h) &&
                texture;
            if (acquired)
            {
                presenter->draw(
                    cmd,
                    source->getTexture(),
                    texture,
                    static_cast<int>(SDL_GetGPUSwapchainTextureFormat(device, window)),
                    got);
            }
            SDL_SubmitGPUCommandBuffer(cmd);
            SDL_WaitForGPUIdle(device);
            std::cout << "Swapchain " << gpu::getLabel(composition) << ": " <<
                (got == composition ? "supported" : "not supported") << ", " <<
                (acquired ? "presented" : "no texture to present to") <<
                " (" << w << "x" << h << ")" << std::endl;
            out &= got == composition && acquired;
        }
        SDL_ReleaseWindowFromGPUDevice(device, window);
        SDL_DestroyWindow(window);
        return out;
    }
}

namespace
{
    // The GLSL compiler, where there is one: that it compiles what is
    // right and says so of what is wrong. The renderer's own shaders are
    // compiled as they are made when FTK_GPU_VALIDATE is set.
    bool glsl()
    {
        if (!gpu::hasGLSLCompiler())
        {
            std::cout << "GLSL: no compiler in this build" << std::endl;
            return true;
        }
        const std::string good =
            "#version 450\n"
            "layout(location = 0) out vec4 outColor;\n"
            "void main() { outColor = vec4(1.0); }\n";
        const std::string bad =
            "#version 450\n"
            "layout(location = 0) out vec4 outColor;\n"
            "void main() { outColor = nothing; }\n";
        bool out = !gpu::compileGLSL(good, gpu::ShaderStage::Fragment).empty();
        bool threw = false;
        try
        {
            gpu::compileGLSL(bad, gpu::ShaderStage::Fragment);
        }
        catch (const std::exception&)
        {
            threw = true;
        }
        out &= threw;
        std::cout << "GLSL: compiler " << (out ? "works" : "does not work") <<
            (gpu::validateGLSL() ? ", and every shader made here is compiled with it" : "") <<
            std::endl;
        return out;
    }
}

int main(int argc, char** argv)
{
    int r = 1;
    try
    {
        if (argc > 3 && std::string(argv[1]) == "-compare")
        {
            return compare(Context::create(), argv[2], argv[3], argc > 4 ? argv[4] : "");
        }
        const std::filesystem::path dir = argc > 1 ? argv[1] : ".";
        auto context = Context::create();
        gl::init(context);
        gpu::init(context);
        auto logSystem = context->getLogSystem();
        auto fontSystem = context->getSystem<FontSystem>();
        const Scene scene = createScene(fontSystem);
        const int frames = 100;

        // OpenGL. The rows come back bottom first.
        ImageInfo info(size, ImageType::RGBA_U8);
        auto glImage = Image::create(info);
        double glMs = 0.0;
        {
            auto window = gl::Window::create(
                context,
                "ftk-gpu-test",
                Size2I(100, 100),
                static_cast<int>(gl::WindowOptions::MakeCurrent));
            auto buffer = gl::OffscreenBuffer::create(size, gl::TextureType::RGBA_U8);
            gl::OffscreenBufferBinding binding(buffer);
            auto render = gl::Render::create(logSystem, fontSystem);
            std::vector<uint8_t> tmp(glImage->getByteCount());
            for (int i = 0; i < frames + 1; ++i)
            {
                // The first frame compiles the shaders and fills the
                // glyph atlas; it is not what a frame costs.
                const auto t0 = std::chrono::steady_clock::now();
                render->begin(size);
                draw(render, scene);
                render->end();
                glFinish();
                if (i > 0)
                {
                    glMs += std::chrono::duration<double, std::milli>(
                        std::chrono::steady_clock::now() - t0).count();
                }
            }
            glPixelStorei(GL_PACK_ALIGNMENT, 1);
            glReadPixels(0, 0, size.w, size.h, GL_RGBA, GL_UNSIGNED_BYTE, tmp.data());
            const size_t row = static_cast<size_t>(size.w) * 4;
            for (int y = 0; y < size.h; ++y)
            {
                std::memcpy(
                    glImage->getData() + y * row,
                    tmp.data() + (size.h - 1 - y) * row,
                    row);
            }
        }

        // The GPU renderer.
        std::shared_ptr<Image> gpuImage;
        double gpuMs = 0.0;
        std::string driver;
        {
            auto system = context->getSystem<gpu::System>();
            driver = system->getDriver();
            auto buffer = gpu::OffscreenBuffer::create(system, size);
            auto render = gpu::Render::create(system, logSystem, fontSystem);
            render->setTarget(buffer);
            for (int i = 0; i < frames + 1; ++i)
            {
                const auto t0 = std::chrono::steady_clock::now();
                render->begin(size);
                draw(render, scene);
                render->end();
                // Until it is drawn, as glFinish() waits above.
                SDL_WaitForGPUIdle(system->getDevice());
                if (i > 0)
                {
                    gpuMs += std::chrono::duration<double, std::milli>(
                        std::chrono::steady_clock::now() - t0).count();
                }
            }
            gpuImage = buffer->read();
        }
        const bool presentOK =
            glsl() &&
            present(context->getSystem<gpu::System>()) &&
            swapchain(context->getSystem<gpu::System>());

        // How far apart they are.
        auto diffImage = Image::create(info);
        size_t differing = 0;
        size_t far = 0;
        int max = 0;
        double sum = 0.0;
        const size_t count = glImage->getByteCount();
        for (size_t i = 0; i < count; ++i)
        {
            const int d = std::abs(
                static_cast<int>(glImage->getData()[i]) -
                static_cast<int>(gpuImage->getData()[i]));
            max = std::max(max, d);
            sum += d;
            differing += d > 2;
            far += d > 32;
            diffImage->getData()[i] = 3 == (i % 4) ? 255 : static_cast<uint8_t>(std::min(255, d * 16));
        }
        if (std::getenv("FTK_GPU_TEST_PROBE"))
        {
            // A row through the title, to see how the two differ.
            const int y = 30;
            for (int x = 20; x < 60; ++x)
            {
                const uint8_t* a = glImage->getData() + (static_cast<size_t>(y) * size.w + x) * 4;
                const uint8_t* b = gpuImage->getData() + (static_cast<size_t>(y) * size.w + x) * 4;
                std::cout << x << ": " <<
                    int(a[0]) << " " << int(a[1]) << " " << int(a[2]) << " " << int(a[3]) << " | " <<
                    int(b[0]) << " " << int(b[1]) << " " << int(b[2]) << " " << int(b[3]) << std::endl;
            }
        }
        // The writer takes the first row for the top one unless the image
        // says otherwise, and these were compared with it there.
        auto gpuTop = Image::create(info, gpuImage->getData());
        write(context, dir / "gl.png", glImage);
        write(context, dir / "gpu.png", gpuTop);
        write(context, dir / "diff.png", diffImage);

        std::cout << "GPU driver: " << driver << std::endl;
        std::cout << "Frame time: OpenGL " << glMs / frames << "ms, GPU " << gpuMs / frames << "ms" << std::endl;
        std::cout << "Difference: max " << max << ", mean " << sum / count <<
            ", channels off by more than 2: " << differing << " of " << count <<
            " (" << 100.0 * differing / count << "%), by more than 32: " << far << std::endl;
        // The edges of glyphs differ a little, and nothing else does: the
        // OpenGL renderer keeps texture coordinates in sixteen bits, which
        // is a sixteenth of a texel in an atlas this size. With the same
        // done here the two are the same to the bit.
        r = 0 == far && (100.0 * differing / count) < 3.0 && presentOK ? 0 : 1;
        std::cout << (0 == r ? "PASS" : "FAIL") << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cout << "ERROR: " << e.what() << std::endl;
    }
    return r;
}
