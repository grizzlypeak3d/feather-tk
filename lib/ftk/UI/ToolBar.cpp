// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ToolBar.h>

#include <ftk/UI/Action.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ToolButton.h>

#include <array>

namespace ftk
{
    struct ToolBar::Private
    {
        Orientation orientation = Orientation::Horizontal;
        std::shared_ptr<RowLayout> layout;
        SizeRole spacingRole = SizeRole::SpacingTool;
        bool grouped = false;
    };

    void ToolBar::_init(
        const std::shared_ptr<Context>& context,
        Orientation orientation,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::ToolBar", parent);
        FTK_P();
        p.orientation = orientation;
        switch (orientation)
        {
        case Orientation::Horizontal:
            p.layout = HorizontalLayout::create(context);
            _setWidget(p.layout);
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
        FTK_P();
        p.spacingRole = value;
        p.layout->setSpacingRole(p.grouped ? SizeRole::None : value);
    }

    bool ToolBar::isGrouped() const
    {
        return _p->grouped;
    }

    void ToolBar::setGrouped(bool value)
    {
        FTK_P();
        if (value == p.grouped)
            return;
        p.grouped = value;
        // The buttons of a run touch; what separates the runs is the other
        // widgets and whatever the tool bar sits in.
        p.layout->setSpacingRole(p.grouped ? SizeRole::None : p.spacingRole);
        setSizeUpdate();
        setDrawUpdate();
    }

    void ToolBar::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();

        // Where each member sits in its run, worked out here so that it
        // follows widgets being shown and hidden as well as added. Left
        // alone unless grouped: an ungrouped tool bar's buttons keep
        // whatever roles and corners they were given.
        if (p.grouped)
        {
            std::vector<std::shared_ptr<IWidget> > run;
            const auto flush = [&p, &run]
            {
                for (size_t i = 0; i < run.size(); ++i)
                {
                    const bool first = 0 == i;
                    const bool last = run.size() - 1 == i;
                    // Top left, top right, bottom right, bottom left.
                    run[i]->setSegment(
                        ColorRole::Header,
                        Orientation::Horizontal == p.orientation ?
                        std::array<bool, 4>{ first, last, last, first } :
                        std::array<bool, 4>{ first, first, last, last });
                }
                run.clear();
            };
            for (const auto& child : p.layout->getChildren())
            {
                if (!child->isVisible(false))
                {
                    continue;
                }
                if (child->isSegment())
                {
                    run.push_back(child);
                }
                else
                {
                    flush();
                }
            }
            flush();
        }

        IContainer::sizeHintEvent(event);
    }
}