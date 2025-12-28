// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ToolBar.h>

#include <ftk/UI/Action.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ToolButton.h>

namespace ftk
{
    struct ToolBar::Private
    {
        std::shared_ptr<RowLayout> layout;
    };

    void ToolBar::_init(
        const std::shared_ptr<Context>& context,
        Orientation orientation,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::ToolBar", parent);
        FTK_P();
        switch (orientation)
        {
        case Orientation::Horizontal:
            p.layout = HorizontalLayout::create(context, shared_from_this());
            p.layout->setSpacingRole(SizeRole::SpacingTool);
            break;
        case Orientation::Vertical:
            p.layout = VerticalLayout::create(context, shared_from_this());
            p.layout->setSpacingRole(SizeRole::SpacingTool);
            break;
        default: break;
        }
    }

    ToolBar::ToolBar() :
        _p(new Private)
    {}

    ToolBar::~ToolBar()
    {}

    std::shared_ptr<ToolBar> ToolBar::create(
        const std::shared_ptr<Context>& context,
        Orientation orientation,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ToolBar>(new ToolBar);
        out->_init(context, orientation, parent);
        return out;
    }

    std::shared_ptr<ToolButton> ToolBar::addAction(const std::shared_ptr<Action>& action)
    {
        FTK_P();
        std::shared_ptr<ToolButton> out;
        if (auto context = getContext())
        {
            out = ToolButton::create(context, action, p.layout);
        }
        return out;
    }

    void ToolBar::addWidget(const std::shared_ptr<IWidget>& widget)
    {
        widget->setParent(_p->layout);
    }

    void ToolBar::clear()
    {
        FTK_P();
        auto children = p.layout->getChildren();
        for (const auto& child : children)
        {
            child->setParent(nullptr);
        }
    }

    SizeRole ToolBar::getMarginRole() const
    {
        return _p->layout->getMarginRole();
    }

    void ToolBar::setMarginRole(SizeRole value)
    {
        _p->layout->setMarginRole(value);
    }

    SizeRole ToolBar::getSpacingRole() const
    {
        return _p->layout->getSpacingRole();
    }

    void ToolBar::setSpacingRole(SizeRole value)
    {
        _p->layout->setSpacingRole(value);
    }

    void ToolBar::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    void ToolBar::sizeHintEvent(const SizeHintEvent& event)
    {
        setSizeHint(_p->layout->getSizeHint());
    }
}