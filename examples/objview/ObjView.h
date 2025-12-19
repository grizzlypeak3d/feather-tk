// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include "Settings.h"

#include <ftk/UI/IMouseWidget.h>

#include <ftk/GL/Mesh.h>
#include <ftk/GL/OffscreenBuffer.h>
#include <ftk/GL/Shader.h>
#include <ftk/GL/Texture.h>

namespace objview
{
    class App;
    class Document;
    class HUDWidget;

    //! Object view widget.
    class ObjView : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<Document>&,
            const std::shared_ptr<ftk::IWidget>& parent);

        ObjView() = default;

    public:
        virtual ~ObjView();

        //! Create a new view.
        static std::shared_ptr<ObjView> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<Document>&,
            const std::shared_ptr<ftk::IWidget>& parent = nullptr);

        //! \name View
        ///@{

        void setOrbit(const ftk::V2F&);
        void setDistance(float);
        void frame();
        void zoomIn();
        void zoomOut();

        void orbitLeft();
        void orbitRight();
        void orbitUp();
        void orbitDown();

        ///@}

        void setGeometry(const ftk::Box2I&) override;
        void drawEvent(const ftk::Box2I&, const ftk::DrawEvent&) override;
        void mouseEnterEvent(ftk::MouseEnterEvent&) override;
        void mouseLeaveEvent() override;
        void mouseMoveEvent(ftk::MouseMoveEvent&) override;
        void mousePressEvent(ftk::MouseClickEvent&) override;
        void mouseReleaseEvent(ftk::MouseClickEvent&) override;
        void scrollEvent(ftk::ScrollEvent&) override;

    private:
        ftk::M44F _getModelTransform() const;
        ftk::M44F _getViewTransform(float distance) const;
        ftk::M44F _getProjectionTransform() const;
        ftk::M44F _getMVPTransform(float distance) const;
        ftk::Box3F _getObjectBBoxTransformed() const;
        ftk::Box2I _getScreenBBox(const ftk::M44F&) const;

        std::shared_ptr<ftk::TriMesh3F> _mesh;
        ftk::Box3F _objectBBox;
        float _objectSize = 0.F;
        float _fov = 60.F;
        ftk::V3F _rotation;
        ftk::V3F _position;
        ftk::V2F _orbit = ftk::V2F(30.F, 30.F);
        float _distance = 10.F;
        RenderSettings _settings;

        bool _mouseInside = false;
        ftk::MouseButton _mouseButton = ftk::MouseButton::None;

        bool _frameInit = true;
        bool _doRender = true;
        std::shared_ptr<ftk::gl::OffscreenBuffer> _gridBuffer;
        std::shared_ptr<ftk::gl::VBO> _gridVbo;
        std::shared_ptr<ftk::gl::VAO> _gridVao;
        std::shared_ptr<ftk::gl::Shader> _gridShader;
        std::shared_ptr<ftk::gl::VBO> _vbo;
        std::shared_ptr<ftk::gl::VAO> _vao;
        std::shared_ptr<ftk::gl::Shader> _shader;
        std::shared_ptr<ftk::gl::OffscreenBuffer> _buffer;

        std::shared_ptr<HUDWidget> _hudWidget;

        std::shared_ptr<ftk::Observer<ftk::V3F> > _rotationObserver;
        std::shared_ptr<ftk::Observer<RenderSettings> > _renderSettingsObserver;
    };
}
