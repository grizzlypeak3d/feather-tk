// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Window.h>

#include <ftk/UI/App.h>

#include <ftk/UI/IconSystem.h>
#include <ftk/UI/Style.h>
#include <ftk/UI/Util.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/OffscreenBuffer.h>
#include <ftk/GL/System.h>
#include <ftk/GL/Window.h>
#include <ftk/GL/Init.h>
#include <ftk/GL/Mesh.h>
#include <ftk/GL/Shader.h>

#if defined(FTK_GPU)
#include <ftk/GPU/OffscreenBuffer.h>
#include <ftk/GPU/Present.h>
#include <ftk/GPU/Render.h>
#include <ftk/GPU/System.h>
#endif // FTK_GPU

#include <ftk/Core/Context.h>
#include <ftk/Core/DiagSystem.h>

#if defined(FTK_SDL2)
#include <SDL2/SDL.h>
#elif defined(FTK_SDL3)
#include <SDL3/SDL.h>
#endif // FTK_SDL2

#include <cmath>
#include <cstdlib>
#include <cstring>
#include <map>
#include <stdexcept>
#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>
#include <ftk/Core/FontSystem.h>

namespace ftk
{
    struct Window::Private
    {
        std::weak_ptr<Context> context;

        std::shared_ptr<gl::Window> window;

        CursorShape cursor = CursorShape::Arrow;
        std::map<CursorShape, SDL_Cursor*> sdlCursors;

        std::shared_ptr<gl::OffscreenBuffer> buffer;
        std::shared_ptr<IRender> render;
        // What the buffer is drawn to the window with on OpenGL ES, which
        // is asked to copy one frame buffer to another less readily than
        // OpenGL is.
        std::shared_ptr<gl::Shader> shader;
        std::shared_ptr<gl::VBO> vbo;
        std::shared_ptr<gl::VAO> vao;
        Size2I vboSize;
        size_t vboTriangles = 0;

#if defined(FTK_GPU)
        // Drawn with SDL's GPU API instead: the window has no OpenGL
        // context, and what the renderer draws into is copied to the
        // window's swapchain.
        bool gpu = false;
        std::shared_ptr<gpu::System> gpuSystem;
        std::shared_ptr<gpu::OffscreenBuffer> gpuBuffer;
        bool gpuClaimed = false;
        std::shared_ptr<gpu::Present> gpuPresent;
        gpu::Composition gpuComposition = gpu::Composition::SDR;
        // Whether the swapchain follows the display, which it does unless
        // one was asked for by name.
        bool gpuCompositionAuto = true;
#endif // FTK_GPU
    };

