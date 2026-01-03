// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Stack.h"

#include <ftk/UI/Divider.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/StackLayout.h>

using namespace ftk;

namespace widgets
{
    void Stack::_init(const std::shared_ptr<Context>& context)
    {
        ftk::IWidget::_init(context, "Stack", nullptr);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        layout->setSpacingRole(SizeRole::None);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create a stack of labels.
        auto stackLayout = StackLayout::create(context, layout);
        stackLayout->setStretch(Stretch::Expanding);
        auto label = Label::create(context, "One", stackLayout);
        FontInfo fontInfo;
        fontInfo.size = 64;
        label->setFontInfo(fontInfo);
        label->setAlign(HAlign::Center, VAlign::Center);
        label = Label::create(context, "Two", stackLayout);
        label->setFontInfo(fontInfo);
        label->setAlign(HAlign::Center, VAlign::Center);
        label = Label::create(context, "Three", stackLayout);
        label->setFontInfo(fontInfo);
        label->setAlign(HAlign::Center, VAlign::Center);
        label = Label::create(context, "Four", stackLayout);
        label->setFontInfo(fontInfo);
        label->setAlign(HAlign::Center, VAlign::Center);

        // Create buttons to change the current label.
        Divider::create(context, Orientation::Vertical, layout);
        auto hLayout = HorizontalLayout::create(context, layout);
        hLayout->setMarginRole(SizeRole::Margin);

        auto nextButton = PushButton::create(context, "Next", hLayout);
        nextButton->setClickedCallback(
            [stackLayout]
            {
                stackLayout->nextIndex();
            });
        _hasNextObserver = Observer<bool>::create(
            stackLayout->observeHasNextIndex(),
            [nextButton](bool value)
            {
                nextButton->setEnabled(value);
            });

        auto prevButton = PushButton::create(context, "Previous", hLayout);
        prevButton->setClickedCallback(
            [stackLayout]
            {
                stackLayout->prevIndex();
            });
        _hasPrevObserver = Observer<bool>::create(
            stackLayout->observeHasPrevIndex(),
            [prevButton](bool value)
            {
                prevButton->setEnabled(value);
            });
    }

    Stack::~Stack()
    {}

    std::shared_ptr<Stack> Stack::create(const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<Stack>(new Stack);
        out->_init(context);
        return out;
    }
    
    Size2I Stack::getSizeHint() const
    {
        return _scrollWidget->getSizeHint();
    }

    void Stack::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }
}
