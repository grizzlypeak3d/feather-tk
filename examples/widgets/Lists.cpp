// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Lists.h"

#include <ftk/UI/GroupBox.h>
#include <ftk/UI/ListWidget.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Format.h>

using namespace ftk;

namespace widgets
{
    void Lists::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        ftk::IWidget::_init(context, "Lists", parent);

        // Create a layout: a captioned column per list type, side by side.
        auto layout = HorizontalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        std::vector<std::string> items;
        for (size_t i = 0; i < 40; ++i)
        {
            items.push_back(Format("Item {0}").arg(i));
        }

        // Create a clickable list widget.
        auto groupBox = GroupBox::create(context, "Click", layout);
        auto listWidget = ListWidget::create(context, ButtonGroupType::Click, groupBox);
        listWidget->setItems(items);
        listWidget->setCallback(
            [](int index, bool)
            {
                std::cout << "Click: " << index << std::endl;
            });

        // Create a checkable list widget.
        groupBox = GroupBox::create(context, "Check", layout);
        listWidget = ListWidget::create(context, ButtonGroupType::Check, groupBox);
        listWidget->setItems(items);
        listWidget->setChecked(0);
        listWidget->setChecked(1);
        listWidget->setChecked(2);
        listWidget->setChecked(4);
        listWidget->setCallback(
            [](int index, bool value)
            {
                std::cout << "Check: " << index << std::endl;
            });

        // Create a radio button list widget.
        groupBox = GroupBox::create(context, "Radio", layout);
        listWidget = ListWidget::create(context, ButtonGroupType::Radio, groupBox);
        listWidget->setItems(items);
        listWidget->setChecked(0);
        listWidget->setCallback(
            [](int index, bool)
            {
                std::cout << "Radio: " << index << std::endl;
            });

        // Create a toggle button list widget.
        groupBox = GroupBox::create(context, "Toggle", layout);
        listWidget = ListWidget::create(context, ButtonGroupType::Toggle, groupBox);
        listWidget->setItems(items);
        listWidget->setChecked(0);
        listWidget->setCallback(
            [](int index, bool)
            {
                std::cout << "Toggle: " << index << std::endl;
            });
    }

    Lists::~Lists()
    {}

    std::shared_ptr<IWidget> Lists::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Lists>(new Lists);
        out->_init(context, app, parent);
        return out;
    }
    
    Size2I Lists::getSizeHint() const
    {
        return _scrollWidget->getSizeHint();
    }

    void Lists::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }
}
