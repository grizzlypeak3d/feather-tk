// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ScrollArea.h>

#include <ftk/UI/DrawUtil.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <optional>
#include <sstream>

namespace ftk
{
    FTK_ENUM_IMPL(
        ScrollType,
        "Both",
        "Horizontal",
        "Vertical",
        "Menu");

    struct ScrollArea::Private
    {
        ScrollType scrollType = ScrollType::Both;
        Size2I scrollSize;
        V2I scrollPos;
        std::function<void(const Size2I&)> scrollSizeCallback;
        std::function<void(const V2I&)> scrollPosCallback;
        SizeRole sizeHintRole = SizeRole::ScrollArea;

        struct SizeData
        {
            std::optional<float> displayScale;
            int size = 0;
        };
        SizeData size;
    };

    void ScrollArea::_init(
        const std::shared_ptr<Context>& context,
        ScrollType scrollType,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::ScrollArea", parent);
        FTK_P();

        setBackgroundRole(ColorRole::Window);

        p.scrollType = scrollType;
    }

    ScrollArea::ScrollArea() :
        _p(new Private)
    {}

    ScrollArea::~ScrollArea()
    {}

    std::shared_ptr<ScrollArea> ScrollArea::create(
        const std::shared_ptr<Context>& context,
        ScrollType scrollType,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ScrollArea>(new ScrollArea);
        out->_init(context, scrollType, parent);
        return out;
    }

    ScrollType ScrollArea::getScrollType() const
    {
        return _p->scrollType;
    }

    void ScrollArea::setScrollType(ScrollType value)
    {
        FTK_P();
        if (value == p.scrollType)
            return;
        p.scrollType = value;
        setSizeUpdate();
    }

    const Size2I& ScrollArea::getScrollSize() const
    {
        return _p->scrollSize;
    }

    void ScrollArea::setScrollSizeCallback(const std::function<void(const Size2I&)>& value)
    {
        _p->scrollSizeCallback = value;
    }

    const V2I& ScrollArea::getScrollPos() const
    {
        return _p->scrollPos;
    }

    void ScrollArea::setScrollPos(const V2I& value, bool clamp)
    {
        FTK_P();
        V2I tmp = value;
        if (clamp)
        {
            const Box2I& g = getGeometry();
            tmp = V2I(
                ftk::clamp(tmp.x, 0, std::max(0, p.scrollSize.w - g.w())),
                ftk::clamp(tmp.y, 0, std::max(0, p.scrollSize.h - g.h())));
        }
        if (tmp == p.scrollPos)
            return;
        p.scrollPos = tmp;
        setSizeUpdate();
        setDrawUpdate();
        if (p.scrollPosCallback)
        {
            p.scrollPosCallback(p.scrollPos);
        }
    }

    void ScrollArea::scrollTo(const Box2I& value)
    {
        FTK_P();
        const Size2I size = getGeometry().size();
        V2I scrollPos = p.scrollPos;
        if (value.max.x > scrollPos.x + size.w)
        {
            scrollPos.x = value.max.x - size.w - 1;
        }
        if (value.min.x < scrollPos.x)
        {
            scrollPos.x = value.min.x;
        }
        if (value.max.y > scrollPos.y + size.h)
        {
            scrollPos.y = value.max.y - size.h + 1;
        }
        if (value.min.y < scrollPos.y)
        {
            scrollPos.y = value.min.y;
        }
        setScrollPos(scrollPos, false);
    }

    void ScrollArea::setScrollPosCallback(const std::function<void(const V2I&)>& value)
    {
        _p->scrollPosCallback = value;
    }

    SizeRole ScrollArea::getSizeHintRole() const
    {
        return _p->sizeHintRole;
    }

    void ScrollArea::setSizeHintRole(SizeRole value)
    {
        FTK_P();
        if (value == p.sizeHintRole)
            return;
        p.sizeHintRole = value;
        p.size.displayScale.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    void ScrollArea::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        FTK_P();

        Size2I scrollSize;
        for (const auto& child : getChildren())
        {
            Size2I childSizeHint = child->getSizeHint();
            switch (p.scrollType)
            {
            case ScrollType::Horizontal:
                childSizeHint.h = std::max(childSizeHint.h, value.h());
                break;
            case ScrollType::Vertical:
            case ScrollType::Menu:
                childSizeHint.w = std::max(childSizeHint.w, value.w());
                break;
            case ScrollType::Both:
                childSizeHint.w = std::max(childSizeHint.w, value.w());
                childSizeHint.h = std::max(childSizeHint.h, value.h());
                break;
            default: break;
            }
            scrollSize.w = std::max(scrollSize.w, childSizeHint.w);
            scrollSize.h = std::max(scrollSize.h, childSizeHint.h);
            const Box2I g2(
                value.min.x - p.scrollPos.x,
                value.min.y - p.scrollPos.y,
                childSizeHint.w,
                childSizeHint.h);
            child->setGeometry(g2);
        }
        if (scrollSize != p.scrollSize)
        {
            p.scrollSize = scrollSize;
            setSizeUpdate();
            setDrawUpdate();
            if (p.scrollSizeCallback)
            {
                p.scrollSizeCallback(p.scrollSize);
            }
        }

        const V2I scrollPos(
            clamp(p.scrollPos.x, 0, std::max(0, p.scrollSize.w - value.w())),
            clamp(p.scrollPos.y, 0, std::max(0, p.scrollSize.h - value.h())));
        if (scrollPos != p.scrollPos)
        {
            p.scrollPos = scrollPos;
            setSizeUpdate();
            setDrawUpdate();
            if (p.scrollPosCallback)
            {
                p.scrollPosCallback(p.scrollPos);
            }
        }
    }

    void ScrollArea::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();

        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.size = event.style->getSizeRole(p.sizeHintRole, event.displayScale);
        }

        Size2I sizeHint;
        for (const auto& child : getChildren())
        {
            const Size2I& childSizeHint = child->getSizeHint();
            sizeHint.w = std::max(sizeHint.w, childSizeHint.w);
            sizeHint.h = std::max(sizeHint.h, childSizeHint.h);
        }
        switch (p.scrollType)
        {
        case ScrollType::Horizontal:
            sizeHint.w = p.size.size;
            break;
        case ScrollType::Vertical:
            sizeHint.h = p.size.size;
            break;
        case ScrollType::Both:
            sizeHint.w = sizeHint.h = p.size.size;
            break;
        default: break;
        }
        _setSizeHint(sizeHint);
    }
}
