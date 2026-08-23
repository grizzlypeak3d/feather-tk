// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>
#include <ftk/Core/FontSystem.h>
#include <ftk/Core/Matrix.h>
#include <ftk/Core/Mesh.h>
#include <ftk/Core/RenderOptions.h>
#include <ftk/Core/Vector.h>

#include <memory>
#include <utility>

namespace ftk
{
    class FontSystem;
    class LogSystem;

    //! \name Rendering
    ///@{

    //! Render diagnostics.
    struct FTK_CORE_API_TYPE RenderDiag
    {
        //! Mean frame time over the last few dozen frames, in microseconds.
        //!
        //! A mean rather than the last frame, and microseconds rather than
        //! milliseconds, because this is read by a sampler that fires every
        //! few seconds: one frame rounded to a millisecond varies by more than
        //! the thing being measured, which makes the number useless for
        //! comparing one configuration against another.
        int64_t time = 0;

        //! The worst frame over the same window, in microseconds. The mean
        //! says what it costs; the peak says whether it hitches.
        int64_t timePeak = 0;

        int64_t triangles = 0;
        int64_t textures = 0;
        int64_t glyphs = 0;
    };

    //! Base class for renderers.
    class FTK_CORE_API_TYPE IRender : public std::enable_shared_from_this<IRender>
    {
        FTK_NON_COPYABLE(IRender);

    protected:
        FTK_CORE_API void _init(
            const std::shared_ptr<LogSystem>&,
            const std::shared_ptr<FontSystem>&);

        IRender() = default;

    public:
        FTK_CORE_API virtual ~IRender() = 0;

        //! Begin a render.
        FTK_CORE_API virtual void begin(
            const Size2I&,
            const RenderOptions& = RenderOptions()) = 0;

        //! Finish a render.
        FTK_CORE_API virtual void end() = 0;

        //! Get the render size.
        FTK_CORE_API virtual Size2I getRenderSize() const = 0;

        //! Set the render size.
        FTK_CORE_API virtual void setRenderSize(const Size2I&) = 0;

        //! Get the render options.
        FTK_CORE_API virtual RenderOptions getRenderOptions() const = 0;

        //! Get the viewport.
        FTK_CORE_API virtual Box2I getViewport() const = 0;

        //! Set the viewport.
        FTK_CORE_API virtual void setViewport(const Box2I&) = 0;

        //! Clear the viewport.
        FTK_CORE_API virtual void clearViewport(const Color4F&) = 0;

        //! Get whether the clipping rectangle is enabled.
        FTK_CORE_API virtual bool getClipRectEnabled() const = 0;

        //! Set whether the clipping rectangle is enabled.
        FTK_CORE_API virtual void setClipRectEnabled(bool) = 0;

        //! Get the clipping rectangle.
        FTK_CORE_API virtual Box2I getClipRect() const = 0;

        //! Set the clipping rectangle.
        FTK_CORE_API virtual void setClipRect(const Box2I&) = 0;

        //! Get the transformation matrix.
        FTK_CORE_API virtual M44F getTransform() const = 0;

        //! Set the transformation matrix.
        FTK_CORE_API virtual void setTransform(const M44F&) = 0;

        //! Draw a filled rectangle.
        FTK_CORE_API virtual void drawRect(
            const Box2F&,
            const Color4F&) = 0;

        //! Draw a filled rectangle.
        FTK_CORE_API virtual void drawRect(
            const Box2I&,
            const Color4F&);

        //! Draw filled rectangles.
        FTK_CORE_API virtual void drawRects(
            const std::vector<Box2F>&,
            const Color4F&) = 0;

        //! Draw filled rectangles.
        FTK_CORE_API virtual void drawRects(
            const std::vector<Box2I>&,
            const Color4F&);

        //! Draw a line.
        FTK_CORE_API virtual void drawLine(
            const V2F&,
            const V2F&,
            const Color4F&,
            const LineOptions& = LineOptions()) = 0;

        //! Draw a line.
        FTK_CORE_API virtual void drawLine(
            const V2I&,
            const V2I&,
            const Color4F&,
            const LineOptions& = LineOptions());

        //! Draw multiple lines.
        FTK_CORE_API virtual void drawLines(
            const std::vector<std::pair<V2F, V2F> >&,
            const Color4F&,
            const LineOptions& = LineOptions()) = 0;

        //! Draw multiple lines.
        FTK_CORE_API virtual void drawLines(
            const std::vector<std::pair<V2I, V2I> >&,
            const Color4F&,
            const LineOptions& = LineOptions());

        //! Draw a triangle mesh.
        FTK_CORE_API virtual void drawMesh(
            const TriMesh2F&,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
            const V2F& pos = V2F()) = 0;

        //! Draw a triangle mesh with vertex color information.
        FTK_CORE_API virtual void drawColorMesh(
            const TriMesh2F&,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
            const V2F& pos = V2F()) = 0;

        //! Reduce a texture with a separable two pass resample, drawing the
        //! result into the box. Use when the reduction is large enough that
        //! ImageFilter::Linear's four texels miss most of the source; see
        //! ImageFilter::HighQuality.
        FTK_CORE_API virtual void drawTextureScaled(
            unsigned int,
            const Size2I& sourceSize,
            const Box2I& rect,
            bool mirrorV = true);

        FTK_CORE_API virtual void drawTexture(
            unsigned int,
            const Box2I&,
            bool mirrorV = false,
            const Color4F& = Color4F(1.F, 1.F, 1.F),
            AlphaBlend = AlphaBlend::Straight) = 0;

        //! Draw text.
        FTK_CORE_API virtual void drawText(
            const std::vector<std::shared_ptr<Glyph> >&,
            const FontMetrics&,
            const V2F& position,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F)) = 0;

        //! Draw text.
        FTK_CORE_API virtual void drawText(
            const std::vector<std::shared_ptr<Glyph> >&,
            const FontMetrics&,
            const V2I& position,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F));

        //! Draw an image.
        FTK_CORE_API virtual void drawImage(
            const std::shared_ptr<Image>&,
            const TriMesh2F&,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
            const ImageOptions& = ImageOptions()) = 0;

        //! Draw an image.
        FTK_CORE_API virtual void drawImage(
            const std::shared_ptr<Image>&,
            const Box2F&,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
            const ImageOptions& = ImageOptions()) = 0;

        //! Draw an image.
        FTK_CORE_API virtual void drawImage(
            const std::shared_ptr<Image>&,
            const Box2I&,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
            const ImageOptions& = ImageOptions());

        //! Get diagnostics for the last render.
        FTK_CORE_API virtual RenderDiag getDiag() const = 0;

    protected:
        std::weak_ptr<LogSystem> _logSystem;
        std::weak_ptr<FontSystem> _fontSystem;
    };

    //! Base class for render factories.
    class FTK_CORE_API_TYPE IRenderFactory
    {
    public:
        FTK_CORE_API virtual ~IRenderFactory() = 0;

        //! Create a new renderer.
        FTK_CORE_API virtual std::shared_ptr<IRender> createRender(
            const std::shared_ptr<LogSystem>&,
            const std::shared_ptr<FontSystem>&) = 0;
    };
        
    ///@}
}

