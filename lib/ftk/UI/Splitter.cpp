// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Splitter.h>

#include <ftk/UI/LayoutUtil.h>

#include <algorithm>
#include <optional>

namespace ftk
{
    struct Splitter::Private
    {
        Orientation orientation = Orientation::Horizontal;
        float split = .5F;
        bool border = true;
        std::function<void(float)> splitCallback;

        struct SizeData
        {
            bool init = true;
            int handle = 0;
            int border = 0;
            int reach = 0;
            Box2I g = Box2I(0, 0, -1, -1);
            Box2I g2;
            Box2I g3;
            Box2I grip;
            Box2I touch = Box2I(0, 0, -1, -1);
        };
        SizeData size;

        struct MouseData
        {
            bool hoverHandle = false;
            bool pressedHandle = false;
            int offset = 0;
            float pressSplit = 0.F;
        };
        MouseData mouse;
    };

    void Splitter::_init(
        const std::shared_ptr<Context>& context,
        Orientation orientation,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::Splitter", parent);
        FTK_P();
        setStretch(Stretch::Expanding);
        p.orientation = orientation;
    }

    Splitter::Splitter() :
        _p(new Private)
    {}

    Splitter::~Splitter()
    {}

    std::shared_ptr<Splitter> Splitter::create(
        const std::shared_ptr<Context>& context,
        Orientation orientation,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Splitter>(new Splitter);
        out->_init(context, orientation, parent);
        return out;
    }

    void Splitter::setWidgets(const std::vector<std::shared_ptr<IWidget> >& value)
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

    float Splitter::getSplit() const
    {
        return _p->split;
    }

    void Splitter::setSplit(float value)
    {
        FTK_P();
        if (value == p.split)
            return;
        p.split = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    void Splitter::setSplitCallback(const std::function<void(float)>& value)
    {
        _p->splitCallback = value;
    }

    bool Splitter::hasBorder() const
    {
        return _p->border;
    }

    void Splitter::setBorder(bool value)
    {
        FTK_P();
        if (value == p.border)
            return;
        p.border = value;
        setDrawUpdate();
    }

    Size2I Splitter::getSizeHint() const
    {
        FTK_P();
        Size2I out;
        const auto& children = getChildren();
        auto i = children.begin();
        if (children.size() > 0)
        {
            out = (*i)->getSizeHint();
            ++i;
        }
        if (children.size() > 1)
        {
            const auto& childSizeHint = (*i)->getSizeHint();
            out.w = std::max(out.w, childSizeHint.w);
            out.h = std::max(out.h, childSizeHint.h);
            switch (p.orientation)
            {
            case Orientation::Horizontal:
                out.w += p.size.handle;
                break;
            case Orientation::Vertical:
                out.h += p.size.handle;
                break;
            default: break;
            }
        }
        return out;
    }

    namespace
    {
        //! How far either side of the split a fingertip takes the handle,
        //! at a display scale of one: a band about as wide as a finger, the
        //! reach the viewport's drawing tools give one too.
        const int fingerReach = 22;
    }

    bool Splitter::isTouchHandle(const V2I& pos) const
    {
        FTK_P();
        return p.size.touch.isValid() && contains(p.size.touch, pos);
    }

    int Splitter::_split(int size) const
    {
        FTK_P();
        // The handle straddles the split, so a split hard against either end
        // leaves half of it outside the widget, drawn over whatever the
        // widget's neighbour happens to be. Kept away from the ends by half a
        // handle, which still collapses a child to nothing: the handle simply
        // sits flush against the edge rather than across it.
        const int half = p.size.handle / 2;
        return clamp(
            static_cast<int>(size * p.split),
            half,
            std::max(size - half, half));
    }

    void Splitter::setGeometry(const Box2I& value)
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

        std::vector<Box2I> childGeometry;
        p.size.g = Box2I(0, 0, -1, -1);
        p.size.touch = Box2I(0, 0, -1, -1);
        if (1 == children.size())
        {
            childGeometry.push_back(g);
        }
        else if (children.size() > 1)
        {
            switch (p.orientation)
            {
            case Orientation::Horizontal:
            {
                const int s = _split(g.w());
                childGeometry.push_back(Box2I(
                    g.min.x,
                    g.min.y,
                    s - p.size.handle / 2,
                    g.h()));
                childGeometry.push_back(Box2I(
                    g.min.x + s + p.size.handle / 2,
                    g.min.y,
                    g.w() - (s + p.size.handle / 2),
                    g.h()));
                p.size.g = Box2I(
                    g.min.x + s - p.size.handle / 2,
                    g.min.y,
                    p.size.handle,
                    g.h());
                p.size.g2 = Box2I(
                    p.size.g.min.x,
                    p.size.g.min.y,
                    p.size.border,
                    p.size.g.h());
                p.size.g3 = Box2I(
                    p.size.g.max.x + 1 - p.size.border,
                    p.size.g.min.y,
                    p.size.border,
                    p.size.g.h());
                const int reach = std::max(p.size.reach, p.size.handle / 2);
                p.size.touch = intersect(
                    Box2I(g.min.x + s - reach, g.min.y, reach * 2, g.h()),
                    g);
                const int gripW = std::max(1, p.size.handle / 4);
                const int gripH = std::min(p.size.handle * 3, p.size.g.h());
                p.size.grip = Box2I(
                    p.size.g.min.x + (p.size.g.w() - gripW) / 2,
                    p.size.g.min.y + (p.size.g.h() - gripH) / 2,
                    gripW,
                    gripH);
                break;
            }
            case Orientation::Vertical:
            {
                const int s = _split(g.h());
                childGeometry.push_back(Box2I(
                    g.min.x,
                    g.min.y,
                    g.w(),
                    s - p.size.handle / 2));
                childGeometry.push_back(Box2I(
                    g.min.x,
                    g.min.y + s + p.size.handle / 2,
                    g.w(),
                    g.h() - (s + p.size.handle / 2)));
                p.size.g = Box2I(
                    g.min.x,
                    g.min.y + s - p.size.handle / 2,
                    g.w(),
                    p.size.handle);
                p.size.g2 = Box2I(
                    p.size.g.min.x,
                    p.size.g.min.y,
                    p.size.g.w(),
                    p.size.border);
                p.size.g3 = Box2I(
                    p.size.g.min.x,
                    p.size.g.max.y + 1 - p.size.border,
                    p.size.g.w(),
                    p.size.border);
                const int reach = std::max(p.size.reach, p.size.handle / 2);
                p.size.touch = intersect(
                    Box2I(g.min.x, g.min.y + s - reach, g.w(), reach * 2),
                    g);
                const int gripW = std::min(p.size.handle * 3, p.size.g.w());
                const int gripH = std::max(1, p.size.handle / 4);
                p.size.grip = Box2I(
                    p.size.g.min.x + (p.size.g.w() - gripW) / 2,
                    p.size.g.min.y + (p.size.g.h() - gripH) / 2,
                    gripW,
                    gripH);
                break;
            }
            default: break;
            }
        }

        for (size_t i = 0; i < children.size() && i < childGeometry.size(); ++i)
        {
            children[i]->setGeometry(childGeometry[i]);
        }
    }

