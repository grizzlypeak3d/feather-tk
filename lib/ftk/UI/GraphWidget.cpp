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
        std::map<ColorRole, std::list<int> > samples;
        int samplesMax = 0;

        struct SizeData
        {
            std::optional<float> displayScale;
            int sizeHint = 0;
            int sampleSize = 1;
            int border = 0;
        };
        SizeData size;
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

    void GraphWidget::addSample(int value, ColorRole colorRole)
    {
        FTK_P();
        p.samples[colorRole].push_back(value);
        _samplesUpdate();
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
        }
    }

    void GraphWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.sizeHint = event.style->getSizeRole(SizeRole::ScrollAreaSmall, event.displayScale) / 2;
            p.size.sampleSize = event.style->getSizeRole(SizeRole::Handle, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
        }
    }

    void GraphWidget::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IWidget::drawEvent(drawRect, event);
        FTK_P();

        const Box2I& g = getGeometry();
        event.render->drawMesh(
            border(g, p.size.border),
            event.style->getColorRole(ColorRole::Border));

        for (const auto& i : p.samples)
        {
            const std::vector<int> samples(i.second.begin(), i.second.end());
            std::vector<std::pair<ftk::V2I, ftk::V2I> > lines;
            const Box2I g2 = margin(g, -p.size.border);
            int x = g2.min.x;
            int y = g2.min.y;
            int h = g2.h();
            for (int i = 1; i < samples.size(); ++i, x += p.size.sampleSize)
            {
                const float v0 = samples[i - 1] / static_cast<float>(p.samplesMax);
                const float v1 = samples[i] / static_cast<float>(p.samplesMax);
                lines.push_back(std::make_pair(
                    ftk::V2I(x, y + h - 1 - v0 * h),
                    ftk::V2I(x + p.size.sampleSize, y + h - 1 - v1 * h)));
            }
            LineOptions lineOptions;
            lineOptions.width = p.size.border * 2;
            event.render->drawLines(
                lines,
                event.style->getColorRole(i.first),
                lineOptions);
        }
    }

    void GraphWidget::_samplesUpdate()
    {
        FTK_P();

        bool changed = false;

        const Box2I& g = getGeometry();
        const Box2I g2 = margin(g, -p.size.border);
        const int w = g2.w();
        for (auto i = p.samples.begin(); i != p.samples.end(); ++i)
        {
            while (i->second.size() * p.size.sampleSize > (w + p.size.sampleSize))
            {
                changed = true;
                i->second.pop_front();
            }
        }

        int samplesMax = p.samplesMax;
        for (const auto& i : p.samples)
        {
            for (auto j : i.second)
            {
                samplesMax = std::max(samplesMax, j);
            }
        }
        if (samplesMax != p.samplesMax)
        {
            changed = true;
            p.samplesMax = samplesMax;
        }

        if (changed)
        {
            setDrawUpdate();
        }
    }
}