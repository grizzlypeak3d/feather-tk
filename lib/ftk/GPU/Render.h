// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/GPU/Export.h>
#include <ftk/GPU/Shader.h>

#include <ftk/Core/IRender.h>

#include <string>

struct SDL_GPUSampler;
struct SDL_GPUTexture;

namespace ftk
{
    namespace gpu
    {
        class OffscreenBuffer;
        class System;
        class Texture;

        //! How what is drawn is put over what is there.
        enum class FTK_GPU_API_TYPE Blend
        {
            //! glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA): the alpha
            //! goes the way the color does.
            Default,
            //! What is drawn replaces what is there.
            None,
            Straight,
            Premultiplied,
            //! Premultiplied color, with the alpha added to what is there.
            PremultipliedAddAlpha
        };

        //! A texture and how it is sampled, for a shader's slot.
        struct FTK_GPU_API_TYPE TextureBinding
        {
            SDL_GPUTexture* texture = nullptr;
            SDL_GPUSampler* sampler = nullptr;
        };

        class Render;

        //! What a renderer that draws with the GPU renderer says of itself:
        //! which one. A renderer built on this one -- to draw things of its
        //! own -- is still an IRender, and this is how what it draws with
        //! is found, to be told what to draw into.
        class FTK_GPU_API_TYPE IGPURender
        {
        public:
            FTK_GPU_API virtual ~IGPURender() = 0;

            FTK_GPU_API virtual std::shared_ptr<Render> getGPURender() = 0;
        };

        //! Get the GPU renderer a renderer draws with, if it does.
        FTK_GPU_API std::shared_ptr<Render> getRender(const std::shared_ptr<IRender>&);

        //! Get whether a device filters thirty-two bit float textures: a
        //! lookup table kept in one is read between its entries, and a
        //! device that cannot reads the nearest instead, which shows as
        //! steps. Where it cannot, a texture of them is kept as half float,
        //! which every device filters. Vulkan leaves it to the driver and
        //! SDL has no way to ask, so it is found out by drawing: once for a
        //! device, and false where that cannot be done.
        //! FTK_GPU_NO_FLOAT_FILTER says it cannot, to try what such a
        //! device gets.
        FTK_GPU_API bool hasFloatFilter(const std::shared_ptr<System>&);

        //! GPU renderer.
        //!
        //! Where the OpenGL renderer draws as it is called, this one cannot:
        //! the API draws inside a render pass and copies data outside of
        //! one. So what a pass draws is kept until the pass ends, and is
        //! then written into the frame's command buffer after the vertices
        //! and the textures it draws with. A frame is one command buffer,
        //! submitted when it ends.
        class FTK_GPU_API_TYPE Render : public IRender, public IGPURender
        {
        protected:
            void _init(
                const std::shared_ptr<System>&,
                const std::shared_ptr<LogSystem>&,
                const std::shared_ptr<FontSystem>&);

            Render();

        public:
            FTK_GPU_API virtual ~Render();

            //! Create a new renderer.
            FTK_GPU_API static std::shared_ptr<Render> create(
                const std::shared_ptr<System>&,
                const std::shared_ptr<LogSystem>&,
                const std::shared_ptr<FontSystem>&);

            //! Set what is drawn into, before begin(). OpenGL has a frame
            //! buffer that is bound; here it is said.
            FTK_GPU_API void setTarget(const std::shared_ptr<OffscreenBuffer>&);

            //! Get the system.
            FTK_GPU_API const std::shared_ptr<System>& getSystem() const;

            FTK_GPU_API std::shared_ptr<Render> getGPURender() override;

            //! \name Targets
            //! Drawing into another buffer for a while, between begin() and
            //! end(): what OpenGL does by binding a frame buffer. The
            //! viewport becomes the whole of the buffer and the clipping is
            //! turned off; both are put back, with the target, by the pop.
            ///@{

            FTK_GPU_API void pushTarget(
                const std::shared_ptr<OffscreenBuffer>&,
                bool clear = true,
                const Color4F& = Color4F(0.F, 0.F, 0.F, 0.F));
            FTK_GPU_API void popTarget();
            FTK_GPU_API const std::shared_ptr<OffscreenBuffer>& getTarget() const;

            ///@}

            //! \name Shaders
            //! For what draws through this renderer with shaders of its
            //! own. A shader is a fragment stage, drawn with this
            //! renderer's vertex stage: positions, and texture coordinates
            //! passed on at location zero ("user(locn0)" to Metal). See
            //! ShaderSource for where its uniforms and textures go.
            ///@{

            FTK_GPU_API void setShader(
                const std::string& name,
                const ShaderSource& fragmentSource,
                size_t samplers);
            FTK_GPU_API bool hasShader(const std::string& name) const;
            FTK_GPU_API void removeShader(const std::string& name);
            FTK_GPU_API void drawShader(
                const std::string& name,
                Blend,
                const TriMesh2F&,
                const M44F& transform,
                const void* uniforms,
                size_t uniformsByteCount,
                const std::vector<TextureBinding>& = {});

