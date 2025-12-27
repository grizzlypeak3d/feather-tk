// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/IRender.h>

namespace ftk
{
    void IRender::_init(
        const std::shared_ptr<LogSystem>& logSystem,
        const std::shared_ptr<FontSystem>& fontSystem)
    {
        _logSystem = logSystem;
        _fontSystem = fontSystem;
    }

    IRender::~IRender()
    {}

    void IRender::drawRect(
        const Box2I& rect,
        const Color4F& color)
    {
        drawRect(Box2F(rect.min.x, rect.min.y, rect.w(), rect.h()), color);
    }

    void IRender::drawRects(
        const std::vector<Box2I>& rects,
        const Color4F& color)
    {
        std::vector<Box2F> rectsF;
        for (const auto& rect : rects)
        {
            rectsF.push_back(Box2F(rect.min.x, rect.min.y, rect.w(), rect.h()));
        }
        drawRects(rectsF, color);
    }
    
    void IRender::drawLine(
        const V2I& v0,
        const V2I& v1,
        const Color4F& color,
        const LineOptions& options)
    {
        drawLine(V2F(v0.x, v0.y), V2F(v1.x, v1.y), color, options);
    }

    void IRender::drawLines(
        const std::vector<std::pair<V2I, V2I> >& lines,
        const Color4F& color,
        const LineOptions& options)
    {
        std::vector<std::pair<V2F, V2F> > linesF;
        for (const auto line : lines)
        {
            linesF.push_back(std::make_pair(
                V2F(line.first.x, line.first.y),
                V2F(line.second.x, line.second.y)));
        }
        drawLines(linesF, color, options);
    }

    void IRender::drawText(
        const std::vector<std::shared_ptr<Glyph> >& glyphs,
        const FontMetrics& fontMetics,
        const V2I& position,
        const Color4F& color)
    {
        drawText(glyphs, fontMetics, V2F(position.x, position.y), color);
    }

    void IRender::drawImage(
        const std::shared_ptr<Image>& image,
        const Box2I& rect,
        const Color4F& color,
        const ImageOptions& options)
    {
        drawImage(
            image,
            Box2F(rect.min.x, rect.min.y, rect.w(), rect.h()),
            color,
            options);
    }

    IRenderFactory::~IRenderFactory()
    {}
}
