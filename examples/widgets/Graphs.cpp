// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Graphs.h"

#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Format.h>

using namespace ftk;

namespace widgets
{
    void Graphs::_init(const std::shared_ptr<Context>& context)
    {
        ftk::IWidget::_init(context, "Graphs", nullptr);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create graph widgets.
        auto vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _imagesLabel = Label::create(context, "Images:", vLayout);
        _imagesGraph = GraphWidget::create(context, vLayout);

        vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _imagesSizeLabel = Label::create(context, "Total image size (MB):", vLayout);
        _imageSizeGraph = GraphWidget::create(context, vLayout);
        
        vLayout = VerticalLayout::create(context, layout);
        layout->setSpacingRole(SizeRole::SpacingSmall);
        _widgetsLabel = Label::create(context, "Widgets:", vLayout);
        _widgetsGraph = GraphWidget::create(context, vLayout);

        _timer = Timer::create(context);
        _timer->setRepeating(true);
        _timer->start(
            std::chrono::milliseconds(1000),
            [this]
            {
                size_t count = Image::getObjectCount();
                _imagesLabel->setText(Format("Images: {0}").arg(count));
                _imagesGraph->addSample(count);

                count = Image::getTotalByteCount() / megabyte;
                _imagesSizeLabel->setText(Format("Total image size (MB): {0}").arg(count));
                _imageSizeGraph->addSample(count);

                count = IWidget::getObjectCount();
                _widgetsLabel->setText(Format("Widgets: {0}").arg(count));
                _widgetsGraph->addSample(count);
            });
    }

    Graphs::~Graphs()
    {}

    std::shared_ptr<Graphs> Graphs::create(const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<Graphs>(new Graphs);
        out->_init(context);
        return out;
    }

    Size2I Graphs::getSizeHint() const
    {
        return _scrollWidget->getSizeHint();
    }

    void Graphs::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }
}
