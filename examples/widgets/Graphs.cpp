// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Graphs.h"

#include "MainWindow.h"

using namespace ftk;

namespace widgets
{
    void Graphs::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        ftk::IWidget::_init(context, "Graphs", parent);

        auto diagModel = getParentT<MainWindow>()->getDiagModel();
        _widget = DiagWidget::create(context, diagModel);
        _widget->setMarginRole(SizeRole::Margin);

        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(_widget);
    }

    Graphs::~Graphs()
    {}

    std::shared_ptr<IWidget> Graphs::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Graphs>(new Graphs);
        out->_init(context, parent);
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
