// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DiagWidget.h>

#include <ftk/UI/DiagModel.h>
#include <ftk/UI/GraphWidget.h>
#include <ftk/UI/RowLayout.h>

namespace ftk
{
    struct DiagWidget::Private
    {
        std::map<std::string, std::shared_ptr<GraphWidget> > graphs;
        std::shared_ptr<RowLayout> layout;

        std::shared_ptr<MapObserver<DiagData, int64_t> > sampleIncObserver;
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

        p.graphs["GLObjects"] = GraphWidget::create(
            context,
            "OpenGL Objects",
            {
                { ColorRole::Cyan, "Buffers: {0}" },
                { ColorRole::Magenta, "Meshes: {0}" },
                { ColorRole::Yellow, "Shaders: {0}" },
                { ColorRole::Red, "Textures: {0}" }
            },
            p.layout);
        p.graphs["GLMemory"] = GraphWidget::create(
            context,
            "OpenGL Memory",
            {
                { ColorRole::Cyan, "Buffers: {0}MB" },
                { ColorRole::Magenta, "Meshes: {0}MB" },
                { ColorRole::Yellow, "Textures: {0}MB" }
            },
            p.layout);
        p.graphs["Objects"] = GraphWidget::create(
            context,
            "Objects",
            {
                { ColorRole::Cyan, "Images: {0}" },
                { ColorRole::Magenta, "Widgets: {0}" }
            },
            p.layout);
        p.graphs["Memory"] = GraphWidget::create(
            context,
            "Memory",
            {
                { ColorRole::Cyan, "Images: {0}MB" }
            },
            p.layout);

        const std::map<DiagData, std::pair<std::shared_ptr<GraphWidget>, ColorRole> > graphData =
        {
            { DiagData::GLBuffers, std::make_pair(p.graphs["GLObjects"], ColorRole::Cyan) },
            { DiagData::GLMeshes, std::make_pair(p.graphs["GLObjects"], ColorRole::Magenta) },
            { DiagData::GLShaders, std::make_pair(p.graphs["GLObjects"], ColorRole::Yellow) },
            { DiagData::GLTextures, std::make_pair(p.graphs["GLObjects"], ColorRole::Red) },
            { DiagData::GLBuffersMB, std::make_pair(p.graphs["GLMemory"], ColorRole::Cyan) },
            { DiagData::GLMeshesMB, std::make_pair(p.graphs["GLMemory"], ColorRole::Magenta) },
            { DiagData::GLTexturesMB, std::make_pair(p.graphs["GLMemory"], ColorRole::Yellow) },
            { DiagData::Images, std::make_pair(p.graphs["Objects"], ColorRole::Cyan) },
            { DiagData::Widgets, std::make_pair(p.graphs["Objects"], ColorRole::Magenta) },
            { DiagData::ImagesMB, std::make_pair(p.graphs["Memory"], ColorRole::Cyan) }
        };

        const auto& samples = model->getSamples();
        for (const auto& i : graphData)
        {
            const auto j = samples.find(i.first);
            if (j != samples.end())
            {
                i.second.first->setSamples(i.second.second, j->second);
            }
        }

        p.sampleIncObserver = MapObserver<DiagData, int64_t>::create(
            model->observeSamplesInc(),
            [this, graphData](const std::map<DiagData, int64_t>& value)
            {
                FTK_P();
                for (const auto& i : graphData)
                {
                    const auto j = value.find(i.first);
                    if (j != value.end())
                    {
                        i.second.first->addSample(i.second.second, j->second);
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