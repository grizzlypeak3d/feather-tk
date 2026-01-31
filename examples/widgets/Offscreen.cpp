// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Offscreen.h"

#include <ftk/GL/GL.h>
#include <ftk/GL/Mesh.h>

#include <ftk/Core/Format.h>
#include <ftk/Core/Matrix.h>
#include <ftk/Core/RenderUtil.h>

using namespace ftk;

namespace widgets
{
    void Offscreen::_init(const std::shared_ptr<Context>& context)
    {
        ftk::IWidget::_init(context, "Offscreen", nullptr);

        _timer = Timer::create(context);
        _timer->setRepeating(true);
        _timer->start(
            std::chrono::microseconds(16),
            [this]
            {
                _rotation += .1F;
                _doRender = true;
                setDrawUpdate();
            });
    }

    Offscreen::~Offscreen()
    {}

    std::shared_ptr<Offscreen> Offscreen::create(const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<Offscreen>(new Offscreen);
        out->_init(context);
        return out;
    }

    std::string vertexSource()
    {
        return
            "#version 410\n"
            "\n"
            "layout(location = 0) in vec3 vPos;\n"
            "layout(location = 1) in vec4 vColor;\n"
            "out vec4 fColor;\n"
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
            "    fColor = vColor;\n"
            "}\n";
    }

    std::string meshFragmentSource()
    {
        return
            "#version 410\n"
            "\n"
            "in vec4 fColor;\n"
            "out vec4 outColor;\n"
            "\n"
            "uniform vec4 color;\n"
            "\n"
            "void main()\n"
            "{\n"
            "\n"
            "    outColor = fColor * color;\n"
            "}\n";
    }

    void Offscreen::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        IWidget::drawEvent(drawRect, event);
        const Box2I& g = getGeometry();
        try
        {
            if (!_shader)
            {
                _shader = gl::Shader::create(
                    vertexSource(),
                    meshFragmentSource());
            }

            const Size2I size = g.size();
            gl::OffscreenBufferOptions offscreenBufferOptions;
#if defined(FTK_API_GL_4_1)
            offscreenBufferOptions.depth = gl::OffscreenDepth::_24;
            offscreenBufferOptions.stencil = gl::OffscreenStencil::_8;
#elif defined(FTK_API_GLES_2)
            offscreenBufferOptions.stencil = gl::OffscreenStencil::_8;
#endif // FTK_API_GL_4_1
            if (gl::doCreate(_buffer, size, gl::offscreenColorDefault, offscreenBufferOptions))
            {
                _buffer = gl::OffscreenBuffer::create(
                    size,
                    gl::offscreenColorDefault,
                    offscreenBufferOptions);
            }

            if (_doRender && _buffer)
            {
                _doRender = false;

                const ViewportState viewportState(event.render);
                const ClipRectEnabledState clipRectEnabledState(event.render);
                const ClipRectState clipRectState(event.render);
                const TransformState transformState(event.render);
                const RenderSizeState renderSizeState(event.render);

                gl::OffscreenBufferBinding binding(_buffer);
                event.render->setRenderSize(size);
                event.render->setViewport(Box2I(0, 0, g.w(), g.h()));
                event.render->setClipRectEnabled(false);
                event.render->clearViewport(Color4F(0.F, 0.F, 0.F));

                const float aspect = aspectRatio(size);
                const M44F vm =
                    translate(V3F(0.F, 0.F, -10.F)) *
                    rotateZ(_rotation);
                const M44F pm = perspective(60.F, aspect, .1F, 10000.F);

                _shader->bind();
                _shader->setUniform("transform.mvp", pm * vm);
                _shader->setUniform("color", Color4F(1.F, 1.F, 1.F));

                TriMesh3F mesh;
                mesh.v.push_back(V3F(-5.F, -5.F, 0.F));
                mesh.v.push_back(V3F(5.F, -5.F, 0.F));
                mesh.v.push_back(V3F(5.F, 5.F, 0.F));
                mesh.v.push_back(V3F(-5.F, 5.F, 0.F));
                mesh.c.push_back(V4F(1.F, 0.F, 0.F));
                mesh.c.push_back(V4F(0.F, 1.F, 0.F));
                mesh.c.push_back(V4F(0.F, 0.F, 1.F));
                mesh.c.push_back(V4F(1.F, 1.F, 1.F));
                Triangle3 triangle;
                triangle.v[0].v = 1;
                triangle.v[1].v = 2;
                triangle.v[2].v = 3;
                triangle.v[0].c = 1;
                triangle.v[1].c = 2;
                triangle.v[2].c = 3;
                mesh.triangles.push_back(triangle);
                triangle.v[0].v = 3;
                triangle.v[1].v = 4;
                triangle.v[2].v = 1;
                triangle.v[0].c = 3;
                triangle.v[1].c = 4;
                triangle.v[2].c = 1;
                mesh.triangles.push_back(triangle);

                auto vbo = gl::VBO::create(mesh.triangles.size() * 3, gl::VBOType::Pos3_F32_Color_U8);
                vbo->copy(convert(mesh, vbo->getType()));
                auto vao = gl::VAO::create(vbo->getType(), vbo->getID());
                vao->bind();
                vao->draw(GL_TRIANGLES, 0, vbo->getSize());
            }
        }
        catch (const std::exception& e)
        {
            if (auto context = getContext())
            {
                context->log("OffscreenWidget", e.what(), LogType::Error);
            }
        }

        if (_buffer)
        {
            const unsigned int id = _buffer->getColorID();
            event.render->drawTexture(id, g);
        }
    }
}
