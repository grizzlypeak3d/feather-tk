// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Bellows.h"

#include <ftk/UI/Bellows.h>
#include <ftk/UI/CheckBox.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/Core/Format.h>

using namespace ftk;

namespace widgets
{
    void Bellows::_init(const std::shared_ptr<Context>& context)
    {
        ftk::IWidget::_init(context, "Bellows", nullptr);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        //layout->setMarginRole(SizeRole::None);
        layout->setSpacingRole(SizeRole::None);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Add a bellows.
        auto vLayout = VerticalLayout::create(context);
        vLayout->setMarginRole(SizeRole::Margin);
        vLayout->setSpacingRole(SizeRole::SpacingSmall);
        for (size_t i = 0; i < 10; ++i)
        {
            PushButton::create(context, Format("Push Button {0}").arg(i), vLayout);
        }
        auto bellows = ftk::Bellows::create(context, "Push Buttons", layout);
        bellows->setWidget(vLayout);

        // Add a bellows.
        vLayout = VerticalLayout::create(context);
        vLayout->setMarginRole(SizeRole::Margin);
        vLayout->setSpacingRole(SizeRole::SpacingSmall);
        const std::vector<std::string> icons =
        {
            "Audio",
            "Close",
            "Copy",
            "Directory",
            "File",
            "FrameNext",
            "FramePrev",
            "Mute",
            "Search",
            "Volume"
        };
        for (size_t i = 0; i < 10; ++i)
        {
            auto button = ToolButton::create(context, Format("Tool Button {0}").arg(i), vLayout);
            if (i < icons.size())
            {
                button->setIcon(icons[i]);
            }
        }
        bellows = ftk::Bellows::create(context, "Tool Buttons", layout);
        bellows->setWidget(vLayout);

        // Add a bellows.
        vLayout = VerticalLayout::create(context);
        vLayout->setMarginRole(SizeRole::Margin);
        vLayout->setSpacingRole(SizeRole::SpacingSmall);
        for (size_t i = 0; i < 10; ++i)
        {
            CheckBox::create(context, Format("Check Box {0}").arg(i), vLayout);
        }
        bellows = ftk::Bellows::create(context, "Check Boxes", layout);
        bellows->setWidget(vLayout);
    }

    Bellows::~Bellows()
    {}

    std::shared_ptr<Bellows> Bellows::create(const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<Bellows>(new Bellows);
        out->_init(context);
        return out;
    }

    Size2I Bellows::getSizeHint() const
    {
        return _scrollWidget->getSizeHint();
    }

    void Bellows::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }
}
