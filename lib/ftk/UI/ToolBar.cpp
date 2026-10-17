// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ToolBar.h>

#include <ftk/UI/Action.h>
#include <ftk/UI/Divider.h>
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
        std::vector<std::shared_ptr<Action> > actions;

        struct Item
        {
            std::shared_ptr<IWidget> widget;
            std::vector<std::shared_ptr<Action> > actions;
            bool divider = false;
            bool overflow = false;
        };
        std::vector<Item> items;
        size_t overflow = 0;

        struct SizeData
        {
            int margin = 0;
            int spacing = 0;
        };
        SizeData size;

        bool isShown(const Item& item) const
        {
            return item.widget->isVisible(false) || item.overflow;
        }

        std::vector<bool> hidden(size_t count) const
        {
            std::vector<bool> out(items.size(), false);
            size_t dropped = 0;
            for (size_t i = items.size(); i > 0; --i)
            {
                const Item& item = items[i - 1];
                if (!isShown(item))
                {
                    continue;
                }
                if (dropped >= count)
                {
                    break;
                }
                out[i - 1] = true;
                if (!item.divider)
                {
                    ++dropped;
                }
            }
            // A divider with nothing after it goes too.
            if (count > 0)
            {
                for (size_t i = items.size(); i > 0; --i)
                {
                    const Item& item = items[i - 1];
                    if (!isShown(item) || out[i - 1])
                    {
                        continue;
                    }
                    if (item.divider)
                    {
                        out[i - 1] = true;
                    }
                    break;
                }
            }
            return out;
        }
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
            p.actions.push_back(action);
            Private::Item item;
            item.widget = out;
            item.actions.push_back(action);
            p.items.push_back(item);
        }
        return out;
    }

    void ToolBar::addOverflowAction(const std::shared_ptr<Action>& action)
    {
        FTK_P();
        p.actions.push_back(action);
        if (!p.items.empty())
        {
            p.items.back().actions.push_back(action);
        }
    }

    const std::vector<std::shared_ptr<Action> >& ToolBar::getActions() const
    {
        return _p->actions;
    }

    void ToolBar::addWidget(const std::shared_ptr<IWidget>& widget)
    {
        FTK_P();
        widget->setParent(p.layout);
        Private::Item item;
        item.widget = widget;
        item.divider = std::dynamic_pointer_cast<Divider>(widget) != nullptr;
        p.items.push_back(item);
    }

    size_t ToolBar::getOverflowMax() const
    {
        FTK_P();
        size_t out = 0;
        for (const auto& item : p.items)
        {
            if (p.isShown(item) && !item.divider)
            {
                ++out;
            }
        }
        return out;
    }

    int ToolBar::getOverflowWidth(size_t count) const
    {
        FTK_P();
        const std::vector<bool> hidden = p.hidden(count);
        int out = 0;
        size_t n = 0;
        for (size_t i = 0; i < p.items.size(); ++i)
        {
            if (p.isShown(p.items[i]) && !hidden[i])
            {
                out += p.items[i].widget->getSizeHint().w;
                ++n;
            }
        }
        if (n > 1)
        {
            out += p.size.spacing * static_cast<int>(n - 1);
        }
        return n > 0 ? out + p.size.margin * 2 : 0;
    }

    size_t ToolBar::getOverflow() const
    {
        return _p->overflow;
    }

    void ToolBar::setOverflow(size_t value)
    {
        FTK_P();
        if (value == p.overflow)
            return;
        p.overflow = value;
        // Only what changes is shown or hidden, so laying out the same
        // overflow again does not ask for another layout.
        const std::vector<bool> hidden = p.hidden(value);
        for (size_t i = 0; i < p.items.size(); ++i)
        {
            auto& item = p.items[i];
            if (hidden[i] != item.overflow)
            {
                item.overflow = hidden[i];
                item.widget->setVisible(!hidden[i]);
            }
        }
    }

    std::vector<std::shared_ptr<Action> > ToolBar::getOverflowActions() const
    {
        std::vector<std::shared_ptr<Action> > out;
        for (const auto& item : _p->items)
        {
            if (item.overflow)
            {
                out.insert(out.end(), item.actions.begin(), item.actions.end());
            }
        }
        return out;
    }

    void ToolBar::clear()
    {
        FTK_P();
        auto children = p.layout->getChildren();
        for (const auto& child : children)
        {
            child->setParent(nullptr);
        }
        p.actions.clear();
        p.items.clear();
        p.overflow = 0;
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
        p.size.margin = event.style->getSizeRole(p.layout->getMarginRole(), event.displayScale);
        p.size.spacing = event.style->getSizeRole(
            p.grouped ? SizeRole::None : p.spacingRole,
            event.displayScale);
    }
}