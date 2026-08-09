// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FlowLayout.h>

#include <ftk/UI/LayoutUtil.h>

#include <algorithm>

namespace ftk
{
    struct FlowLayout::Private
    {
        SizeRole marginRole = SizeRole::None;
        SizeRole spacingRole = SizeRole::SpacingSmall;

        struct SizeData
        {
            bool init = true;
            int margin = 0;
            int spacing = 0;

            //! The height the last width needed, and the width it needed it
            //! for. Held so that the size hint can report something better
            //! than a guess once the layout has been placed at least once.
            int flowHeight = 0;
            int flowWidth = 0;
        };
        SizeData size;
    };

    void FlowLayout::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::FlowLayout", parent);
    }

    FlowLayout::FlowLayout() :
        _p(new Private)
    {}

    FlowLayout::~FlowLayout()
    {}

    std::shared_ptr<FlowLayout> FlowLayout::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FlowLayout>(new FlowLayout);
        out->_init(context, parent);
        return out;
    }

    SizeRole FlowLayout::getMarginRole() const
    {
        return _p->marginRole;
    }

    void FlowLayout::setMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.marginRole)
            return;
        p.marginRole = value;
        p.size.init = true;
        setSizeUpdate();
        setDrawUpdate();
    }

    SizeRole FlowLayout::getSpacingRole() const
    {
        return _p->spacingRole;
    }

    void FlowLayout::setSpacingRole(SizeRole value)
    {
        FTK_P();
        if (value == p.spacingRole)
            return;
        p.spacingRole = value;
        p.size.init = true;
        setSizeUpdate();
        setDrawUpdate();
    }

    int FlowLayout::_flow(const Box2I& g, bool place)
    {
        FTK_P();
        const int width = g.w();
        int x = 0;
        int y = 0;
        int lineHeight = 0;
        for (const auto& child : getChildren())
        {
            if (!child->isVisible(false))
                continue;
            const Size2I size = child->getSizeHint();
            // Wrap before placing, never after, so a child that would hang off
            // the end starts the next line instead. The first on a line is
            // placed whatever its width: there is nowhere narrower to put it.
            if (x > 0 && x + size.w > width)
            {
                x = 0;
                y += lineHeight + p.size.spacing;
                lineHeight = 0;
            }
            if (place)
            {
                child->setGeometry(Box2I(
                    g.min.x + x,
                    g.min.y + y,
                    size.w,
                    size.h));
            }
            x += size.w + p.size.spacing;
            lineHeight = std::max(lineHeight, size.h);
        }
        return y + lineHeight;
    }

    Size2I FlowLayout::getSizeHint() const
    {
        FTK_P();
        // As wide as the widest child and no wider: anything more and a
        // parent that hands out size hints would never give the layout a
        // reason to wrap.
        Size2I out;
        for (const auto& child : getChildren())
        {
            if (child->isVisible(false))
            {
                const Size2I size = child->getSizeHint();
                out.w = std::max(out.w, size.w);
                out.h = std::max(out.h, size.h);
            }
        }
        if (p.size.flowWidth > 0)
        {
            out.h = p.size.flowHeight;
        }
        out.w += p.size.margin * 2;
        out.h += p.size.margin * 2;
        return out;
    }

    void FlowLayout::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        FTK_P();
        const Box2I g = margin(getGeometry(), -p.size.margin);
        const int height = _flow(g, true);
        if (height != p.size.flowHeight || g.w() != p.size.flowWidth)
        {
            // The height only becomes knowable here, where the width is. Ask
            // to be measured again with it; the second pass agrees, because
            // the parent's width does not depend on our height.
            p.size.flowHeight = height;
            p.size.flowWidth = g.w();
            setSizeUpdate();
        }
    }

    void FlowLayout::styleEvent(const StyleEvent& event)
    {
        IWidget::styleEvent(event);
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
        }
    }

    void FlowLayout::sizeHintEvent(const SizeHintEvent& event)
    {
        IWidget::sizeHintEvent(event);
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.margin = event.style->getSizeRole(p.marginRole, event.displayScale);
            p.size.spacing = event.style->getSizeRole(p.spacingRole, event.displayScale);
            // The wrap depends on both, so what was measured before no longer
            // applies.
            p.size.flowWidth = 0;
            p.size.flowHeight = 0;
        }
    }
}
