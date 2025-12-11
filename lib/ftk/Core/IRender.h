// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/FontSystem.h>
#include <ftk/Core/Matrix.h>
#include <ftk/Core/Mesh.h>
#include <ftk/Core/RenderOptions.h>
#include <ftk/Core/Vector.h>

#include <memory>
#include <utility>

namespace ftk
{
    class LogSystem;

    //! \name Rendering
    ///@{
        
    //! Base class for renderers.
    class FTK_API_TYPE IRender : public std::enable_shared_from_this<IRender>
    {
        FTK_NON_COPYABLE(IRender);

    protected:
        void _init(const std::shared_ptr<LogSystem>&);

        IRender() = default;

    public:
        virtual ~IRender() = 0;

        //! Begin a render.
        virtual void begin(
            const Size2I&,
            const RenderOptions& = RenderOptions()) = 0;

        //! Finish a render.
        virtual void end() = 0;

        //! Get the render size.
        virtual Size2I getRenderSize() const = 0;

        //! Set the render size.
        virtual void setRenderSize(const Size2I&) = 0;

        //! Get the render options.
        virtual RenderOptions getRenderOptions() const = 0;

        //! Get the viewport.
        virtual Box2I getViewport() const = 0;

        //! Set the viewport.
        virtual void setViewport(const Box2I&) = 0;

        //! Clear the viewport.
        virtual void clearViewport(const Color4F&) = 0;

        //! Get whether the clipping rectangle is enabled.
        virtual bool getClipRectEnabled() const = 0;

        //! Set whether the clipping rectangle is enabled.
        virtual void setClipRectEnabled(bool) = 0;

        //! Get the clipping rectangle.
        virtual Box2I getClipRect() const = 0;

        //! Set the clipping rectangle.
        virtual void setClipRect(const Box2I&) = 0;

        //! Get the transformation matrix.
        virtual M44F getTransform() const = 0;

        //! Set the transformation matrix.
        virtual void setTransform(const M44F&) = 0;

        //! Draw a filled rectangle.
        virtual void drawRect(
            const Box2F&,
            const Color4F&) = 0;

        //! Draw a filled rectangle.
        virtual void drawRect(
            const Box2I&,
            const Color4F&);

        //! Draw filled rectangles.
        virtual void drawRects(
            const std::vector<Box2F>&,
            const Color4F&) = 0;

        //! Draw filled rectangles.
        virtual void drawRects(
            const std::vector<Box2I>&,
            const Color4F&);

        //! Draw a line.
        virtual void drawLine(
            const V2F&,
            const V2F&,
            const Color4F&,
            const LineOptions& = LineOptions()) = 0;

        //! Draw a line.
        virtual void drawLine(
            const V2I&,
            const V2I&,
            const Color4F&,
            const LineOptions& = LineOptions());

        //! Draw multiple lines.
        virtual void drawLines(
            const std::vector<std::pair<V2F, V2F> >&,
            const Color4F&,
            const LineOptions& = LineOptions()) = 0;

        //! Draw multiple lines.
        virtual void drawLines(
            const std::vector<std::pair<V2I, V2I> >&,
            const Color4F&,
            const LineOptions& = LineOptions());

        //! Draw a triangle mesh.
        virtual void drawMesh(
            const TriMesh2F&,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
            const V2F& pos = V2F()) = 0;

        //! Draw a triangle mesh with vertex color information.
        virtual void drawColorMesh(
            const TriMesh2F&,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
            const V2F& pos = V2F()) = 0;

        //! Draw a texture.
        virtual void drawTexture(
            unsigned int,
            const Box2I&,
            bool flipV = false,
            const Color4F& = Color4F(1.F, 1.F, 1.F),
            AlphaBlend = AlphaBlend::Straight) = 0;

        //! Draw text.
        virtual void drawText(
            const std::vector<std::shared_ptr<Glyph> >&,
            const FontMetrics&,
            const V2F& position,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F)) = 0;

        //! Draw text.
        virtual void drawText(
            const std::vector<std::shared_ptr<Glyph> >&,
            const FontMetrics&,
            const V2I& position,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F));

        //! Draw an image.
        virtual void drawImage(
            const std::shared_ptr<Image>&,
            const TriMesh2F&,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
            const ImageOptions& = ImageOptions()) = 0;

        //! Draw an image.
        virtual void drawImage(
            const std::shared_ptr<Image>&,
            const Box2F&,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
            const ImageOptions& = ImageOptions()) = 0;

        //! Draw an image.
        virtual void drawImage(
            const std::shared_ptr<Image>&,
            const Box2I&,
            const Color4F& = Color4F(1.F, 1.F, 1.F, 1.F),
            const ImageOptions& = ImageOptions());

    protected:
        std::weak_ptr<LogSystem> _logSystem;
    };

    //! Base class for render factories.
    class FTK_API_TYPE IRenderFactory
    {
    public:
        virtual ~IRenderFactory() = 0;

        //! Create a new renderer.
        virtual std::shared_ptr<IRender> createRender(const std::shared_ptr<LogSystem>&) = 0;
    };
        
    ///@}
}

