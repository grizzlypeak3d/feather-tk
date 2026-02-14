// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Lists.h"

#include <ftk/UI/ListWidget.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Format.h>

using namespace ftk;

namespace widgets
{
    void Lists::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        ftk::IWidget::_init(context, "Lists", parent);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create a clickable list widget.
        auto listWidget = ListWidget::create(context, ButtonGroupType::Click, layout);
        std::vector<std::string> items;
        for (size_t i = 0; i < 100; ++i)
        {
            items.push_back(Format("Click {0}").arg(i));
        }
        listWidget->setItems(items);
        listWidget->setCallback(
            [](int index, bool)
            {
                std::cout << "Click: " << index << std::endl;
            });

        // Create a checkable list widget.
        listWidget = ListWidget::create(context, ButtonGroupType::Check, layout);
        items.clear();
        for (size_t i = 0; i < 100; ++i)
        {
            items.push_back(Format("Check {0}").arg(i));
        }
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
        listWidget->setCurrent(static_cast<int>(items.size()) - 1);

        // Create a radio button list widget.
        listWidget = ListWidget::create(context, ButtonGroupType::Radio, layout);
        items.clear();
        for (size_t i = 0; i < 100; ++i)
        {
            items.push_back(Format("Radio {0}").arg(i));
        }
        listWidget->setItems(items);
        listWidget->setChecked(0);
        listWidget->setCallback(
            [](int index, bool)
            {
                std::cout << "Radio: " << index << std::endl;
            });

        // Create a toggle button list widget.
        listWidget = ListWidget::create(context, ButtonGroupType::Toggle, layout);
        items.clear();
        for (size_t i = 0; i < 100; ++i)
        {
            items.push_back(Format("Toggle {0}").arg(i));
        }
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
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Lists>(new Lists);
        out->_init(context, parent);
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