    void Window::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::string& title,
        const Size2I& size)
    {
        IWindow::_init(context, app, title);
        FTK_P();

        p.context = context;

        int windowOptions = static_cast<int>(gl::WindowOptions::DoubleBuffer);
#if defined(FTK_GPU)
        p.gpu = gpu::isEnabled();
        if (p.gpu)
        {
            p.gpuSystem = context->getSystem<gpu::System>();
            windowOptions = static_cast<int>(gl::WindowOptions::NoContext);
            if ("vulkan" == p.gpuSystem->getDriver())
            {
                windowOptions |= static_cast<int>(gl::WindowOptions::Vulkan);
            }
        }
#endif // FTK_GPU
        p.window = gl::Window::create(
            context,
            title,
            size,
            windowOptions);
#if defined(FTK_GPU)
        if (p.gpu)
        {
            SDL_GPUDevice* device = p.gpuSystem->getDevice();
            p.gpuClaimed = SDL_ClaimWindowForGPUDevice(device, p.window->getSDLWindow());
            if (!p.gpuClaimed)
            {
                throw std::runtime_error(Format("Cannot claim window: {0}").arg(SDL_GetError()));
            }
            p.gpuPresent = gpu::Present::create(p.gpuSystem);
            p.gpuCompositionAuto = !gpu::hasCompositionRequest();
            p.gpuComposition = gpu::setComposition(
                p.gpuSystem,
                p.window->getSDLWindow(),
                p.gpuCompositionAuto ?
                    gpu::getComposition(p.gpuSystem, p.window->getSDLWindow()) :
                    gpu::getCompositionRequest());
            const SDL_PropertiesID props = SDL_GetWindowProperties(p.window->getSDLWindow());
            context->getSystem<LogSystem>()->print(
                "ftk::Window",
                Format(
                    "Drawing with the GPU renderer: {0}\n"
                    "    * Swapchain: {1}\n"
                    "    * SDR white level: {2}\n"
                    "    * HDR headroom: {3}").
                arg(p.gpuSystem->getDriver()).
                arg(gpu::getLabel(p.gpuComposition)).
                arg(gpu::getSDRWhiteLevel(p.window->getSDLWindow(), p.gpuComposition)).
                arg(SDL_GetFloatProperty(props, SDL_PROP_WINDOW_HDR_HEADROOM_FLOAT, 1.F)));
        }
#endif // FTK_GPU

        // Now that the platform window exists it can say what display
        // scale it really has; _addWindow ran before it existed and could
        // not.
        app->setDisplayScaleFromWindow(p.window->getDisplayScale());
        setDisplayScale(app->observeDisplayScale()->get());

        // The initial sizes, asked for rather than waited for: a window
        // that is never shown -- an offscreen screenshot run -- gets no
        // SHOWN or EXPOSED event to deliver them, and without a buffer
        // size nothing ever draws.
        const Size2I initialSize = p.window->getSize();
        const Size2I initialBufferSize = p.window->getFrameBufferSize();
        if (initialSize.isValid() && initialBufferSize.isValid())
        {
            _setSize(initialSize, initialBufferSize);
        }

#if defined(FTK_GPU)
        if (p.gpu)
        {
            p.render = p.gpuSystem->getRenderFactory()->createRender(
                context->getLogSystem(),
                context->getSystem<FontSystem>());
        }
#endif // FTK_GPU
        if (!p.render)
        {
            p.render = context->getSystem<gl::System>()->getRenderFactory()->createRender(
                context->getLogSystem(),
                context->getSystem<FontSystem>());
        }

        auto diagSystem = context->getSystem<DiagSystem>();
        std::weak_ptr<Window> windowWeak(std::dynamic_pointer_cast<Window>(shared_from_this()));
        diagSystem->addSampler(
            "ftk Frame/Time: {0}ms",
            [windowWeak]
            {
                auto window = windowWeak.lock();
                return window ? window->_p->render->getDiag().time : 0;
            },
            // Sampled in microseconds so the graph keeps its detail, read in
            // milliseconds because that is the unit a frame is thought about
            // in.
            DiagFormat{ 1000.0, 1 });
        diagSystem->addSampler(
            "ftk Frame/Peak: {0}ms",
            [windowWeak]
            {
                auto window = windowWeak.lock();
                return window ? window->_p->render->getDiag().timePeak : 0;
            },
            DiagFormat{ 1000.0, 1 });
        diagSystem->addSampler(
            "ftk Frame/Triangles: {0}",
            [windowWeak]
            {
                auto window = windowWeak.lock();
                return window ? window->_p->render->getDiag().triangles : 0;
            });
        diagSystem->addSampler(
            "ftk Frame/Textures: {0}",
            [windowWeak]
            {
                auto window = windowWeak.lock();
                return window ? window->_p->render->getDiag().textures : 0;
            });
        diagSystem->addSampler(
            "ftk Frame/Glyphs: {0}",
            [windowWeak]
            {
                auto window = windowWeak.lock();
                return window ? window->_p->render->getDiag().glyphs : 0;
            });

        setVisible(false);
    }

    Window::Window() :
        _p(new Private)
    {}

    Window::~Window()
    {
        FTK_P();
        for (const auto& i : p.sdlCursors)
        {
#if defined(FTK_SDL2)
            SDL_FreeCursor(i.second);
#elif defined(FTK_SDL3)
            SDL_DestroyCursor(i.second);
#endif // FTK_SDL2
        }
        // There is no window when making one failed: the application
        // already holds this one by then, and lets go of it like any other.
        if (p.window)
        {
            p.window->makeCurrent();
        }
        p.render.reset();
        p.buffer.reset();
#if defined(FTK_GPU)
        p.gpuBuffer.reset();
        p.gpuPresent.reset();
        if (p.gpuClaimed && p.window)
        {
            SDL_ReleaseWindowFromGPUDevice(p.gpuSystem->getDevice(), p.window->getSDLWindow());
        }
#endif // FTK_GPU
    }

    std::shared_ptr<Window> Window::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::string& title,
        const Size2I& size)
    {
        auto out = std::shared_ptr<Window>(new Window);
        out->_init(context, app, title, size);
        return out;
    }

    uint32_t Window::getID() const
    {
        return _p->window->getID();
    }

    float Window::getSystemDisplayScale() const
    {
        FTK_P();
        return p.window ? p.window->getDisplayScale() : 0.F;
    }

    int Window::getScreen() const
    {
        return _p->window->getScreen();
    }

    void Window::setTitle(const std::string& value)
    {
        IWindow::setTitle(value);
        _p->window->setTitle(value);
    }

    void Window::setSize(const Size2I& value)
    {
        IWindow::setSize(value);
        _p->window->setSize(value);
    }

    void Window::setMinSize(const Size2I& value)
    {
        IWindow::setMinSize(value);
        _p->window->setMinSize(value);
    }

    void Window::setFullScreen(bool value)
    {
        IWindow::setFullScreen(value);
        _p->window->setFullScreen(value);
#if defined(FTK_SDL2)
        // SDL2 does not say when the change is done; it is as done as it
        // will be told.
        _fullScreenFromEvent(value);
#endif // FTK_SDL2
    }

    V2I Window::getPos() const
    {
        return _p->window->getPos();
    }

    void Window::setPos(const V2I& value)
    {
        _p->window->setPos(value);
    }

    bool Window::isMaximized() const
    {
        return _p->window->isMaximized();
    }

    void Window::setMaximized(bool value)
    {
        _p->window->setMaximized(value);
    }

    Box2I Window::getNormalGeometry() const
    {
        return _p->window->getNormalGeometry();
    }

    void Window::_geometryFromEvent(bool maximized)
    {
        _p->window->geometryChanged(maximized);
    }

    void Window::setFloatOnTop(bool value)
    {
        IWindow::setFloatOnTop(value);
        _p->window->setFloatOnTop(value);
    }

    void Window::raise()
    {
        _p->window->raise();
    }

    void Window::setCursor(CursorShape value)
    {
        FTK_P();
        if (value == p.cursor)
            return;
        p.cursor = value;
        // The SDL cursor is process wide, not per window: the shape set
        // last wins wherever the pointer is. Callers set it from the
        // widget the pointer is over, which keeps that honest.
        const auto i = p.sdlCursors.find(value);
        SDL_Cursor* cursor = nullptr;
        if (i != p.sdlCursors.end())
        {
            cursor = i->second;
        }
        else
        {
#if defined(FTK_SDL2)
            SDL_SystemCursor id = SDL_SYSTEM_CURSOR_ARROW;
#elif defined(FTK_SDL3)
            SDL_SystemCursor id = SDL_SYSTEM_CURSOR_DEFAULT;
#endif // FTK_SDL2
            switch (value)
            {
            case CursorShape::Crosshair:
                id = SDL_SYSTEM_CURSOR_CROSSHAIR;
                break;
            default: break;
            }
            cursor = SDL_CreateSystemCursor(id);
            p.sdlCursors[value] = cursor;
        }
        if (cursor)
        {
            SDL_SetCursor(cursor);
        }
    }

    void Window::setTextInput(bool value)
    {
        IWindow::setTextInput(value);
        _p->window->setTextInput(value);
    }

    void Window::setTextInputArea(const Box2I& value)
    {
        _p->window->setTextInputArea(value);
    }

    void Window::setIcon(const std::shared_ptr<Image>& icon)
    {
        _p->window->setIcon(icon);
    }

    std::shared_ptr<Image> Window::screenshot(const Box2I& rect)
    {
        FTK_P();
        std::shared_ptr<Image> out;
#if defined(FTK_GPU)
        if (p.gpuBuffer)
        {
            // Read whole, and handed over the way OpenGL reads: the bottom
            // row first, and the rectangle counted from the bottom.
            const auto image = p.gpuBuffer->readU8();
            Box2I rect2 = rect;
            if (!rect.isValid())
            {
                rect2 = Box2I(V2I(), image->getSize());
            }
            if (rect2.isValid())
            {
                out = Image::create(rect2.w(), rect2.h(), ImageType::RGBA_U8);
                const int h = image->getHeight();
                const size_t srcRow = static_cast<size_t>(image->getWidth()) * 4;
                const size_t dstRow = static_cast<size_t>(rect2.w()) * 4;
                for (int y = 0; y < rect2.h(); ++y)
                {
                    const int srcY = h - 1 - (rect2.y() + y);
                    if (srcY >= 0 && srcY < h)
                    {
                        std::memcpy(
                            out->getData() + y * dstRow,
                            image->getData() + srcY * srcRow + static_cast<size_t>(rect2.x()) * 4,
                            dstRow);
                    }
                }
            }
            return out;
        }
#endif // FTK_GPU
        if (p.buffer)
        {
            Box2I rect2 = rect;
            if (!rect.isValid())
            {
                rect2 = Box2I(V2I(), p.buffer->getSize());
            }
            if (rect2.isValid())
            {
                out = Image::create(rect2.w(), rect2.h(), ImageType::RGBA_U8);
                p.window->makeCurrent();
                gl::OffscreenBufferBinding bufferBinding(p.buffer);
                glPixelStorei(GL_PACK_ALIGNMENT, 1);
                if (!gl::isGLES())
                {
                    glPixelStorei(GL_PACK_SWAP_BYTES, 0);
                }
                glReadPixels(
                    rect2.x(),
                    rect2.y(),
                    rect2.w(),
                    rect2.h(),
                    GL_RGBA,
                    GL_UNSIGNED_BYTE,
                    out->getData());
            }
        }
        return out;
    }

    WindowHDR Window::getHDR() const
    {
        FTK_P();
        WindowHDR out;
#if defined(FTK_GPU)
        if (p.gpu && p.gpuComposition != gpu::Composition::SDR)
        {
            const SDL_PropertiesID props = SDL_GetWindowProperties(p.window->getSDLWindow());
            out.enabled = true;
            out.headroom = SDL_GetFloatProperty(props, SDL_PROP_WINDOW_HDR_HEADROOM_FLOAT, 1.F);
#if !defined(__APPLE__)
            // In units of eighty nits, which is what one is in scRGB. On
            // macOS one is the white of the display, whatever that is set
            // to, and the system keeps its luminance to itself.
            out.whiteNits =
                gpu::getSDRWhiteLevel(p.window->getSDLWindow(), p.gpuComposition) * 80.F;
#endif // __APPLE__
        }
#endif // FTK_GPU
        return out;
    }

    std::vector<std::pair<std::string, std::string> > Window::getWindowInfo() const
    {
        FTK_P();
        std::vector<std::pair<std::string, std::string> > out;
#if defined(FTK_GPU)
        if (p.gpu)
        {
            return p.gpuSystem->getInfo();
        }
#endif // FTK_GPU
        const auto& glInfo = p.window->getGLInfo();
        out.push_back(std::make_pair("GL vendor", glInfo.vendor));
        out.push_back(std::make_pair("GL renderer", glInfo.renderer));
        out.push_back(std::make_pair("GL version", glInfo.version));
        return out;
    }
    
    Size2I Window::getSizeHint() const
    {
        Size2I out;
        for (const auto& child : getChildren())
        {
            const Size2I& childSizeHint = child->getSizeHint();
            out.w = std::max(out.w, childSizeHint.w);
            out.h = std::max(out.h, childSizeHint.h);
        }
        return out;
    }

    void Window::setGeometry(const Box2I& value)
    {
        IWindow::setGeometry(value);
        for (const auto& child : getChildren())
        {
            child->setGeometry(value);
        }
    }

    void Window::setVisible(bool value)
    {
        IWindow::setVisible(value);
        FTK_P();
        // An offscreen window is visible to the widgets -- it ticks, lays out
        // and draws into the buffer a screenshot is read from -- while the
        // platform window it would be shown in stays hidden.
        if (isOffscreen())
        {
            return;
        }
        if (value)
        {
            p.window->show();
        }
        else
        {
            p.window->hide();
        }
    }
    
    void Window::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        IWindow::drawEvent(drawRect, event);
    }
    
    namespace
    {
        gl::TextureType getTextureType(WindowBufferType value)
        {
            gl::TextureType out = gl::TextureType::None;
            switch (value)
            {
            case WindowBufferType::U8: out = gl::TextureType::RGBA_U8; break;
            case WindowBufferType::F16: out = gl::TextureType::RGBA_F16; break;
            case WindowBufferType::F32: out = gl::TextureType::RGBA_F32; break;
            default: break;
            }
            return out;
        }
    }

    void Window::_update(
        const std::shared_ptr<FontSystem>& fontSystem,
        const std::shared_ptr<IconSystem>& iconSystem,
        const std::shared_ptr<Style>& style)
    {
        IWindow::_update(fontSystem, iconSystem, style);
        FTK_P();
#if defined(FTK_GPU)
        if (p.gpu)
        {
            if (_hasDrawUpdate(shared_from_this()))
            {
                _updateGPU(fontSystem, iconSystem, style);
            }
            return;
        }
#endif // FTK_GPU
        if (_hasDrawUpdate(shared_from_this()))
        {
            p.window->makeCurrent();

            const Size2I& bufferSize = getBufferSize();
            const WindowBufferType bufferType = getBufferType();
            const gl::TextureType textureType = getTextureType(bufferType);
            if (gl::doCreate(p.buffer, bufferSize, textureType))
            {
                p.buffer = gl::OffscreenBuffer::create(bufferSize, textureType);
            }
            if (p.buffer)
            {
                gl::OffscreenBufferBinding bufferBinding(p.buffer);
                p.render->begin(bufferSize);
                const Box2I drawRect(V2I(), bufferSize);
                p.render->setClipRectEnabled(false);
                p.render->setClipRect(drawRect);
                DrawEvent drawEvent(
                    fontSystem,
                    iconSystem,
                    getDisplayScale(),
                    style,
                    p.render);
                _drawEventRecursive(
                    shared_from_this(),
                    drawRect,
                    drawEvent);
                p.render->setClipRectEnabled(false);
                p.render->end();
            }

            const bool gles = gl::isGLES();
            if (p.buffer && !gles)
            {
                glBindFramebuffer(
                    GL_READ_FRAMEBUFFER,
                    p.buffer->getID());
                glBlitFramebuffer(
                    0,
                    0,
                    bufferSize.w,
                    bufferSize.h,
                    0,
                    0,
                    bufferSize.w,
                    bufferSize.h,
                    GL_COLOR_BUFFER_BIT,
                    GL_LINEAR);
            }
            if (p.buffer && gles && !p.shader)
            {
                try
                {
                    const std::string vertexSource =
                        "\n"
                        "in vec3 vPos;\n"
                        "in vec2 vTexture;\n"
                        "out vec2 fTexture;\n"
                        "\n"
                        "struct Transform\n"
                        "{\n"
                        "    mat4 mvp;\n"
                        "};\n"
                        "\n"
                        "uniform Transform transform;\n"
                        "\n"
                        "void main()\n"
                        "{\n"
                        "    gl_Position = transform.mvp * vec4(vPos, 1.0);\n"
                        "    fTexture = vTexture;\n"
                        "}\n";
                    const std::string fragmentSource =
                        "out vec4 outColor;\n"
                        "\n"
                        "in vec2 fTexture;\n"
                        "\n"
                        "uniform sampler2D textureSampler;\n"
                        "\n"
                        "void main()\n"
                        "{\n"
                        "    outColor = texture(textureSampler, fTexture);\n"
                        "}\n";
                    p.shader = gl::Shader::create(vertexSource, fragmentSource);
                }
                catch (const std::exception& e)
                {
                    if (auto context = p.context.lock())
                    {
                        context->getSystem<LogSystem>()->print(
                            "ftk::Window",
                            Format("Cannot compile shader: {0}").arg(e.what()),
                            LogType::Error);
                    }
                }
            }
            if (p.buffer && gles && p.shader)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glDisable(GL_BLEND);
                glDisable(GL_SCISSOR_TEST);
                // A present pass has no back faces worth keeping: the
                // winding that survives culling on the desktop is the one
                // ANGLE culls.
                glDisable(GL_CULL_FACE);

                p.shader->bind();
                p.shader->setUniform(
                    "transform.mvp",
                    ortho(
                        0.F,
                        static_cast<float>(bufferSize.w),
                        0.F,
                        static_cast<float>(bufferSize.h),
                        -1.F,
                        1.F));
                p.shader->setUniform("textureSampler", 0);

                glActiveTexture(static_cast<GLenum>(GL_TEXTURE0));
                glBindTexture(GL_TEXTURE_2D, p.buffer->getColorID());

                if (!p.vbo || !p.vao || p.vboSize != bufferSize)
                {
                    const auto mesh = ftk::mesh(Box2I(
                        0,
                        0,
                        bufferSize.w,
                        bufferSize.h));
                    const auto vboData = gl::convert(
                        mesh,
                        gl::VBOType::Pos2_F32_UV_U16,
                        RangeSizeT(0, mesh.triangles.size() - 1));
                    p.vbo = gl::VBO::create(mesh.triangles.size() * 3, gl::VBOType::Pos2_F32_UV_U16);
                    p.vbo->copy(vboData);
                    p.vao = gl::VAO::create(gl::VBOType::Pos2_F32_UV_U16, p.vbo->getID());
                    p.vboSize = bufferSize;
                    p.vboTriangles = mesh.triangles.size();
                }
                p.vao->bind();
                p.vao->draw(GL_TRIANGLES, 0, p.vboTriangles * 3);
            }

            // Presenting to the windowing system, which an offscreen window
            // has nothing to present to. The swap interval is 1, so this waits
            // for the display: skipping it is what lets a headless run go
            // faster than the monitor's refresh rate.
            if (!isOffscreen())
            {
                p.window->swap();
            }
        }
    }

