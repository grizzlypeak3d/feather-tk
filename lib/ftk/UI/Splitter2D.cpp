// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Splitter2D.h>

#include <ftk/Core/Math.h>

namespace ftk
{
    namespace
    {
        //! Which division a drag is moving.
        enum class Drag
        {
            None,

            //! The upright division, which moves the split across.
            X,

            //! The flat division, which moves the split down.
            Y,

            //! Where they cross, which moves both.
            Both
        };
    }

    struct Splitter2D::Private
    {
        V2F split = V2F(.5F, .5F);
        bool border = true;
        std::function<void(const V2F&)> splitCallback;

        struct SizeData
        {
            bool init = true;
            int handle = 0;
            int border = 0;

            //! The upright division, the flat one, and the square where they
            //! cross. The square is part of both, and is hit first.
            Box2I x = Box2I(0, 0, -1, -1);
            Box2I y = Box2I(0, 0, -1, -1);
            Box2I both = Box2I(0, 0, -1, -1);

            std::vector<Box2I> borders;
        };
        SizeData size;

        struct MouseData
        {
            Drag hover = Drag::None;
            Drag pressed = Drag::None;
        };
        MouseData mouse;
    };

    void Splitter2D::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::Splitter2D", parent);
        setStretch(Stretch::Expanding);
    }

    Splitter2D::Splitter2D() :
        _p(new Private)
    {}

    Splitter2D::~Splitter2D()
    {}

    std::shared_ptr<Splitter2D> Splitter2D::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Splitter2D>(new Splitter2D);
        out->_init(context, parent);
        return out;
    }

    void Splitter2D::setWidgets(const std::vector<std::shared_ptr<IWidget> >& value)
    {
        auto children = getChildren();
        for (const auto& child : children)
        {
            child->setParent(nullptr);
        }
        for (const auto& widget : value)
        {
            if (widget)
            {
                widget->setParent(shared_from_this());
            }
        }
    }

    const V2F& Splitter2D::getSplit() const
    {
        return _p->split;
    }

    void Splitter2D::setSplit(const V2F& value)
    {
        FTK_P();
        if (value == p.split)
            return;
        p.split = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    void Splitter2D::setSplitCallback(const std::function<void(const V2F&)>& value)
    {
        _p->splitCallback = value;
    }

    bool Splitter2D::hasBorder() const
    {
        return _p->border;
    }

    void Splitter2D::setBorder(bool value)
    {
        FTK_P();
        if (value == p.border)
            return;
        p.border = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    Size2I Splitter2D::getSizeHint() const
    {
        FTK_P();
        // The widest of the left column and of the right, plus the division
        // between them; likewise down. Anything smaller and a quadrant is
        // asked to be smaller than its contents.
        Size2I a, b, c, d;
        const auto& children = getChildren();
        auto i = children.begin();
        if (i != children.end()) { a = (*i)->getSizeHint(); ++i; }
        if (i != children.end()) { b = (*i)->getSizeHint(); ++i; }
        if (i != children.end()) { c = (*i)->getSizeHint(); ++i; }
        if (i != children.end()) { d = (*i)->getSizeHint(); }
        Size2I out;
        out.w = std::max(a.w, c.w) + std::max(b.w, d.w) + p.size.handle;
        out.h = std::max(a.h, b.h) + std::max(c.h, d.h) + p.size.handle;
        return out;
    }

    void Splitter2D::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        FTK_P();
        const Box2I& g = getGeometry();

        std::vector<std::shared_ptr<IWidget> > children;
        for (const auto& child : getChildren())
        {
            if (child->isVisible(false))
            {
                children.push_back(child);
            }
        }

        const int half = p.size.handle / 2;
        const int sx = g.w() * p.split.x;
        const int sy = g.h() * p.split.y;
        const int x0 = g.min.x;
        const int y0 = g.min.y;
        const int x1 = g.min.x + sx + half;
        const int y1 = g.min.y + sy + half;
        const int w0 = sx - half;
        const int h0 = sy - half;
        const int w1 = g.w() - (sx + half);
        const int h1 = g.h() - (sy + half);

        const std::vector<Box2I> childGeometry =
        {
            Box2I(x0, y0, w0, h0),
            Box2I(x1, y0, w1, h0),
            Box2I(x0, y1, w0, h1),
            Box2I(x1, y1, w1, h1)
        };
        for (size_t i = 0; i < children.size() && i < childGeometry.size(); ++i)
        {
            children[i]->setGeometry(childGeometry[i]);
        }

        p.size.x = Box2I(g.min.x + sx - half, g.min.y, p.size.handle, g.h());
        p.size.y = Box2I(g.min.x, g.min.y + sy - half, g.w(), p.size.handle);
        p.size.both = Box2I(
            g.min.x + sx - half,
            g.min.y + sy - half,
            p.size.handle,
            p.size.handle);

        // The border is the outline of the plus the two divisions make, not
        // two lines laid across each other. Run each edge the whole way and
        // they meet in the middle as a hash, drawn twice where they cross; so
        // the upright edges carry on through the crossing to its far side and
        // the flat ones stop at it, which leaves the corners closed and
        // nothing overlapping.
        const int b = p.size.border;
        const int vx0 = p.size.x.min.x;
        const int vx1 = p.size.x.max.x + 1;
        const int hy0 = p.size.y.min.y;
        const int hy1 = p.size.y.max.y + 1;
        const int gx1 = g.max.x + 1;
        const int gy1 = g.max.y + 1;
        const int above = (hy0 + b) - g.min.y;
        const int below = gy1 - (hy1 - b);
        p.size.borders =
        {
            Box2I(vx0,     g.min.y,  b, above),
            Box2I(vx1 - b, g.min.y,  b, above),
            Box2I(vx0,     hy1 - b,  b, below),
            Box2I(vx1 - b, hy1 - b,  b, below),
            Box2I(g.min.x, hy0,      vx0 - g.min.x, b),
            Box2I(g.min.x, hy1 - b,  vx0 - g.min.x, b),
            Box2I(vx1,     hy0,      gx1 - vx1, b),
            Box2I(vx1,     hy1 - b,  gx1 - vx1, b)
        };
    }

    void Splitter2D::styleEvent(const StyleEvent& event)
    {
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
        }
    }

    void Splitter2D::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
        }
    }

    void Splitter2D::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        IWidget::drawEvent(drawRect, event);
        FTK_P();
        if (!p.size.x.isValid())
            return;

        const Drag state = Drag::None != p.mouse.pressed ?
            p.mouse.pressed :
            p.mouse.hover;
        if (Drag::None != state)
        {
            const ColorRole role = Drag::None != p.mouse.pressed ?
                ColorRole::Pressed :
                ColorRole::Hover;
            // Dragging the crossing moves both divisions, so both light up.
            if (Drag::X == state || Drag::Both == state)
            {
                event.render->drawRect(p.size.x, event.style->getColorRole(role));
            }
            if (Drag::Y == state || Drag::Both == state)
            {
                event.render->drawRect(p.size.y, event.style->getColorRole(role));
            }
        }
        if (p.border)
        {
            event.render->drawRects(
                p.size.borders,
                event.style->getColorRole(ColorRole::Border));
        }
    }

    void Splitter2D::mouseEnterEvent(MouseEnterEvent& event)
    {
        IWidget::mouseEnterEvent(event);
        FTK_P();
        // The crossing is part of both divisions, so it is asked about first.
        const Drag hover =
            contains(p.size.both, event.pos) ? Drag::Both :
            contains(p.size.x, event.pos) ? Drag::X :
            contains(p.size.y, event.pos) ? Drag::Y :
            Drag::None;
        if (hover != p.mouse.hover)
        {
            if (Drag::None != hover)
            {
                event.accept = true;
            }
            p.mouse.hover = hover;
            setDrawUpdate();
        }
    }

    void Splitter2D::mouseLeaveEvent()
    {
        FTK_P();
        if (Drag::None != p.mouse.hover)
        {
            p.mouse.hover = Drag::None;
            setDrawUpdate();
        }
    }

    void Splitter2D::mouseMoveEvent(MouseMoveEvent& event)
    {
        FTK_P();
        event.accept = true;
        if (Drag::None != p.mouse.pressed)
        {
            const Box2I& g = getGeometry();
            V2F split = p.split;
            if (Drag::X == p.mouse.pressed || Drag::Both == p.mouse.pressed)
            {
                split.x = clamp(event.pos.x - g.min.x, 0, g.w() - 1) /
                    static_cast<float>(g.w());
            }
            if (Drag::Y == p.mouse.pressed || Drag::Both == p.mouse.pressed)
            {
                split.y = clamp(event.pos.y - g.min.y, 0, g.h() - 1) /
                    static_cast<float>(g.h());
            }
            if (split != p.split)
            {
                p.split = split;
                setSizeUpdate();
                setDrawUpdate();
                if (p.splitCallback)
                {
                    p.splitCallback(p.split);
                }
            }
        }
        else
        {
            const Drag hover =
                contains(p.size.both, event.pos) ? Drag::Both :
                contains(p.size.x, event.pos) ? Drag::X :
                contains(p.size.y, event.pos) ? Drag::Y :
                Drag::None;
            if (hover != p.mouse.hover)
            {
                p.mouse.hover = hover;
                setDrawUpdate();
            }
        }
    }

    void Splitter2D::mousePressEvent(MouseClickEvent& event)
    {
        FTK_P();
        const Drag pressed =
            contains(p.size.both, event.pos) ? Drag::Both :
            contains(p.size.x, event.pos) ? Drag::X :
            contains(p.size.y, event.pos) ? Drag::Y :
            Drag::None;
        if (Drag::None != pressed)
        {
            event.accept = true;
            p.mouse.pressed = pressed;
            setDrawUpdate();
        }
    }

    void Splitter2D::mouseReleaseEvent(MouseClickEvent& event)
    {
        FTK_P();
        event.accept = true;
        p.mouse.pressed = Drag::None;
        setDrawUpdate();
    }
}
