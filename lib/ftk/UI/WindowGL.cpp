// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Window.h>

#include <ftk/UI/IconSystem.h>
#include <ftk/UI/Style.h>
#include <ftk/UI/Util.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/OffscreenBuffer.h>
#include <ftk/GL/System.h>
#include <ftk/GL/Window.h>
#if defined(FTK_API_GLES_2)
#include <ftk/GL/Mesh.h>
#include <ftk/GL/Shader.h>
#endif // FTK_API_GLES_2

#include <ftk/Core/Context.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/LogSystem.h>
#include <ftk/Core/FontSystem.h>

#if defined(FTK_SDL2)
#include <SDL2/SDL.h>
#elif defined(FTK_SDL3)
#include <SDL3/SDL.h>
#endif // FTK_SDL2

#include <codecvt>
#include <locale>

namespace ftk
{
    struct Window::Private
    {
        std::weak_ptr<Context> context;

        int modifiers = 0;
        std::shared_ptr<gl::Window> window;

        std::shared_ptr<gl::OffscreenBuffer> buffer;
        std::shared_ptr<IRender> render;
#if defined(FTK_API_GLES_2)
        std::shared_ptr<gl::Shader> shader;
#endif // FTK_API_GLES_2
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

        p.window = gl::Window::create(
            context,
            title,
            size,
            static_cast<int>(gl::WindowOptions::DoubleBuffer));

        p.render = context->getSystem<gl::System>()->getRenderFactory()->createRender(
            context->getLogSystem(),
            context->getSystem<FontSystem>());

        setVisible(false);
    }

    Window::Window() :
        _p(new Private)
    {}

    Window::~Window()
    {
        FTK_P();
        p.window->makeCurrent();
        p.render.reset();
        p.buffer.reset();
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
    }

    void Window::setFloatOnTop(bool value)
    {
        IWindow::setFloatOnTop(value);
        _p->window->setFloatOnTop(value);
    }

    void Window::setIcon(const std::shared_ptr<Image>& icon)
    {
        _p->window->setIcon(icon);
    }

    std::shared_ptr<Image> Window::screenshot(const Box2I& rect)
    {
        FTK_P();
        std::shared_ptr<Image> out;
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
#if defined(FTK_API_GL_4_1)
                glPixelStorei(GL_PACK_SWAP_BYTES, 0);
#endif // FTK_API_GL_4_1
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

    std::vector<std::pair<std::string, std::string> > Window::getWindowInfo() const
    {
        FTK_P();
        std::vector<std::pair<std::string, std::string> > out;
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
    
    void Window::_update(
        const std::shared_ptr<FontSystem>& fontSystem,
        const std::shared_ptr<IconSystem>& iconSystem,
        const std::shared_ptr<Style>& style)
    {
        FTK_P();

        const Size2I& frameBufferSize = getFrameBufferSize();
        const float displayScale = getDisplayScale();

        const bool sizeUpdate = _hasSizeUpdate(shared_from_this());
        if (sizeUpdate)
        {
            SizeHintEvent sizeHintEvent(
                fontSystem,
                iconSystem,
                displayScale,
                style);
            _sizeHintEventRecursive(shared_from_this(), sizeHintEvent);

            setGeometry(Box2I(V2I(), frameBufferSize));

            _clipEventRecursive(
                shared_from_this(),
                getGeometry(),
                !isVisible(false));
        }

        const bool drawUpdate = _hasDrawUpdate(shared_from_this());
        if (drawUpdate || sizeUpdate)
        {
            p.window->makeCurrent();

            if (gl::doCreate(p.buffer, frameBufferSize, getFrameBufferType()))
            {
                p.buffer = gl::OffscreenBuffer::create(frameBufferSize, getFrameBufferType());
            }

            if (p.buffer && (drawUpdate || sizeUpdate))
            {
                gl::OffscreenBufferBinding bufferBinding(p.buffer);
                p.render->begin(frameBufferSize);
                p.render->setClipRectEnabled(true);
                DrawEvent drawEvent(
                    fontSystem,
                    iconSystem,
                    displayScale,
                    style,
                    p.render);
                _drawEventRecursive(
                    shared_from_this(),
                    Box2I(V2I(), frameBufferSize),
                    drawEvent);
                p.render->setClipRectEnabled(false);
                p.render->end();
            }

#if defined(FTK_API_GL_4_1)
            if (p.buffer)
            {
                glBindFramebuffer(
                    GL_READ_FRAMEBUFFER,
                    p.buffer->getID());
                glBlitFramebuffer(
                    0,
                    0,
                    frameBufferSize.w,
                    frameBufferSize.h,
                    0,
                    0,
                    frameBufferSize.w,
                    frameBufferSize.h,
                    GL_COLOR_BUFFER_BIT,
                    GL_LINEAR);
            }
#elif defined(FTK_API_GLES_2)
            if (!p.shader)
            {
                try
                {
                    const std::string vertexSource =
                        "precision mediump float;\n"
                        "\n"
                        "attribute vec3 vPos;\n"
                        "attribute vec2 vTexture;\n"
                        "varying vec2 fTexture;\n"
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
                        "precision mediump float;\n"
                        "\n"
                        "varying vec2 fTexture;\n"
                        "\n"
                        "uniform sampler2D textureSampler;\n"
                        "\n"
                        "void main()\n"
                        "{\n"
                        "    gl_FragColor = texture2D(textureSampler, fTexture);\n"
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
            if (p.shader)
            {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                glDisable(GL_BLEND);
                glDisable(GL_SCISSOR_TEST);

                p.shader->bind();
                p.shader->setUniform(
                    "transform.mvp",
                    ortho(
                        0.F,
                        static_cast<float>(frameBufferSize.w),
                        0.F,
                        static_cast<float>(frameBufferSize.h),
                        -1.F,
                        1.F));
                p.shader->setUniform("textureSampler", 0);

                glActiveTexture(static_cast<GLenum>(GL_TEXTURE0));
                glBindTexture(GL_TEXTURE_2D, p.buffer->getColorID());

                auto mesh = ftk::mesh(Box2I(
                    0,
                    0,
                    frameBufferSize.w,
                    frameBufferSize.h));
                auto vboData = gl::convert(
                    mesh,
                    gl::VBOType::Pos2_F32_UV_U16,
                    RangeSizeT(0, mesh.triangles.size() - 1));
                auto vbo = gl::VBO::create(mesh.triangles.size() * 3, gl::VBOType::Pos2_F32_UV_U16);
                vbo->copy(vboData);
                auto vao = gl::VAO::create(gl::VBOType::Pos2_F32_UV_U16, vbo->getID());
                vao->bind();
                vao->draw(GL_TRIANGLES, 0, mesh.triangles.size() * 3);
            }
#endif // FTK_API_GL_4_1

            p.window->swap();
        }
    }

    void Window::_makeCurrent()
    {
        _p->window->makeCurrent();
    }

    void Window::_clearCurrent()
    {
        _p->window->clearCurrent();
    }
}
