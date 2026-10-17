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
            bool overflow = false;
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
            if (group.overflow)
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
            const Size2I hint = group.toolBar->getSizeHint();
            if (!first && group.divider)
            {
                out.w += p.size.spacing * 2 + group.divider->getSizeHint().w;
            }
            first = false;
            out.w += hint.w;
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

        // What is shown, and how wide each piece is.
        std::vector<size_t> shown;
        for (size_t i = 0; i < p.groups.size(); ++i)
        {
            p.groups[i].overflow = false;
            if (p.groups[i].toolBar->isVisible(false))
            {
                shown.push_back(i);
            }
        }
        const auto dividerWidth = [&p](size_t i)
        {
            return p.groups[i].divider ?
                p.groups[i].divider->getSizeHint().w + p.size.spacing * 2 :
                0;
        };
        const auto widthOf = [&p, &dividerWidth](const std::vector<size_t>& indexes)
        {
            int out = 0;
            for (size_t k = 0; k < indexes.size(); ++k)
            {
                if (k > 0)
                {
                    out += dividerWidth(indexes[k]);
                }
                out += p.groups[indexes[k]].toolBar->getSizeHint().w;
            }
            return out;
        };

        // Drop whole tool bars until what is left and the button fit, from
        // the end of the row; the last one, when it is kept, goes only
        // when it is all that is left.
        std::vector<size_t> kept = shown;
        bool overflow = widthOf(kept) > g.w();
        if (overflow)
        {
            const int buttonWidth =
                p.overflowButton->getSizeHint().w + p.size.spacing;
            while (!kept.empty() && widthOf(kept) + buttonWidth > g.w())
            {
                auto drop = kept.end() - 1;
                if (p.keepLast && kept.size() > 1)
                {
                    drop = kept.end() - 2;
                }
                p.groups[*drop].overflow = true;
                kept.erase(drop);
            }
        }

        // Left to right. What does not fit goes past the end of the row,
        // where it is clipped.
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
        for (size_t i = 0; i < p.groups.size(); ++i)
        {
            auto& group = p.groups[i];
            const bool here =
                std::find(kept.begin(), kept.end(), i) != kept.end();
            if (group.divider)
            {
                if (here && !first)
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
            if (here)
            {
                x += place(group.toolBar, x);
                first = false;
            }
            else if (group.toolBar->isVisible(false))
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
        // The actions of each tool bar that did not fit, in the order they
        // sit in the row, a divider between one tool bar and the next.
        p.menu = Menu::create(context);
        bool first = true;
        for (const auto& group : p.groups)
        {
            if (!group.overflow)
            {
                continue;
            }
            const auto& actions = group.toolBar->getActions();
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
