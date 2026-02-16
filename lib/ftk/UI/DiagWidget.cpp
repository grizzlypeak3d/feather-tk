// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DiagWidget.h>

#include <ftk/UI/DiagModel.h>
#include <ftk/UI/GraphWidget.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Format.h>

namespace ftk
{
    struct DiagWidget::Private
    {
        std::map<std::string, std::shared_ptr<GraphWidget> > graphs;
        std::map<std::string, std::pair<std::shared_ptr<GraphWidget>, ColorRole> > samples;
        std::shared_ptr<RowLayout> layout;

        std::shared_ptr<MapObserver<std::string, int64_t> > sampleIncObserver;
    };

    void DiagWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DiagModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::DiagWidget", parent);
        FTK_P();

        p.layout = VerticalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingSmall);

        const std::vector<ColorRole> colors =
        {
            ColorRole::Cyan,
            ColorRole::Magenta,
            ColorRole::Yellow,
            ColorRole::Red,
            ColorRole::Green,
            ColorRole::Blue
        };
        for (const auto& group : model->getGroups())
        {
            std::map<ColorRole, std::string> labels;
            const auto& names = model->getNames(group);
            for (size_t i = 0; i < names.size() && i < colors.size(); ++i)
            {
                labels[colors[i]] = names[i] + ": {0}";
            }
            auto graph = GraphWidget::create(context, group, labels, p.layout);
            for (size_t i = 0; i < names.size() && i < colors.size(); ++i)
            {
                p.samples[group + "/" + names[i]] = std::make_pair(graph, colors[i]);
            }
            p.graphs[group] = graph;
        }
        const auto& samples = model->getSamples();
        for (const auto& i : samples)
        {
            const auto j = p.samples.find(i.first);
            if (j != p.samples.end())
            {
                j->second.first->setSamples(j->second.second, i.second);
            }
        }

        p.sampleIncObserver = MapObserver<std::string, int64_t>::create(
            model->observeSamplesInc(),
            [this](const std::map<std::string, int64_t>& value)
            {
                FTK_P();
                for (const auto& i : value)
                {
                    const auto j = p.samples.find(i.first);
                    if (j != p.samples.end())
                    {
                        j->second.first->addSample(j->second.second, i.second);
                    }
                }
            });
    }

    DiagWidget::DiagWidget() :
        _p(new Private)
    {}

    DiagWidget::~DiagWidget()
    {}

    std::shared_ptr<DiagWidget> DiagWidget::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DiagModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DiagWidget>(new DiagWidget);
        out->_init(context, model, parent);
        return out;
    }

    SizeRole DiagWidget::getMarginRole() const
    {
        return _p->layout->getMarginRole();
    }

    void DiagWidget::setMarginRole(SizeRole value)
    {
        _p->layout->setMarginRole(value);
    }
    
    Size2I DiagWidget::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void DiagWidget::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }
}