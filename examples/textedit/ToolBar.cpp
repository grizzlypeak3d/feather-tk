// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "ToolBar.h"

#include "Actions.h"

#include <ftk/UI/Divider.h>
#include <ftk/UI/ToolBar.h>

using namespace ftk;

namespace textedit
{
    void ToolBar::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<Actions>& actions,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "examples::textedit::ToolBar", parent);

        // Create the layout.
        _layout = HorizontalLayout::create(context, shared_from_this());
        _layout->setSpacingRole(SizeRole::SpacingSmall);

        // Create the file tool bar.
        auto fileToolBar = ftk::ToolBar::create(context, Orientation::Horizontal, _layout);
        for (const auto& key :
            { "File/New", "File/Open", "File/Close", "File/CloseAll", "File/Save" })
        {
            fileToolBar->addAction(actions->getAction(key));
        }
        Divider::create(context, Orientation::Horizontal, _layout);

        // Create the edit tool bar.
        auto editToolBar = ftk::ToolBar::create(context, Orientation::Horizontal, _layout);
        for (const auto& key :
            { "Edit/Undo", "Edit/Redo", "Edit/Cut", "Edit/Copy", "Edit/Paste" })
        {
            editToolBar->addAction(actions->getAction(key));
        }
        Divider::create(context, Orientation::Horizontal, _layout);

        // Create the window tool bar.
        auto windowToolBar = ftk::ToolBar::create(context, Orientation::Horizontal, _layout);
        for (const auto& key :
            { "Window/FullScreen" })
        {
            windowToolBar->addAction(actions->getAction(key));
        }
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
}
