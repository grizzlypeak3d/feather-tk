// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Splitter.h>

#include <ftk/UI/LayoutUtil.h>

#include <optional>

namespace ftk
{
    struct Splitter::Private
    {
        Orientation orientation = Orientation::Horizontal;
        float split = .5F;
        bool border = true;

        struct SizeData
        {
            std::optional<float> displayScale;
            int handle = 0;
            int border = 0;

            Box2I g = Box2I(0, 0, -1, -1);
            Box2I g2;
            Box2I g3;
        };
        SizeData size;

        struct MouseData
        {
            bool hoverHandle = false;
            bool pressedHandle = false;
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

    void Splitter::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        FTK_P();

        const Box2I& g = getGeometry();
        const int w = g.w();
        const int h = g.h();

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
                const int s = g.w() * p.split;
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
                break;
            }
            case Orientation::Vertical:
            {
                const int s = g.h() * p.split;
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

    void Splitter::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();

        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
        }

        Size2I sizeHint;
        const auto& children = getChildren();
        auto i = children.begin();
        if (children.size() > 0)
        {
            sizeHint = (*i)->getSizeHint();
            ++i;
        }
        if (children.size() > 1)
        {
            const auto& childSizeHint = (*i)->getSizeHint();
            sizeHint.w = std::max(sizeHint.w, childSizeHint.w);
            sizeHint.h = std::max(sizeHint.h, childSizeHint.h);
            switch (p.orientation)
            {
            case Orientation::Horizontal:
                sizeHint.w += p.size.handle;
                break;
            case Orientation::Vertical:
                sizeHint.h += p.size.handle;
                break;
            default: break;
            }
        }
        setSizeHint(sizeHint);
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
                p.split = clamp(event.pos.x - g.min.x, 0, w - 1) / static_cast<float>(w);
                break;
            case Orientation::Vertical:
                p.split = clamp(event.pos.y - g.min.y, 0, h - 1) / static_cast<float>(h);
                break;
            default: break;
            }
            setSizeUpdate();
            setDrawUpdate();
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
        if (contains(p.size.g, event.pos))
        {
            event.accept = true;
            p.mouse.pressedHandle = true;
            setDrawUpdate();
        }
    }

    void Splitter::mouseReleaseEvent(MouseClickEvent& event)
    {
        FTK_P();
        event.accept = true;
        p.mouse.pressedHandle = false;
        setDrawUpdate();
    }
}