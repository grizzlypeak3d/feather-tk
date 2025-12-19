// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "ObjView.h"

#include "App.h"
#include "Document.h"
#include "HUDWidget.h"
#include "SettingsModel.h"

#include <ftk/GL/GL.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/Matrix.h>
#include <ftk/Core/RenderUtil.h>
#include <ftk/Core/String.h>

using namespace ftk;

namespace objview
{
    void ObjView::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<Document>& doc,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "examples::objview::ObjView", parent);

        // Initialize the mesh.
        _mesh = doc->getMesh();
        //_mesh = std::make_shared<ftk::TriMesh3F>(sphere(5.F, 64, 64));
        if (_mesh)
        {
            _objectBBox = bbox(*_mesh);
            _objectSize = std::max(std::max(_objectBBox.w(), _objectBBox.h()), _objectBBox.d());
        }

        // Create the HUD widget.
        _hudWidget = HUDWidget::create(context, doc, shared_from_this());
        _hudWidget->setOrbit(_orbit);
        _hudWidget->setDistance(_distance);

        // Observe the object rotation and update the render.
        _rotationObserver = Observer<V3F>::create(
            doc->observeRotation(),
            [this](const V3F& value)
            {
                _rotation = value;
                _doRender = true;
                _gridVbo.reset();
                _gridVao.reset();
                setDrawUpdate();
            });

        // Observe the render settings and update the render.
        _renderSettingsObserver = Observer<RenderSettings>::create(
            app->getSettingsModel()->observeRender(),
            [this](const RenderSettings& value)
            {
                _settings = value;
                _doRender = true;
                setDrawUpdate();
            });
    }

    ObjView::~ObjView()
    {}

    std::shared_ptr<ObjView> ObjView::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<Document>& doc,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ObjView>(new ObjView);
        out->_init(context, app, doc, parent);
        return out;
    }

    void ObjView::setOrbit(const V2F& value)
    {
        V2F tmp;
        tmp.x = value.x;
        while (tmp.x >= 360.F)
        {
            tmp.x -= 360.F;
        }
        while (tmp.x < 0.F)
        {
            tmp.x += 360.F;
        }
        tmp.y = clamp(value.y, -89.F, 89.F);
        if (tmp == _orbit)
            return;
        _orbit = tmp;
        _hudWidget->setOrbit(_orbit);
        _doRender = true;
        setDrawUpdate();
    }

    void ObjView::setDistance(float value)
    {
        const float tmp = std::max(_objectSize * .1F, value);
        if (tmp == _distance)
            return;
        _distance = tmp;
        _hudWidget->setDistance(_distance);
        _doRender = true;
        setDrawUpdate();
    }

    void ObjView::frame()
    {
        const Box3F bbox = _getObjectBBoxTransformed();
        _position = -center(bbox);
        setDistance(_objectSize * 1.25F);
    }

    void ObjView::zoomIn()
    {
        setDistance(_distance * .9F);
    }

    void ObjView::zoomOut()
    {
        setDistance(_distance * 1.1F);
    }

    void ObjView::orbitLeft()
    {
        setOrbit(_orbit + V2F(10.F, 0.F));
    }

    void ObjView::orbitRight()
    {
        setOrbit(_orbit + V2F(-10.F, 0.F));
    }

    void ObjView::orbitUp()
    {
        setOrbit(_orbit + V2F(0.F, 10.F));
    }

    void ObjView::orbitDown()
    {
        setOrbit(_orbit + V2F(0.F, -10.F));
    }

    void ObjView::setGeometry(const ftk::Box2I& value)
    {
        const bool changed = value != getGeometry();
        IWidget::setGeometry(value);
        _doRender |= changed;
        if (_frameInit)
        {
            _frameInit = false;
            frame();
        }
        _hudWidget->setGeometry(value);
    }

    std::string gridVertexSource()
    {
        return
            "#version 410\n"
            "\n"
            "layout(location = 0) in vec3 vPos;\n"
            "layout(location = 1) in vec2 vTex;\n"
            "out vec2 fTex;\n"
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
            "    fTex = vTex;\n"
            "}\n";
    }

    std::string gridFragmentSource()
    {
        return
            "#version 410\n"
            "\n"
            "in vec2 fTex;\n"
            "out vec4 outColor;\n"
            "\n"
            "uniform vec4 color;\n"
            "uniform sampler2D textureSampler;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    outColor = texture(textureSampler, fTex) * color;\n"
            "}\n";
    };

    std::string meshVertexSource()
    {
        return
            "#version 410\n"
            "\n"
            "layout(location = 0) in vec3 vPos;\n"
            "layout(location = 1) in vec2 vTex;\n"
            "layout(location = 2) in vec3 vNorm;\n"
            "layout(location = 3) in vec4 vColor;\n"
            "out vec2 fTex;\n"
            "out vec3 fNorm;\n"
            "out vec4 fColor;\n"
            "\n"
            "struct Transform\n"
            "{\n"
            "    mat4 m;\n"
            "    mat4 mvp;\n"
            "};\n"
            "\n"
            "uniform Transform transform;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    gl_Position = transform.mvp * vec4(vPos, 1.0);\n"
            "    fTex = vTex;\n"
            //"    fNorm = vNorm;\n"
            //! \todo Replace this with a pre-computed uniform variable.
            "    fNorm = mat3(transpose(inverse(transform.m))) * vNorm;\n"
            "    fColor = vColor;\n"
            "}\n";
    }

    std::string meshFragmentSource()
    {
        return
            "#version 410\n"
            "\n"
            "in vec2 fTex;\n"
            "in vec3 fNorm;\n"
            "in vec4 fColor;\n"
            "out vec4 outColor;\n"
            "\n"
            "// enum RenderMode\n"
            "const uint RenderMode_Shaded  = 0;\n"
            "const uint RenderMode_Flat    = 1;\n"
            "const uint RenderMode_Texture = 2;\n"
            "const uint RenderMode_Normals = 3;\n"
            "\n"
            "uniform int renderMode;\n"
            "uniform vec4 color;\n"
            "uniform vec3 lightDir;\n"
            "uniform vec3 ambientLight;\n"
            "\n"
            "void main()\n"
            "{\n"
            "    outColor.r = 0;\n"
            "    outColor.g = 0;\n"
            "    outColor.b = 0;\n"
            "    outColor.a = 0;\n"
            "    if (RenderMode_Shaded == renderMode)\n"
            "    {\n"
            "        vec3 l = normalize(lightDir);\n"
            "        vec3 n = normalize(fNorm);\n"
            "        float d = max(dot(n, l), 0.0);\n"
            "        outColor.r = d * fColor.r * color.r + ambientLight.r;\n"
            "        outColor.g = d * fColor.g * color.g + ambientLight.g;\n"
            "        outColor.b = d * fColor.b * color.b + ambientLight.b;\n"
            "        outColor.a = fColor.a * color.a;\n"
            "    }\n"
            "    else if (RenderMode_Flat == renderMode)\n"
            "    {\n"
            "        outColor = fColor * color;\n"
            "    }\n"
            "    else if (RenderMode_Texture == renderMode)\n"
            "    {\n"
            "        outColor.r = fTex.x;\n"
            "        outColor.g = fTex.y;\n"
            "        outColor.a = 1;\n"
            "    }\n"
            "    else if (RenderMode_Normals == renderMode)\n"
            "    {\n"
            "        outColor.r = fNorm.x;\n"
            "        outColor.g = fNorm.y;\n"
            "        outColor.b = fNorm.z;\n"
            "        outColor.a = 1;\n"
            "    }\n"
            "}\n";
    }

    void ObjView::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        const Box2I& g = getGeometry();

        // Get transforms.
        const M44F model = _getModelTransform();
        const M44F view = _getViewTransform(_distance);
        const M44F projection = _getProjectionTransform();
        const Box3F bbox = _getObjectBBoxTransformed();

        try
        {
            // Create the grid texture.
            if (!_gridBuffer)
            {
                const int gridCells = 10;
                const int gridCell = 100;
                const int gridLine = 4;
                const int axisLine = 10;

                // Create the grid offscreen buffer.
                const Size2I gridSize(
                    gridCell * gridCells + gridLine,
                    gridCell * gridCells + gridLine);
                gl::OffscreenBufferOptions offscreenBufferOptions;
                offscreenBufferOptions.color = ImageType::RGBA_F32;
                _gridBuffer = gl::OffscreenBuffer::create(gridSize, offscreenBufferOptions);
                gl::OffscreenBufferBinding binding(_gridBuffer);

                // Save render state.
                const ViewportState viewportState(event.render);
                const ClipRectEnabledState clipRectEnabledState(event.render);
                const ClipRectState clipRectState(event.render);
                const TransformState transformState(event.render);
                const RenderSizeState renderSizeState(event.render);

                // Setup the camera.
                event.render->setRenderSize(gridSize);
                event.render->setViewport(Box2I(0, 0, gridSize.w, gridSize.h));
                event.render->setClipRectEnabled(false);
                event.render->clearViewport(Color4F(0.F, 0.F, 0.F, 0.F));
                event.render->setTransform(ortho(
                    0.F,
                    static_cast<float>(gridSize.w),
                    static_cast<float>(gridSize.h),
                    0.F,
                    -1.F,
                    1.F));

                // Draw the grid lines.
                std::vector<Box2I> rects;
                for (int x = 0; x < gridSize.w; x += gridCell)
                {
                    rects.push_back(Box2I(x, 0, gridLine, gridSize.h));
                }
                for (int y = 0; y < gridSize.h; y += gridCell)
                {
                    rects.push_back(Box2I(0, y, gridSize.w, gridLine));
                }
                event.render->drawRects(rects, Color4F(.5F, .5F, .5F));

                // Draw the grid axis.
                event.render->drawRect(
                    Box2I(
                        gridCell * gridCells / 2,
                        gridCell * gridCells / 2,
                        gridCell * gridCells / 2,
                        axisLine),
                    Color4F(1.F, 0.F, 0.F));
                event.render->drawRect(
                    Box2I(
                        gridCell * gridCells / 2,
                        gridCell * gridCells / 2,
                        axisLine,
                        gridCell * gridCells / 2),
                    Color4F(0.F, 0.F, 1.F));
            }

            // Crate the grid vertex buffer.
            if (_gridBuffer && !_gridVbo)
            {
                TriMesh3F mesh;
                const float w = _objectSize;
                const float y = -bbox.h() / 2.F;
                mesh.v.push_back(V3F(-w / 2.F, y, -w / 2.F) - _position);
                mesh.v.push_back(V3F( w / 2.F, y, -w / 2.F) - _position);
                mesh.v.push_back(V3F( w / 2.F, y,  w / 2.F) - _position);
                mesh.v.push_back(V3F(-w / 2.F, y,  w / 2.F) - _position);
                mesh.t.push_back(V2F(0.F, 0.F));
                mesh.t.push_back(V2F(1.F, 0.F));
                mesh.t.push_back(V2F(1.F, 1.F));
                mesh.t.push_back(V2F(0.F, 1.F));
                mesh.triangles.push_back({
                    Vertex3(1, 1),
                    Vertex3(3, 3),
                    Vertex3(2, 2) });
                mesh.triangles.push_back({
                    Vertex3(3, 3),
                    Vertex3(1, 1),
                    Vertex3(4, 4) });

                _gridVbo = gl::VBO::create(
                    mesh.triangles.size() * 3,
                    gl::VBOType::Pos3_F32_UV_F32_Normal_F32);
                _gridVbo->copy(gl::convert(mesh, _gridVbo->getType()));
                _gridVao = gl::VAO::create(_gridVbo->getType(), _gridVbo->getID());
            }

            // Create the mesh vertex buffer.
            if (_mesh && !_mesh->triangles.empty() && !_vbo)
            {
                _vbo = gl::VBO::create(
                    _mesh->triangles.size() * 3,
                    gl::VBOType::Pos3_F32_UV_F32_Normal_F32_Color_F32);
                _vbo->copy(gl::convert(*_mesh, _vbo->getType()));
                _vao = gl::VAO::create(_vbo->getType(), _vbo->getID());
            }

            // Create the shaders.
            if (!_gridShader)
            {
                _gridShader = gl::Shader::create(
                    gridVertexSource(),
                    gridFragmentSource());
            }
            if (!_shader)
            {
                _shader = gl::Shader::create(
                    meshVertexSource(),
                    meshFragmentSource());
            }

            // Create the offscreen buffer.
            const Size2I size = g.size();
            gl::OffscreenBufferOptions offscreenBufferOptions;
            offscreenBufferOptions.color = ImageType::RGBA_F32;
#if defined(FTK_API_GL_4_1)
            offscreenBufferOptions.depth = gl::OffscreenDepth::_24;
            offscreenBufferOptions.stencil = gl::OffscreenStencil::_8;
#elif defined(FTK_API_GLES_2)
            offscreenBufferOptions.stencil = gl::OffscreenStencil::_8;
#endif // FTK_API_GL_4_1
            if (gl::doCreate(_buffer, size, offscreenBufferOptions))
            {
                _buffer = gl::OffscreenBuffer::create(size, offscreenBufferOptions);
            }

            // Render the scene.
            if (_doRender && _buffer)
            {
                _doRender = false;
                gl::OffscreenBufferBinding binding(_buffer);

                // Save render state.
                const ViewportState viewportState(event.render);
                const ClipRectEnabledState clipRectEnabledState(event.render);
                const ClipRectState clipRectState(event.render);
                const TransformState transformState(event.render);
                const RenderSizeState renderSizeState(event.render);

                // Setup the camrea.
                event.render->setRenderSize(size);
                event.render->setViewport(Box2I(0, 0, g.w(), g.h()));
                event.render->setClipRectEnabled(false);
                glClearColor(.1F, .1F, .1F, 1.F);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

                // Draw the mesh.
                if (_vbo && _vao)
                {
                    _shader->bind();
                    _shader->setUniform("transform.m", model);
                    _shader->setUniform("transform.mvp", projection * view * model);
                    _shader->setUniform("renderMode", static_cast<int>(_settings.mode));
                    _shader->setUniform("color", Color4F(1.F, 1.F, 1.F));
                    _shader->setUniform("lightDir", V3F(1.F, 1.F, 0.F));
                    _shader->setUniform("ambientLight", V3F(.2F, .2F, .2F));
                    glEnable(GL_DEPTH_TEST);
                    if (_settings.cull)
                    {
                        glEnable(GL_CULL_FACE);
                    }
                    else
                    {
                        glDisable(GL_CULL_FACE);
                    }
                    _vao->bind();
                    _vao->draw(GL_TRIANGLES, 0, _vbo->getSize());
                    glDisable(GL_CULL_FACE);
                    glDisable(GL_DEPTH_TEST);
                }

                // Draw the grid.
                if (_settings.grid && _gridVbo && _gridVao)
                {
                    _gridShader->bind();
                    _gridShader->setUniform("transform.mvp", projection * view);
                    _gridShader->setUniform("color", Color4F(1.F, 1.F, 1.F));
                    _gridShader->setUniform("textureSampler", 0);
                    glBindTexture(GL_TEXTURE_2D, _gridBuffer->getColorID());
                    glActiveTexture(static_cast<GLenum>(GL_TEXTURE0));
                    glEnable(GL_DEPTH_TEST);
                    _gridVao->bind();
                    _gridVao->draw(GL_TRIANGLES, 0, _gridVbo->getSize());
                    glDisable(GL_DEPTH_TEST);
                }
            }
        }
        catch (const std::exception& e)
        {
            if (auto context = getContext())
            {
                context->log("OffscreenWidget", e.what(), LogType::Error);
            }
        }

        // Draw the offscreen buffer.
        if (_buffer)
        {
            const unsigned int id = _buffer->getColorID();
            event.render->drawTexture(id, g, true);
        }

        //const Box2I sb = _getScreenBBox(mvp);
        //event.render->drawRect(
        //    Box2I(g.min.x + sb.min.x, g.min.y + sb.min.y, sb.w(), sb.h()),
        //    Color4F(1.F, 0.F, 0.F, .1F));
    }

    void ObjView::mouseEnterEvent(ftk::MouseEnterEvent& event)
    {
        event.accept = true;
        _mouseInside = true;
    }

    void ObjView::mouseLeaveEvent()
    {
        _mouseInside = false;
    }

    void ObjView::mouseMoveEvent(MouseMoveEvent& event)
    {
        event.accept = true;
        switch (_mouseButton)
        {
        case MouseButton::Left:
        {
            const V2I d = event.pos - event.prev;
            const V2F v(d.x * .25F, d.y * .25F);
            setOrbit(_orbit + v);
            break;
        }
        default: break;
        }
    }

    void ObjView::scrollEvent(ftk::ScrollEvent& event)
    {
        event.accept = true;
        setDistance(_distance + event.value.y * _objectSize * -.1F);
    }

    void ObjView::mousePressEvent(ftk::MouseClickEvent& event)
    {
        event.accept = true;
        takeKeyFocus();
        _mouseButton = event.button;
    }

    void ObjView::mouseReleaseEvent(ftk::MouseClickEvent& event)
    {
        event.accept = true;
        _mouseButton = MouseButton::None;
    }

    ftk::M44F ObjView::_getModelTransform() const
    {
        return
            translate(-_position) *
            rotateX(_rotation.x) *
            rotateY(_rotation.y) *
            rotateZ(_rotation.z) *
            translate(_position);
    }

    ftk::M44F ObjView::_getViewTransform(float distance) const
    {
        return
            translate(V3F(0.F, 0.F, -distance)) *
            rotateX(_orbit.y) *
            rotateY(_orbit.x) *
            translate(_position);
    }

    ftk::M44F ObjView::_getProjectionTransform() const
    {
        const Size2I size = getGeometry().size();
        const float aspect = aspectRatio(size);
        return perspective(_fov, aspect, .1F, 10000.F);
    }

    ftk::M44F ObjView::_getMVPTransform(float distance) const
    {
        return
            _getProjectionTransform() * 
            _getViewTransform(_distance) *
            _getModelTransform();
    }

    ftk::Box3F ObjView::_getObjectBBoxTransformed() const
    {
        auto v = points(_objectBBox);
        const M44F m = _getModelTransform();
        for (size_t i = 0; i < v.size(); ++i)
        {
            v[i] = m * v[i];
        }
        return bbox(v);
    }

    ftk::Box2I ObjView::_getScreenBBox(const ftk::M44F& mvp) const
    {
        auto v = points(_objectBBox);
        std::vector<V2I> v2;
        const Size2I size = getGeometry().size();
        for (size_t i = 0; i < v.size(); ++i)
        {
            const V3F vt = mvp * v[i];
            v2.push_back(V2I(
                (vt.x + 1.F) / 2.F * size.w,
                (vt.y + 1.F) / 2.F * size.h));
        }
        const Box2I b = ftk::bbox(v2);
        return Box2I(b.min.x, size.h - b.min.y - b.h(), b.w(), b.h());
    }
}
