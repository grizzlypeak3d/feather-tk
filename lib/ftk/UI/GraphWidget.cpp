// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/GraphWidget.h>

#include <ftk/UI/ColorSwatch.h>
#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/LayoutUtil.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Format.h>

#include <list>
#include <optional>

namespace ftk
{
    namespace
    {
        class GraphSubWidget : public IWidget
        {
        protected:
            void _init(
                const std::shared_ptr<Context>&,
                const std::shared_ptr<IWidget>& parent);

            GraphSubWidget() = default;

        public:
            virtual ~GraphSubWidget();

            FTK_API static std::shared_ptr<GraphSubWidget> create(
                const std::shared_ptr<Context>&,
                const std::shared_ptr<IWidget>& parent = nullptr);

            void addSample(ColorRole, int);

            Size2I getSizeHint() const override;
            void setGeometry(const Box2I&) override;
            void sizeHintEvent(const SizeHintEvent&) override;
            void drawEvent(const Box2I&, const DrawEvent&) override;

        private:
            void _samplesUpdate();

            std::map<ColorRole, std::list<int> > _samples;
            int _samplesMax = 0;

            struct SizeData
            {
                std::optional<float> displayScale;
                int sizeHint = 0;
                int sampleSize = 1;
                int border = 0;
            };
            SizeData _size;
        };

        void GraphSubWidget::_init(
            const std::shared_ptr<Context>& context,
            const std::shared_ptr<IWidget>& parent)
        {
            IWidget::_init(context, "ftk::GraphSubWidget", parent);
        }

        GraphSubWidget::~GraphSubWidget()
        {}

        std::shared_ptr<GraphSubWidget> GraphSubWidget::create(
            const std::shared_ptr<Context>& context,
            const std::shared_ptr<IWidget>& parent)
        {
            auto out = std::shared_ptr<GraphSubWidget>(new GraphSubWidget);
            out->_init(context, parent);
            return out;
        }

        void GraphSubWidget::addSample(ColorRole colorRole, int value)
        {
            _samples[colorRole].push_back(value);
            _samplesUpdate();
            setDrawUpdate();
        }

        Size2I GraphSubWidget::getSizeHint() const
        {
            return Size2I(_size.sizeHint, _size.sizeHint);
        }

        void GraphSubWidget::setGeometry(const Box2I& value)
        {
            const bool changed = value != getGeometry();
            IWidget::setGeometry(value);
            if (changed)
            {
                _samplesUpdate();
            }
        }

        void GraphSubWidget::sizeHintEvent(const SizeHintEvent& event)
        {
            if (!_size.displayScale.has_value() ||
                (_size.displayScale.has_value() && _size.displayScale.value() != event.displayScale))
            {
                _size.displayScale = event.displayScale;
                _size.sizeHint = event.style->getSizeRole(SizeRole::ScrollAreaSmall, event.displayScale) / 2;
                _size.sampleSize = event.style->getSizeRole(SizeRole::Handle, event.displayScale);
                _size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            }
        }

        void GraphSubWidget::drawEvent(
            const Box2I& drawRect,
            const DrawEvent& event)
        {
            const Box2I& g = getGeometry();
            event.render->drawMesh(
                border(g, _size.border),
                event.style->getColorRole(ColorRole::Border));

            const Box2I g2 = margin(g, -_size.border);
            event.render->drawRect(
                g2,
                event.style->getColorRole(ColorRole::Base));

            for (const auto& i : _samples)
            {
                const std::vector<int> samples(i.second.begin(), i.second.end());
                std::vector<std::pair<ftk::V2I, ftk::V2I> > lines;
                int x = g2.min.x;
                int y = g2.min.y;
                int h = g2.h();
                for (int i = 1; i < samples.size(); ++i, x += _size.sampleSize)
                {
                    const float v0 = samples[i - 1] / static_cast<float>(_samplesMax);
                    const float v1 = samples[i] / static_cast<float>(_samplesMax);
                    lines.push_back(std::make_pair(
                        ftk::V2I(x, y + h - 1 - v0 * h),
                        ftk::V2I(x + _size.sampleSize, y + h - 1 - v1 * h)));
                }
                LineOptions lineOptions;
                lineOptions.width = _size.border * 2;
                event.render->drawLines(
                    lines,
                    event.style->getColorRole(i.first),
                    lineOptions);
            }
        }

        void GraphSubWidget::_samplesUpdate()
        {
            bool changed = false;

            const Box2I& g = getGeometry();
            const Box2I g2 = margin(g, -_size.border);
            const int w = g2.w();
            for (auto i = _samples.begin(); i != _samples.end(); ++i)
            {
                while (i->second.size() * _size.sampleSize > (w + _size.sampleSize))
                {
                    changed = true;
                    i->second.pop_front();
                }
            }

            int samplesMax = _samplesMax;
            for (const auto& i : _samples)
            {
                for (auto j : i.second)
                {
                    samplesMax = std::max(samplesMax, j);
                }
            }
            if (samplesMax != _samplesMax)
            {
                changed = true;
                _samplesMax = samplesMax;
            }

            if (changed)
            {
                setDrawUpdate();
            }
        }
    }

    struct GraphWidget::Private
    {
        std::shared_ptr<GraphSubWidget> graph;
        std::map<ColorRole, std::shared_ptr<ColorSwatch> > swatches;
        std::map<ColorRole, std::shared_ptr<Label> > labels;
        std::map<ColorRole, std::string> labelText;
        std::shared_ptr<HorizontalLayout> labelLayout;
        std::shared_ptr<VerticalLayout> layout;
    };

    void GraphWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::map<ColorRole, std::string>& labels,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::GraphWidget", parent);
        FTK_P();

        p.graph = GraphSubWidget::create(context);
        for (const auto i : labels)
        {
            p.swatches[i.first] = ColorSwatch::create(context);
            p.labels[i.first] = Label::create(context);
            p.labelText[i.first] = i.second;
        }

        p.layout = VerticalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingSmall);
        auto label = Label::create(context, title, p.layout);
        p.graph->setParent(p.layout);
        p.labelLayout = HorizontalLayout::create(context, p.layout);
        p.labelLayout->setSpacingRole(SizeRole::SpacingSmall);
        for (const auto i : labels)
        {
            p.swatches[i.first]->setParent(p.labelLayout);
            p.labels[i.first]->setParent(p.labelLayout);
        }
    }

    GraphWidget::GraphWidget() :
        _p(new Private)
    {}

    GraphWidget::~GraphWidget()
    {}

    std::shared_ptr<GraphWidget> GraphWidget::create(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::map<ColorRole, std::string>& labels,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<GraphWidget>(new GraphWidget);
        out->_init(context, title, labels, parent);
        return out;
    }

    void GraphWidget::addSample(ColorRole colorRole, int value)
    {
        FTK_P();
        p.graph->addSample(colorRole, value);
        const auto i = p.labels.find(colorRole);
        const auto j = p.labelText.find(colorRole);
        if (i != p.labels.end() && j != p.labelText.end())
        {
            i->second->setText(Format(j->second).arg(value));
        }
    }

    Size2I GraphWidget::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void GraphWidget::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }
    
    void GraphWidget::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        FTK_P();
        for (const auto i : p.swatches)
        {
            i.second->setColor(event.style->getColorRole(i.first));
        }
    }
}