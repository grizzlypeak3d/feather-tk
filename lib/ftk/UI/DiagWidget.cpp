// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DiagWidget.h>

#include <ftk/UI/GraphWidget.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/DiagSystem.h>
#include <ftk/Core/Format.h>

#include <algorithm>

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
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::DiagWidget", parent);
        FTK_P();

        p.layout = VerticalLayout::create(context);
        _setWidget(p.layout);
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

        const auto system = context->getSystem<DiagSystem>();
        // A sample now, so the graphs open with their current values rather
        // than with empty boxes and unlabelled colors until the next tick,
        // which is seconds away and reads as the tool being broken.
        system->tick();
        // In the system's order, which puts what the application is doing
        // ahead of the object counts. A graph tells its lines apart by
        // color, so a group with more lines than colors goes on in a second
        // graph rather than losing the rest.
        for (const auto& group : system->getGroups())
        {
            const auto& names = system->getNames(group);
            for (size_t first = 0; first < names.size(); first += colors.size())
            {
                const size_t last = std::min(first + colors.size(), names.size());
                std::vector<GraphLabel> labels;
                for (size_t j = first; j < last; ++j)
                {
                    labels.push_back(GraphLabel{
                        colors[j - first],
                        names[j],
                        system->getFormat(group + "/" + names[j])});
                }
                const std::string title = 0 == first ?
                    group :
                    Format("{0} ({1})").arg(group).arg(first / colors.size() + 1).str();
                auto graph = GraphWidget::create(context, title, labels, p.layout);
                for (size_t j = first; j < last; ++j)
                {
                    p.samples[group + "/" + names[j]] = std::make_pair(graph, colors[j - first]);
                }
                p.graphs[title] = graph;
            }
        }

        const auto& samples = system->getSamples();
        for (const auto& i : samples)
        {
            if (const auto j = p.samples.find(i.first); j != p.samples.end())
            {
                j->second.first->setSamples(j->second.second, i.second);
            }
        }

        p.sampleIncObserver = MapObserver<std::string, int64_t>::create(
            system->observeSamplesInc(),
            [this](const std::map<std::string, int64_t>& value)
            {
                FTK_P();
                for (const auto& i : value)
                {
                    if (const auto j = p.samples.find(i.first);
                        j != p.samples.end())
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
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DiagWidget>(new DiagWidget);
        out->_init(context, parent);
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
    
    

    
}