// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

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

        //! Texture cache.
        typedef LRUCache<
            std::shared_ptr<Image>,
            std::vector<std::shared_ptr<Texture> > > TextureCache;
        
        //! OpenGL renderer.
        class FTK_API_TYPE Render : public IRender
        {
        protected:
            void _init(
                const std::shared_ptr<LogSystem>&,
                const std::shared_ptr<TextureCache>&);

            Render();

        public:
            FTK_API virtual ~Render();

            //! Create a new renderer.
            FTK_API static std::shared_ptr<Render> create(
                const std::shared_ptr<LogSystem>& = nullptr,
                const std::shared_ptr<TextureCache>& = nullptr);

            //! Get a shader.
            FTK_API std::shared_ptr<Shader> getShader(const std::string&);

            //! Get the texture cache.
            FTK_API const std::shared_ptr<TextureCache>& getTextureCache() const;

            FTK_API void begin(
                const Size2I&,
                const RenderOptions& = RenderOptions()) override;
            FTK_API void end() override;
            FTK_API Size2I getRenderSize() const override;
            FTK_API void setRenderSize(const Size2I&) override;
            FTK_API RenderOptions getRenderOptions() const override;
            FTK_API Box2I getViewport() const override;
            FTK_API void setViewport(const Box2I&) override;
            FTK_API void clearViewport(const Color4F&) override;
            FTK_API bool getClipRectEnabled() const override;
            FTK_API void setClipRectEnabled(bool) override;
            FTK_API Box2I getClipRect() const override;
            FTK_API void setClipRect(const Box2I&) override;
            FTK_API M44F getTransform() const override;
            FTK_API void setTransform(const M44F&) override;
            FTK_API void drawRect(
                const Box2F&,
                const Color4F&) override;
            FTK_API void drawRects(
                const std::vector<Box2F>&,
                const Color4F&) override;
            FTK_API void drawLine(
                const V2F&,
                const V2F&,
                const Color4F&,
                const LineOptions& = LineOptions()) override;
            FTK_API void drawLines(
                const std::vector<std::pair<V2F, V2F> >&,
                const Color4F&,
                const LineOptions& = LineOptions()) override;
            FTK_API void drawMesh(
                const TriMesh2F&,
                const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
                const V2F& pos = V2F()) override;
            FTK_API void drawColorMesh(
                const TriMesh2F&,
                const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
                const V2F& pos = V2F()) override;
            FTK_API void drawTexture(
                unsigned int,
                const Box2I&,
                bool flipV = false,
                const Color4F& = Color4F(1.F, 1.F, 1.F),
                AlphaBlend = AlphaBlend::Straight) override;
            FTK_API void drawText(
                const std::vector<std::shared_ptr<Glyph> >&,
                const FontMetrics&,
                const V2F& position,
                const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F)) override;
            FTK_API void drawImage(
                const std::shared_ptr<Image>&,
                const TriMesh2F&,
                const Color4F & = Color4F(1.F, 1.F, 1.F, 1.F),
                const ImageOptions & = ImageOptions()) override;
            FTK_API void drawImage(
                const std::shared_ptr<Image>&,
                const Box2F&,
                const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
                const ImageOptions& = ImageOptions()) override;

        private:
            std::vector<std::shared_ptr<Texture> > _getTextures(
                const ImageInfo&,
                const ImageFilters&,
                size_t offset = 0);
            void _copyTextures(
                const std::shared_ptr<Image>&,
                const std::vector<std::shared_ptr<Texture> >&,
                size_t offset = 0);
            void _setActiveTextures(
                const ImageInfo& info,
                const std::vector<std::shared_ptr<Texture> >&,
                size_t offset = 0);

            void _drawTextMesh(const TriMesh2F&);
            
            void _log();

            FTK_PRIVATE();
        };

        //! OpenGL render factory.
        class FTK_API_TYPE RenderFactory : public IRenderFactory
        {
        public:
            FTK_API std::shared_ptr<IRender> createRender(const std::shared_ptr<LogSystem>& logSystem) override;
        };
        
        ///@}
    }
}

