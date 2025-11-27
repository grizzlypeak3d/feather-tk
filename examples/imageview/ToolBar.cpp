// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "ToolBar.h"

#include "Actions.h"

#include <ftk/UI/Divider.h>
#include <ftk/UI/ToolBar.h>

using namespace ftk;

namespace imageview
{
    void ToolBar::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<Actions>& actions,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "examples::imageview::ToolBar", parent);

        // Create the layout.
        _layout = HorizontalLayout::create(context, shared_from_this());
        _layout->setSpacingRole(SizeRole::SpacingSmall);

        // Create the tool bars.
        _createFileToolBar(context, actions);
        Divider::create(context, Orientation::Horizontal, _layout);
        _createWindowToolBar(context, actions);
        Divider::create(context, Orientation::Horizontal, _layout);
        _createViewToolBar(context, actions);
    }

    ToolBar::~ToolBar()
    {}

    std::shared_ptr<ToolBar> ToolBar::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<Actions>& actions,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ToolBar>(new ToolBar);
        out->_init(context, actions, parent);
        return out;
    }

    void ToolBar::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _layout->setGeometry(value);
    }

    void ToolBar::sizeHintEvent(const SizeHintEvent& event)
    {
        setSizeHint(_layout->getSizeHint());
    }

    void ToolBar::_createFileToolBar(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<Actions>& actions)
    {
        auto toolBar = ftk::ToolBar::create(context, Orientation::Horizontal, _layout);
        for (const auto& key :
            { "File/Open", "File/Close", "File/CloseAll" })
        {
            toolBar->addAction(actions->getAction(key));
        }
    }

    void ToolBar::_createWindowToolBar(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<Actions>& actions)
    {
        auto toolBar = ftk::ToolBar::create(context, Orientation::Horizontal, _layout);
        for (const auto& key :
            { "Window/FullScreen" })
        {
            toolBar->addAction(actions->getAction(key));
        }
    }

    void ToolBar::_createViewToolBar(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<Actions>& actions)
    {
        auto toolBar = ftk::ToolBar::create(context, Orientation::Horizontal, _layout);

        toolBar->addAction(actions->getAction("View/Frame"));
        toolBar->addAction(actions->getAction("View/ZoomReset"));
        auto button = toolBar->addAction(actions->getAction("View/ZoomIn"));
        button->setRepeatClick(true);
        button = toolBar->addAction(actions->getAction("View/ZoomOut"));
        button->setRepeatClick(true);

        button = toolBar->addAction(actions->getAction("View/Red"));
        button->setText("R");
        button = toolBar->addAction(actions->getAction("View/Green"));
        button->setText("G");
        button = toolBar->addAction(actions->getAction("View/Blue"));
        button->setText("B");
        button = toolBar->addAction(actions->getAction("View/Alpha"));
        button->setText("A");
    }
}
