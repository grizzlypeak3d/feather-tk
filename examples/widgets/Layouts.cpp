// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Layouts.h"

#include <ftk/UI/FormLayout.h>
#include <ftk/UI/GridLayout.h>
#include <ftk/UI/GroupBox.h>
#include <ftk/UI/IntEdit.h>
#include <ftk/UI/IntEditSlider.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>

using namespace ftk;

namespace widgets
{
    void Layouts::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        ftk::IWidget::_init(context, "Layouts", parent);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create row layouts.
        auto groupBox = GroupBox::create(context, "Row Layouts", layout);
        auto groupLayout = HorizontalLayout::create(context, groupBox);
        auto hLayout = HorizontalLayout::create(context, groupLayout);
        auto label = Label::create(context, "One", hLayout);
        label = Label::create(context, "Two", hLayout);
        hLayout->addSpacer();
        label = Label::create(context, "Three", hLayout);
        auto vLayout = VerticalLayout::create(context, groupLayout);
        label = Label::create(context, "One", vLayout);
        label = Label::create(context, "Two", vLayout);
        vLayout->addSpacer();
        label = Label::create(context, "Three", vLayout);

        // Create grid layouts.
        groupBox = GroupBox::create(context, "Grid Layouts", layout);
        groupLayout = HorizontalLayout::create(context, groupBox);
        auto gridLayout = GridLayout::create(context, groupLayout);
        label = Label::create(context, "One", gridLayout);
        gridLayout->setGridPos(label, 0, 0);
        label = Label::create(context, "Two", gridLayout);
        gridLayout->setGridPos(label, 0, 1);
        label = Label::create(context, "Three", gridLayout);
        gridLayout->setGridPos(label, 1, 0);
        label = Label::create(context, "Four", gridLayout);
        gridLayout->setGridPos(label, 1, 1);

        // Create form layouts.
        groupBox = GroupBox::create(context, "Form Layouts", layout);
        groupLayout = HorizontalLayout::create(context, groupBox);
        auto formLayout = FormLayout::create(context, groupLayout);
        formLayout->setHStretch(Stretch::Expanding);
        formLayout->addRow("One:", IntEditSlider::create(context));
        formLayout->addRow("Two:", IntEditSlider::create(context));
        formLayout->addSpacer();
        formLayout->addRow("Three:", IntEdit::create(context));
    }

    Layouts::~Layouts()
    {}

    std::shared_ptr<IWidget> Layouts::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Layouts>(new Layouts);
        out->_init(context, app, parent);
        return out;
    }

    Size2I Layouts::getSizeHint() const
    {
        return _scrollWidget->getSizeHint();
    }

    void Layouts::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }
}
