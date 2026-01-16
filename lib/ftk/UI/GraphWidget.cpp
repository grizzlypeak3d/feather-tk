// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/GraphWidget.h>

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/LayoutUtil.h>

#include <ftk/Core/Format.h>

#include <list>
#include <optional>

namespace ftk
{
    struct GraphWidget::Private
    {
        std::list<int> samples;
        int samplesMax = 0;

        struct SizeData
        {
            std::optional<float> displayScale;
            int sizeHint = 0;
            int sampleSize = 1;
            int border = 0;
        };
        SizeData size;

        struct DrawData
        {
            Box2I g;
            TriMesh2F mesh;
        };
        std::optional<DrawData> draw;
    };

    void GraphWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::GraphWidget", parent);
        setBackgroundRole(ColorRole::Base);
    }

    GraphWidget::GraphWidget() :
        _p(new Private)
    {}

    GraphWidget::~GraphWidget()
    {}

    std::shared_ptr<GraphWidget> GraphWidget::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<GraphWidget>(new GraphWidget);
        out->_init(context, parent);
        return out;
    }
    
    void GraphWidget::addSample(int value)
    {
        FTK_P();
        p.samples.push_back(value);
        _samplesUpdate();
        p.draw.reset();
        setDrawUpdate();
    }

    Size2I GraphWidget::getSizeHint() const
    {
        FTK_P();
        return Size2I(p.size.sizeHint, p.size.sizeHint);
    }

    void GraphWidget::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IWidget::setGeometry(value);
        FTK_P();
        if (changed)
        {
            _samplesUpdate();
            p.draw.reset();
        }
    }

    void GraphWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.sizeHint = event.style->getSizeRole(SizeRole::ScrollAreaSmall, event.displayScale);
            p.size.sampleSize = event.style->getSizeRole(SizeRole::Handle, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
        }
    }

    void GraphWidget::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void GraphWidget::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IWidget::drawEvent(drawRect, event);
        FTK_P();

        const Box2I& g = getGeometry();
        const int w = g.w();
        const int h = g.h();
        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            int j = 0;
            for (auto i = p.samples.begin(); i != p.samples.end() && j < w; ++i, j = j + p.size.sampleSize)
            {
                const int v = *i / static_cast<float>(p.samplesMax) * h;
                const Box2I b = margin(Box2I(j, h - 1 - v, p.size.sampleSize, v), -p.size.border);
                p.draw->mesh.v.push_back(V2F(b.min.x, b.min.y));
                p.draw->mesh.v.push_back(V2F(b.min.x, b.max.y));
                p.draw->mesh.v.push_back(V2F(b.max.x, b.min.y));
                p.draw->mesh.v.push_back(V2F(b.max.x, b.max.y));
            }
            for (int i = 0; i < p.draw->mesh.v.size() - 4; i += 4)
            {
                Triangle2 t;
                t.v[0].v = i + 0 + 1;
                t.v[1].v = i + 1 + 1;
                t.v[2].v = i + 3 + 1;
                p.draw->mesh.triangles.push_back(t);
                t.v[0].v = i + 0 + 1;
                t.v[1].v = i + 3 + 1;
                t.v[2].v = i + 2 + 1;
                p.draw->mesh.triangles.push_back(t);
            }
        }

        event.render->drawMesh(
            p.draw->mesh,
            event.style->getColorRole(ColorRole::Cyan),
            V2F(g.min.x, g.min.y));
    }

    void GraphWidget::_samplesUpdate()
    {
        FTK_P();

        bool changed = false;

        const Box2I& g = getGeometry();
        const int w = g.w();
        while ((p.samples.size() - 1) * p.size.sampleSize > w)
        {
            changed = true;
            p.samples.pop_front();
        }

        int samplesMax = p.samplesMax;
        for (auto i : p.samples)
        {
            samplesMax = std::max(samplesMax, i);
        }
        if (samplesMax != p.samplesMax)
        {
            changed = true;
            p.samplesMax = samplesMax;
        }

        if (changed)
        {
            p.draw.reset();
            setDrawUpdate();
        }
    }
}