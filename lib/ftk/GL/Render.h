// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GL/Export.h>
#include <ftk/Core/IRender.h>

#include <ftk/Core/LRUCache.h>

namespace ftk
{
    namespace gl
    {
        class Shader;
        class Texture;

        //! \name Renderer
        ///@{
        
        //! OpenGL renderer.
        class FTK_GL_API_TYPE Render : public IRender
        {
        protected:
            void _init(
                const std::shared_ptr<LogSystem>&,
                const std::shared_ptr<FontSystem>&);

            Render();

        public:
            FTK_GL_API virtual ~Render();

            //! Create a new renderer.
            FTK_GL_API static std::shared_ptr<Render> create(
                const std::shared_ptr<LogSystem>&,
                const std::shared_ptr<FontSystem>&);

            //! Get a shader.
            FTK_GL_API std::shared_ptr<Shader> getShader(const std::string&);

            FTK_GL_API void begin(
                const Size2I&,
                const RenderOptions& = RenderOptions()) override;
            FTK_GL_API void end() override;
            FTK_GL_API Size2I getRenderSize() const override;
            FTK_GL_API void setRenderSize(const Size2I&) override;
            FTK_GL_API RenderOptions getRenderOptions() const override;
            FTK_GL_API Box2I getViewport() const override;
            FTK_GL_API void setViewport(const Box2I&) override;
            FTK_GL_API void clearViewport(const Color4F&) override;
            FTK_GL_API bool getClipRectEnabled() const override;
            FTK_GL_API void setClipRectEnabled(bool) override;
            FTK_GL_API Box2I getClipRect() const override;
            FTK_GL_API void setClipRect(const Box2I&) override;
            FTK_GL_API M44F getTransform() const override;
            FTK_GL_API void setTransform(const M44F&) override;
            FTK_GL_API void drawRect(
                const Box2F&,
                const Color4F&) override;
            FTK_GL_API void drawRects(
                const std::vector<Box2F>&,
                const Color4F&) override;
            FTK_GL_API void drawLine(
                const V2F&,
                const V2F&,
                const Color4F&,
                const LineOptions& = LineOptions()) override;
            FTK_GL_API void drawLines(
                const std::vector<std::pair<V2F, V2F> >&,
                const Color4F&,
                const LineOptions& = LineOptions()) override;
            FTK_GL_API void drawMesh(
                const TriMesh2F&,
                const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
                const V2F& pos = V2F()) override;
            FTK_GL_API void drawColorMesh(
                const TriMesh2F&,
                const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
                const V2F& pos = V2F()) override;
            FTK_GL_API void drawTextureScaled(
                unsigned int,
                const Size2I& sourceSize,
                const Box2I&,
                bool mirrorV = true) override;
            FTK_GL_API void drawTexture(
                unsigned int,
                const Box2I&,
                bool mirrorV = false,
                const Color4F& = Color4F(1.F, 1.F, 1.F),
                AlphaBlend = AlphaBlend::Straight) override;
            FTK_GL_API void drawText(
                const std::vector<std::shared_ptr<Glyph> >&,
                const FontMetrics&,
                const V2F& position,
                const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F)) override;
            FTK_GL_API void drawImage(
                const std::shared_ptr<Image>&,
                const TriMesh2F&,
                const Color4F & = Color4F(1.F, 1.F, 1.F, 1.F),
                const ImageOptions & = ImageOptions()) override;
            FTK_GL_API void drawImage(
                const std::shared_ptr<Image>&,
                const Box2F&,
                const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
                const ImageOptions& = ImageOptions()) override;
            FTK_GL_API RenderDiag getDiag() const override;

        private:
            std::vector<std::shared_ptr<Texture> > _getTextures(
                const ImageInfo&,
                const ImageFilters&,
                size_t offset = 0);
            void _copyTextures(
                const std::shared_ptr<Image>&,
                const std::vector<std::shared_ptr<Texture> >&,
                size_t offset = 0);
            //! Bind the image's planes to consecutive texture units and point
            //! the shader's samplers at them.
            void _setActiveTextures(
                const std::shared_ptr<Shader>&,
                const ImageInfo& info,
                const std::vector<std::shared_ptr<Texture> >&,
                size_t offset = 0);

            void _drawTextMesh(const TriMesh2F&);

            //! Draw an image with a separable two pass resample. Returns false
            //! if the request is not one this can serve, leaving the caller to
            //! draw it the ordinary way.
            bool _drawImageScaled(
                const std::shared_ptr<Image>&,
                const TriMesh2F&,
                const Color4F&,
                const ImageOptions&,
                const std::vector<std::shared_ptr<Texture> >&);

            void _drawScaleQuad(const Box2F&);

            //! Make sure the contribution tables match these sizes.
            void _scaleContribUpdate(const Size2I& source, const Size2I& dest);

            //! Make the intermediate for this size current, creating it if it
            //! is not already kept. Returns false if it could not be made.
            bool _scaleBufferUpdate(const Size2I&);

            FTK_PRIVATE();
        };

        //! OpenGL render factory.
        class FTK_GL_API_TYPE RenderFactory : public IRenderFactory
        {
        public:
            FTK_GL_API std::shared_ptr<IRender> createRender(
                const std::shared_ptr<LogSystem>&,
                const std::shared_ptr<FontSystem>&) override;
        };
        
        ///@}
    }
}

