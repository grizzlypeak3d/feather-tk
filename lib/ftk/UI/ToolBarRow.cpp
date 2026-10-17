// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ToolBarRow.h>

#include <ftk/UI/Action.h>
#include <ftk/UI/Divider.h>
#include <ftk/UI/Menu.h>
#include <ftk/UI/ScreenshotTag.h>
#include <ftk/UI/ToolBar.h>
#include <ftk/UI/ToolButton.h>

#include <algorithm>

namespace ftk
{
    namespace
    {
        struct Group
        {
            std::shared_ptr<ToolBar> toolBar;
            std::shared_ptr<Divider> divider;
            bool present = true;
        };
    }

    struct ToolBarRow::Private
    {
        std::vector<Group> groups;
        std::shared_ptr<ToolButton> overflowButton;
        std::shared_ptr<Menu> menu;
        bool keepLast = false;
        SizeRole marginRole = SizeRole::None;
        SizeRole spacingRole = SizeRole::SpacingSmall;
        struct SizeData
        {
            int margin = 0;
            int spacing = 0;
        };
        SizeData size;
    };

    void ToolBarRow::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::ToolBarRow", parent);
        FTK_P();
        // The tool bars that do not fit are put past the end of the row,
        // where clipping keeps them from being drawn or pressed; hiding
        // them would fight whoever hides and shows them on purpose.
        setClipChildren(true);
        p.overflowButton = ToolButton::create(context, shared_from_this());
        p.overflowButton->setIcon("ToolBarOverflow");
        p.overflowButton->setTooltip("The tools that do not fit");
        setScreenshotTag(p.overflowButton, "ToolBarRow.Overflow");
        p.overflowButton->setVisible(false);
        p.overflowButton->setPressedCallback(
            [this]
            {
                _openMenu();
            });
    }

    ToolBarRow::ToolBarRow() :
        _p(new Private)
    {}

    ToolBarRow::~ToolBarRow()
    {}

    std::shared_ptr<ToolBarRow> ToolBarRow::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ToolBarRow>(new ToolBarRow);
        out->_init(context, parent);
        return out;
    }

    void ToolBarRow::addToolBar(const std::shared_ptr<ToolBar>& toolBar)
    {
        FTK_P();
        if (auto context = getContext())
        {
            Group group;
            group.toolBar = toolBar;
            if (!p.groups.empty())
            {
                group.divider = Divider::create(context, Orientation::Horizontal, shared_from_this());
            }
            toolBar->setParent(shared_from_this());
            p.groups.push_back(group);
            // The button stays last, above the tool bars.
            p.overflowButton->setParent(nullptr);
            p.overflowButton->setParent(shared_from_this());
            setSizeUpdate();
            setDrawUpdate();
        }
    }

    std::vector<std::shared_ptr<ToolBar> > ToolBarRow::getOverflow() const
    {
        std::vector<std::shared_ptr<ToolBar> > out;
        for (const auto& group : _p->groups)
        {
            if (group.toolBar->getOverflow() > 0)
            {
                out.push_back(group.toolBar);
            }
        }
        return out;
    }

    bool ToolBarRow::isKeepLast() const
    {
        return _p->keepLast;
    }

    void ToolBarRow::setKeepLast(bool value)
    {
        FTK_P();
        if (value == p.keepLast)
            return;
        p.keepLast = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    SizeRole ToolBarRow::getMarginRole() const
    {
        return _p->marginRole;
    }

    void ToolBarRow::setMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.marginRole)
            return;
        p.marginRole = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    SizeRole ToolBarRow::getSpacingRole() const
    {
        return _p->spacingRole;
    }

    void ToolBarRow::setSpacingRole(SizeRole value)
    {
        FTK_P();
        if (value == p.spacingRole)
            return;
        p.spacingRole = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    Size2I ToolBarRow::getSizeHint() const
    {
        // Every tool bar that is shown, side by side: the width the row
        // wants. Given less, it overflows rather than asking for more.
        FTK_P();
        Size2I out;
        bool first = true;
        for (const auto& group : p.groups)
        {
            if (!group.toolBar->isVisible(false))
            {
                continue;
            }
            // The width with every button shown, whatever is hidden now:
            // what the row would like, not what it settled for.
            const Size2I hint = group.toolBar->getSizeHint();
            if (!first && group.divider)
            {
                out.w += p.size.spacing * 2 + group.divider->getSizeHint().w;
            }
            first = false;
            out.w += group.toolBar->getOverflowWidth(0);
            out.h = std::max(out.h, hint.h);
        }
        out.w += p.size.margin * 2;
        out.h += p.size.margin * 2;
        return out;
    }

    void ToolBarRow::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        FTK_P();
        const Box2I g = margin(value, -p.size.margin);

        // The tool bars that are shown, and how many of their buttons can
        // go. Worked out in full before anything is shown or hidden, so
        // that laying out the same width again changes nothing.
        std::vector<size_t> shown;
        std::vector<size_t> max(p.groups.size(), 0);
        std::vector<size_t> count(p.groups.size(), 0);
        for (size_t i = 0; i < p.groups.size(); ++i)
        {
            const auto& toolBar = p.groups[i].toolBar;
            if (toolBar->isVisible(false) || toolBar->getOverflow() > 0)
            {
                max[i] = toolBar->getOverflowMax();
                if (max[i] > 0)
                {
                    shown.push_back(i);
                }
            }
        }
        const auto width = [&p, &shown, &max, &count]
        {
            int out = 0;
            bool first = true;
            for (size_t i : shown)
            {
                if (count[i] >= max[i])
                {
                    continue;
                }
                if (!first && p.groups[i].divider)
                {
                    out += p.groups[i].divider->getSizeHint().w + p.size.spacing * 2;
                }
                first = false;
                out += p.groups[i].toolBar->getOverflowWidth(count[i]);
            }
            return out;
        };

        // Buttons go from the end of the row, one at a time: the last tool
        // bar's own last, when it is kept.
        const bool overflow = width() > g.w();
        if (overflow)
        {
            const int buttonWidth = p.overflowButton->getSizeHint().w + p.size.spacing;
            std::vector<size_t> order(shown.rbegin(), shown.rend());
            if (p.keepLast && order.size() > 1)
            {
                order.push_back(order.front());
                order.erase(order.begin());
            }
            for (size_t i : order)
            {
                while (count[i] < max[i] && width() + buttonWidth > g.w())
                {
                    ++count[i];
                }
                if (width() + buttonWidth <= g.w())
                {
                    break;
                }
            }
        }
        for (size_t i = 0; i < p.groups.size(); ++i)
        {
            p.groups[i].toolBar->setOverflow(count[i]);
            p.groups[i].present =
                std::find(shown.begin(), shown.end(), i) != shown.end() &&
                count[i] < max[i];
        }

        // Left to right. A tool bar with nothing left goes past the end of
        // the row, where it is clipped, with its divider.
        const auto place = [&g](const std::shared_ptr<IWidget>& widget, int x)
        {
            const Size2I hint = widget->getSizeHint();
            int y = g.min.y;
            int h = g.h();
            if (VAlign::Center == widget->getVAlign())
            {
                y = g.min.y + g.h() / 2 - hint.h / 2;
                h = hint.h;
            }
            widget->setGeometry(Box2I(x, y, hint.w, h));
            return hint.w;
        };
        int x = g.min.x;
        const int past = value.max.x + 1;
        bool first = true;
        for (auto& group : p.groups)
        {
            if (group.divider)
            {
                if (group.present && !first)
                {
                    x += p.size.spacing;
                    x += place(group.divider, x);
                    x += p.size.spacing;
                }
                else
                {
                    place(group.divider, past);
                }
            }
            if (group.present)
            {
                x += place(group.toolBar, x);
                first = false;
            }
            else
            {
                place(group.toolBar, past);
            }
        }
        p.overflowButton->setVisible(overflow);
        if (overflow)
        {
            const Size2I hint = p.overflowButton->getSizeHint();
            p.overflowButton->setGeometry(Box2I(
                g.max.x - hint.w + 1,
                g.min.y + g.h() / 2 - hint.h / 2,
                hint.w,
                hint.h));
        }
        else if (p.menu)
        {
            p.menu->close();
        }
    }

    void ToolBarRow::sizeHintEvent(const SizeHintEvent& event)
    {
        IWidget::sizeHintEvent(event);
        FTK_P();
        p.size.margin = event.style->getSizeRole(p.marginRole, event.displayScale);
        p.size.spacing = event.style->getSizeRole(p.spacingRole, event.displayScale);
    }

    void ToolBarRow::childRemoveEvent(const ChildRemoveEvent& event)
    {
        IWidget::childRemoveEvent(event);
        FTK_P();
        const auto i = std::find_if(
            p.groups.begin(),
            p.groups.end(),
            [&event](const Group& group)
            {
                return group.toolBar == event.child;
            });
        if (i != p.groups.end())
        {
            if (i->divider)
            {
                i->divider->setParent(nullptr);
            }
            p.groups.erase(i);
            if (!p.groups.empty() && p.groups.front().divider)
            {
                p.groups.front().divider->setParent(nullptr);
                p.groups.front().divider.reset();
            }
        }
        setSizeUpdate();
        setDrawUpdate();
    }

    void ToolBarRow::_openMenu()
    {
        FTK_P();
        if (p.menu)
        {
            p.menu->close();
            return;
        }
        auto context = getContext();
        auto window = getWindow();
        if (!context || !window)
        {
            return;
        }
        // The actions of the buttons that did not fit, in the order they
        // sit in the row, a divider between one tool bar and the next.
        p.menu = Menu::create(context);
        bool first = true;
        for (const auto& group : p.groups)
        {
            const auto actions = group.toolBar->getOverflowActions();
            if (actions.empty())
            {
                continue;
            }
            if (!first)
            {
                p.menu->addDivider();
            }
            first = false;
            for (const auto& action : actions)
            {
                p.menu->addAction(action);
            }
        }
        if (p.menu->isEmpty())
        {
            p.menu.reset();
            return;
        }
        std::weak_ptr<ToolBarRow> weak(
            std::dynamic_pointer_cast<ToolBarRow>(shared_from_this()));
        p.menu->setCloseCallback(
            [weak]
            {
                if (auto widget = weak.lock())
                {
                    widget->_p->menu.reset();
                }
            });
        p.menu->open(window, p.overflowButton->getGeometry());
    }
}