#if defined(FTK_GPU)
    void Window::_updateGPU(
        const std::shared_ptr<FontSystem>& fontSystem,
        const std::shared_ptr<IconSystem>& iconSystem,
        const std::shared_ptr<Style>& style)
    {
        FTK_P();
        // The swapchain follows the display: a window moved to an HDR
        // display, or a display whose HDR is turned on, is given an HDR
        // swapchain, and back again.
        if (p.gpuCompositionAuto && !isOffscreen())
        {
            const gpu::Composition composition = gpu::getComposition(
                p.gpuSystem,
                p.window->getSDLWindow());
            if (composition != p.gpuComposition)
            {
                p.gpuComposition = gpu::setComposition(
                    p.gpuSystem,
                    p.window->getSDLWindow(),
                    composition);
                if (auto context = p.context.lock())
                {
                    context->getSystem<LogSystem>()->print(
                        "ftk::Window",
                        Format("Swapchain: {0}").arg(gpu::getLabel(p.gpuComposition)));
                }
            }
        }

        const Size2I& bufferSize = getBufferSize();
        gpu::BufferType bufferType = gpu::BufferType::RGBA_U8;
        switch (getBufferType())
        {
        case WindowBufferType::F16: bufferType = gpu::BufferType::RGBA_F16; break;
        case WindowBufferType::F32: bufferType = gpu::BufferType::RGBA_F32; break;
        default: break;
        }
        if (bufferSize.isValid() &&
            (!p.gpuBuffer ||
                p.gpuBuffer->getSize() != bufferSize ||
                p.gpuBuffer->getType() != bufferType))
        {
            p.gpuBuffer = gpu::OffscreenBuffer::create(p.gpuSystem, bufferSize, bufferType);
        }
        if (!p.gpuBuffer)
            return;

        // The window's renderer may be one built on the GPU renderer; the
        // target is said to the one that draws.
        auto gpuRender = gpu::getRender(p.render);
        if (!gpuRender)
            return;
        gpuRender->setTarget(p.gpuBuffer);
        const auto& render = p.render;
        render->begin(bufferSize);
        const Box2I drawRect(V2I(), bufferSize);
        render->setClipRectEnabled(false);
        render->setClipRect(drawRect);
        DrawEvent drawEvent(
            fontSystem,
            iconSystem,
            getDisplayScale(),
            style,
            p.render);
        _drawEventRecursive(
            shared_from_this(),
            drawRect,
            drawEvent);
        render->setClipRectEnabled(false);
        if (gpu::getEnvFlag("FTK_GPU_HDR_TEST"))
        {
            // Something to look at on an HDR display: patches at one, two,
            // four and eight times the user interface's white. Display
            // encoded, as everything drawn here is.
            const float h = 40.F * getDisplayScale();
            float x = 0.F;
            for (const float linear : { 1.F, 2.F, 4.F, 8.F })
            {
                const float v = 1.055F * std::pow(linear, 1.F / 2.4F) - .055F;
                render->drawRect(Box2F(x, 0.F, h * 2.F, h), Color4F(v, v, v));
                x += h * 2.F;
            }
        }
        render->end();

        // To the window, which an offscreen one does not have. The
        // swapchain's texture is the window's for this frame only, and
        // submitting the command buffer is what presents it.
        if (!isOffscreen())
        {
            SDL_GPUDevice* device = p.gpuSystem->getDevice();
            SDL_GPUCommandBuffer* cmd = SDL_AcquireGPUCommandBuffer(device);
            SDL_GPUTexture* swapchain = nullptr;
            Uint32 w = 0;
            Uint32 h = 0;
            if (cmd &&
                SDL_WaitAndAcquireGPUSwapchainTexture(cmd, p.window->getSDLWindow(), &swapchain, &w, &h) &&
                swapchain)
            {
                // The white level moves with the display the window is on
                // and with its brightness, so it is asked for each frame.
                SDL_Window* sdlWindow = p.window->getSDLWindow();
                p.gpuPresent->draw(
                    cmd,
                    p.gpuBuffer->getTexture(),
                    swapchain,
                    static_cast<int>(SDL_GetGPUSwapchainTextureFormat(device, sdlWindow)),
                    p.gpuComposition,
                    gpu::getSDRWhiteLevel(sdlWindow, p.gpuComposition));
            }
            if (cmd)
            {
                SDL_SubmitGPUCommandBuffer(cmd);
            }
        }
    }
#endif // FTK_GPU

    void Window::_makeCurrent()
    {
        // There is no window when making one failed, and a window that is
        // let go of then still gets here: an application's makes its
        // context current to take down what it drew with.
        if (_p->window)
        {
            _p->window->makeCurrent();
        }
    }

    void Window::_clearCurrent()
    {
        if (_p->window)
        {
            _p->window->clearCurrent();
        }
    }
}
