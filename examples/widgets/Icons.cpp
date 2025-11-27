// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Icons.h"

#include <ftk/UI/GridLayout.h>
#include <ftk/UI/Icon.h>
#include <ftk/UI/IconSystem.h>
#include <ftk/UI/Label.h>

using namespace ftk;

namespace widgets
{
    void Icons::_init(const std::shared_ptr<Context>& context)
    {
        ftk::IWidget::_init(context, "Icons", nullptr);

        // Create a layout.
        auto layout = GridLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        layout->setSpacingRole(SizeRole::None);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create icons.
        auto iconSystem = context->getSystem<IconSystem>();
        int row = 0;
        for (const auto& name : iconSystem->getNames())
        {
            auto icon = Icon::create(context, name, layout);
            layout->setGridPos(icon, row, 0);
            auto label = Label::create(context, name, layout);
            label->setMarginRole(SizeRole::MarginSmall);
            layout->setGridPos(label, row, 1);
            ++row;
        }
    }

    Icons::~Icons()
    {}

    std::shared_ptr<Icons> Icons::create(const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<Icons>(new Icons);
        out->_init(context);
        return out;
    }

    void Icons::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }

    void Icons::sizeHintEvent(const SizeHintEvent& event)
    {
        setSizeHint(_scrollWidget->getSizeHint());
    }
}
