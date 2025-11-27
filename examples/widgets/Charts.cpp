// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Charts.h"

#include <ftk/UI/PieChart.h>
#include <ftk/UI/RowLayout.h>

using namespace ftk;

namespace widgets
{
    void Charts::_init(const std::shared_ptr<Context>& context)
    {
        ftk::IWidget::_init(context, "Charts", nullptr);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create pie charts.
        auto hLayout = HorizontalLayout::create(context, layout);
        auto pieChart = PieChart::create(context, hLayout);
        pieChart->setData(
            {
                PieChartData(100.F, Color4F(.8F, .2F, 0.F))
            });
        pieChart = PieChart::create(context, hLayout);
        pieChart->setData(
            {
                PieChartData(50.F, Color4F(.8F, .2F, 0.F)),
                PieChartData(50.F, Color4F(.9F, .7F, .1F))
            });
        pieChart = PieChart::create(context, hLayout);
        pieChart->setData(
            {
                PieChartData(40.F, Color4F(.8F, .2F, 0.F)),
                PieChartData(30.F, Color4F(.9F, .7F, .1F)),
                PieChartData(15.F, Color4F(0.F, .8F, .3F)),
                PieChartData(10.F, Color4F(.2F, .2F, .9F)),
                PieChartData(5.F, Color4F(.5F, .1F, .8F))
            });
    }

    Charts::~Charts()
    {}

    std::shared_ptr<Charts> Charts::create(const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<Charts>(new Charts);
        out->_init(context);
        return out;
    }

    void Charts::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }

    void Charts::sizeHintEvent(const SizeHintEvent& event)
    {
        setSizeHint(_scrollWidget->getSizeHint());
    }
}