    void Splitter::styleEvent(const StyleEvent& event)
    {
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
        }
    }

    void Splitter::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.reach = static_cast<int>(fingerReach * event.displayScale);
        }
    }

    void Splitter::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IWidget::drawEvent(drawRect, event);
        FTK_P();
        if (p.size.g.isValid())
        {
            if (p.mouse.pressedHandle)
            {
                event.render->drawRect(
                    p.size.g,
                    event.style->getColorRole(ColorRole::Pressed));
            }
            else if (p.mouse.hoverHandle)
            {
                event.render->drawRect(
                    p.size.g,
                    event.style->getColorRole(ColorRole::Hover));
            }
            if (p.border)
            {
                event.render->drawRect(
                    p.size.g2,
                    event.style->getColorRole(ColorRole::Border));
                event.render->drawRect(
                    p.size.g3,
                    event.style->getColorRole(ColorRole::Border));
            }
            // A grip, so that the handle reads as something to take hold
            // of: there is no hover to show it under a finger. Flat, in
            // the button color, a step up from the window's.
            event.render->drawRect(
                p.size.grip,
                event.style->getColorRole(ColorRole::Button));
        }
    }

    void Splitter::mouseEnterEvent(MouseEnterEvent& event)
    {
        IWidget::mouseEnterEvent(event);
        FTK_P();
        if (contains(p.size.g, event.pos) && !p.mouse.hoverHandle)
        {
            event.accept = true;
            p.mouse.hoverHandle = true;
            setDrawUpdate();
        }
    }

    void Splitter::mouseLeaveEvent()
    {
        FTK_P();
        if (p.mouse.hoverHandle)
        {
            p.mouse.hoverHandle = false;
            setDrawUpdate();
        }
    }

    void Splitter::mouseMoveEvent(MouseMoveEvent& event)
    {
        FTK_P();
        event.accept = true;
        if (p.mouse.pressedHandle)
        {
            const Box2I& g = getGeometry();
            const int w = g.w();
            const int h = g.h();
            switch (p.orientation)
            {
            case Orientation::Horizontal:
                p.split = clamp(event.pos.x - p.mouse.offset - g.min.x, 0, w - 1) / static_cast<float>(w);
                break;
            case Orientation::Vertical:
                p.split = clamp(event.pos.y - p.mouse.offset - g.min.y, 0, h - 1) / static_cast<float>(h);
                break;
            default: break;
            }
            setSizeUpdate();
            setDrawUpdate();
            if (p.splitCallback)
            {
                p.splitCallback(p.split);
            }
        }
        else if (contains(p.size.g, event.pos) && !p.mouse.hoverHandle)
        {
            p.mouse.hoverHandle = true;
            setDrawUpdate();
        }
        else if (!contains(p.size.g, event.pos) && p.mouse.hoverHandle)
        {
            p.mouse.hoverHandle = false;
            setDrawUpdate();
        }
    }

    void Splitter::mousePressEvent(MouseClickEvent& event)
    {
        FTK_P();
        if (contains(p.size.g, event.pos) ||
            (event.touch && isTouchHandle(event.pos)))
        {
            event.accept = true;
            p.mouse.pressedHandle = true;
            p.mouse.pressSplit = p.split;
            // Where the press is from the split, kept while dragging: a
            // finger takes the handle from beside it, and the split moves
            // with the finger rather than jumping under it.
            const Box2I& g = getGeometry();
            switch (p.orientation)
            {
            case Orientation::Horizontal:
                p.mouse.offset = event.pos.x - (g.min.x + _split(g.w()));
                break;
            case Orientation::Vertical:
                p.mouse.offset = event.pos.y - (g.min.y + _split(g.h()));
                break;
            default: break;
            }
            setDrawUpdate();
        }
    }

    void Splitter::mouseReleaseEvent(MouseClickEvent& event)
    {
        FTK_P();
        event.accept = true;
        if (event.cancel && p.mouse.pressedHandle && p.split != p.mouse.pressSplit)
        {
            // A second finger made it a gesture: the split goes back.
            p.split = p.mouse.pressSplit;
            setSizeUpdate();
            if (p.splitCallback)
            {
                p.splitCallback(p.split);
            }
        }
        p.mouse.pressedHandle = false;
        setDrawUpdate();
    }
}