            //! Get a sampler that clamps to the edge.
            FTK_GPU_API SDL_GPUSampler* getSampler(ImageFilter) const;

            //! Set whether what is drawn is blended with what is there, as
            //! glEnable(GL_BLEND) does. Off, everything drawn replaces it.
            FTK_GPU_API void setBlendEnabled(bool);

            ///@}

            FTK_GPU_API void begin(
                const Size2I&,
                const RenderOptions& = RenderOptions()) override;
            FTK_GPU_API void end() override;
            FTK_GPU_API Size2I getRenderSize() const override;
            FTK_GPU_API void setRenderSize(const Size2I&) override;
            FTK_GPU_API RenderOptions getRenderOptions() const override;
            FTK_GPU_API Box2I getViewport() const override;
            FTK_GPU_API void setViewport(const Box2I&) override;
            FTK_GPU_API void clearViewport(const Color4F&) override;
            FTK_GPU_API bool getClipRectEnabled() const override;
            FTK_GPU_API void setClipRectEnabled(bool) override;
            FTK_GPU_API Box2I getClipRect() const override;
            FTK_GPU_API void setClipRect(const Box2I&) override;
            FTK_GPU_API M44F getTransform() const override;
            FTK_GPU_API void setTransform(const M44F&) override;
            using IRender::drawRect;
            FTK_GPU_API void drawRect(
                const Box2F&,
                const Color4F&) override;
            using IRender::drawRects;
            FTK_GPU_API void drawRects(
                const std::vector<Box2F>&,
                const Color4F&) override;
            using IRender::drawLine;
            FTK_GPU_API void drawLine(
                const V2F&,
                const V2F&,
                const Color4F&,
                const LineOptions& = LineOptions()) override;
            using IRender::drawLines;
            FTK_GPU_API void drawLines(
                const std::vector<std::pair<V2F, V2F> >&,
                const Color4F&,
                const LineOptions& = LineOptions()) override;
            FTK_GPU_API void drawMesh(
                const TriMesh2F&,
                const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
                const V2F& pos = V2F()) override;
            FTK_GPU_API void drawColorMesh(
                const TriMesh2F&,
                const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
                const V2F& pos = V2F()) override;
            FTK_GPU_API void drawTextureScaled(
                unsigned int,
                const Size2I& sourceSize,
                const Box2I& rect,
                bool mirrorV = true) override;
            FTK_GPU_API void drawTexture(
                unsigned int,
                const Box2I&,
                bool mirrorV = false,
                const Color4F& = Color4F(1.F, 1.F, 1.F),
                AlphaBlend = AlphaBlend::Straight) override;
            using IRender::drawText;
            FTK_GPU_API void drawText(
                const std::vector<std::shared_ptr<Glyph> >&,
                const FontMetrics&,
                const V2F& position,
                const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F)) override;
            using IRender::drawImage;
            FTK_GPU_API void drawImage(
                const std::shared_ptr<Image>&,
                const TriMesh2F&,
                const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
                const ImageOptions& = ImageOptions()) override;
            FTK_GPU_API void drawImage(
                const std::shared_ptr<Image>&,
                const Box2F&,
                const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
                const ImageOptions& = ImageOptions()) override;
            FTK_GPU_API RenderDiag getDiag() const override;

            //! \name Diagnostics
            //! Summed over the renderers that exist.
            ///@{

            FTK_GPU_API static size_t getShaderCount();
            FTK_GPU_API static size_t getVertexByteCount();
            FTK_GPU_API static size_t getTextureCacheByteCount();
            FTK_GPU_API static size_t getTextureCacheCount();
            FTK_GPU_API static size_t getTexturePoolByteCount();
            FTK_GPU_API static size_t getTexturePoolCount();

            ///@}

        private:
            std::vector<std::shared_ptr<Texture> > _getTextures(
                const ImageInfo&,
                const ImageFilters&);
            void _copyTextures(
                const std::shared_ptr<Image>&,
                const std::vector<std::shared_ptr<Texture> >&);
            bool _drawImageScaled(
                const std::shared_ptr<Image>&,
                const TriMesh2F&,
                const Color4F&,
                const ImageOptions&,
                const std::vector<std::shared_ptr<Texture> >&);

            FTK_PRIVATE();
        };

        //! GPU renderer factory.
        class FTK_GPU_API_TYPE RenderFactory : public IRenderFactory
        {
        public:
            FTK_GPU_API RenderFactory(const std::shared_ptr<System>&);

            FTK_GPU_API virtual ~RenderFactory();

            FTK_GPU_API std::shared_ptr<IRender> createRender(
                const std::shared_ptr<LogSystem>& logSystem,
                const std::shared_ptr<FontSystem>& fontSystem) override;

        private:
            std::weak_ptr<System> _system;
        };
    }
}
