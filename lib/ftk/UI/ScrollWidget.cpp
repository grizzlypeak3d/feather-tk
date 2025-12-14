// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ScrollWidget.h>

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/GridLayout.h>
#include <ftk/UI/ScrollArea.h>
#include <ftk/UI/ScrollBar.h>

#include <optional>

namespace ftk
{
    struct ScrollWidget::Private
    {
        bool scrollBarsVisible = true;
        bool scrollBarsAutoHide = true;
        bool scrollEventsEnabled = true;
        int lineStep = 20;
        bool border = true;
        SizeRole marginRole = SizeRole::None;

        std::shared_ptr<IWidget> widget;
        std::shared_ptr<ScrollArea> scrollArea;
        std::shared_ptr<ScrollBar> hScrollBar;
        std::shared_ptr<ScrollBar> vScrollBar;
        std::shared_ptr<GridLayout> layout;

        std::function<void(const V2I&)> scrollPosCallback;

        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
            int border = 0;
        };
        SizeData size;
    };

    void ScrollWidget::_init(
        const std::shared_ptr<Context>& context,
        ScrollType scrollType,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::ScrollWidget", parent);
        FTK_P();

        p.scrollArea = ScrollArea::create(context, scrollType);
        p.scrollArea->setStretch(Stretch::Expanding);

        p.hScrollBar = ScrollBar::create(context, Orientation::Horizontal);
        p.vScrollBar = ScrollBar::create(context, Orientation::Vertical);

        p.layout = GridLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::None);
        p.scrollArea->setParent(p.layout);
        p.layout->setGridPos(p.scrollArea, 0, 0);
        p.hScrollBar->setParent(p.layout);
        p.layout->setGridPos(p.hScrollBar, 1, 0);
        p.vScrollBar->setParent(p.layout);
        p.layout->setGridPos(p.vScrollBar, 0, 1);

        _scrollBarsUpdate();

        p.hScrollBar->setScrollPosCallback(
            [this](int value)
            {
                FTK_P();
                p.scrollArea->setScrollPos(V2I(value, p.vScrollBar->getScrollPos()));
            });

        p.vScrollBar->setScrollPosCallback(
            [this](int value)
            {
                FTK_P();
                p.scrollArea->setScrollPos(V2I(p.hScrollBar->getScrollPos(), value));
            });

        p.scrollArea->setScrollSizeCallback(
            [this](const Size2I& value)
            {
                FTK_P();
                p.hScrollBar->setScrollSize(value.w);
                p.vScrollBar->setScrollSize(value.h);
            });

        p.scrollArea->setScrollPosCallback(
            [this](const V2I& value)
            {
                FTK_P();
                p.hScrollBar->setScrollPos(value.x);
                p.vScrollBar->setScrollPos(value.y);
                if (p.scrollPosCallback)
                {
                    p.scrollPosCallback(value);
                }
            });
    }

    ScrollWidget::ScrollWidget() :
        _p(new Private)
    {}

    ScrollWidget::~ScrollWidget()
    {}

    std::shared_ptr<ScrollWidget> ScrollWidget::create(
        const std::shared_ptr<Context>& context,
        ScrollType scrollType,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ScrollWidget>(new ScrollWidget);
        out->_init(context, scrollType, parent);
        return out;
    }

    const std::shared_ptr<IWidget>& ScrollWidget::getWidget() const
    {
        return _p->widget;
    }

    void ScrollWidget::setWidget(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        if (p.widget)
        {
            p.widget->setParent(nullptr);
        }
        p.widget = value;
        if (p.widget)
        {
            p.widget->setParent(_p->scrollArea);
        }
        setSizeUpdate();
        setDrawUpdate();
    }

    Box2I ScrollWidget::getViewport() const
    {
        return _p->scrollArea->getChildrenClipRect();
    }

    ScrollType ScrollWidget::getScrollType() const
    {
        return _p->scrollArea->getScrollType();
    }

    void ScrollWidget::setScrollType(ScrollType value)
    {
        FTK_P();
        const bool changed = value != p.scrollArea->getScrollType();
        p.scrollArea->setScrollType(value);
        if (changed)
        {
            _scrollBarsUpdate();
        }
    }

    const Size2I& ScrollWidget::getScrollSize() const
    {
        return _p->scrollArea->getScrollSize();
    }

    const V2I& ScrollWidget::getScrollPos() const
    {
        return _p->scrollArea->getScrollPos();
    }

    void ScrollWidget::setScrollPos(const V2I& value, bool clamp)
    {
        _p->scrollArea->setScrollPos(value, clamp);
    }

    void ScrollWidget::scrollTo(const Box2I& value)
    {
        _p->scrollArea->scrollTo(value);
    }

    void ScrollWidget::setScrollPosCallback(const std::function<void(const V2I&)>& value)
    {
        _p->scrollPosCallback = value;
    }

    bool ScrollWidget::isAreaResizable() const
    {
        return _p->scrollArea->isAreaResizable();
    }

    void ScrollWidget::setAreaResizable(bool value)
    {
        _p->scrollArea->setAreaResizable(value);
    }

    bool ScrollWidget::areScrollBarsVisible() const
    {
        return _p->scrollBarsVisible;
    }

    void ScrollWidget::setScrollBarsVisible(bool value)
    {
        FTK_P();
        if (value == p.scrollBarsVisible)
            return;
        p.scrollBarsVisible = value;
        _scrollBarsUpdate();
    }

    bool ScrollWidget::getScrollBarsAutoHide() const
    {
        return _p->scrollBarsAutoHide;
    }

    void ScrollWidget::setScrollBarsAutoHide(bool value)
    {
        FTK_P();
        if (value == p.scrollBarsAutoHide)
            return;
        p.scrollBarsAutoHide = value;
        _scrollBarsUpdate();
    }

    bool ScrollWidget::areScrollEventsEnabled() const
    {
        return _p->scrollEventsEnabled;
    }

    void ScrollWidget::setScrollEventsEnabled(bool value)
    {
        _p->scrollEventsEnabled = value;
    }

    int ScrollWidget::getLineStep() const
    {
        return _p->lineStep;
    }

    void ScrollWidget::setLineStep(int value)
    {
        _p->lineStep = value;
    }

    SizeRole ScrollWidget::getMarginRole() const
    {
        return _p->marginRole;
    }

    void ScrollWidget::setMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.marginRole)
            return;
        p.marginRole = value;
        p.size.displayScale.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    bool ScrollWidget::hasBorder() const
    {
        return _p->border;
    }

    void ScrollWidget::setBorder(bool value)
    {
        FTK_P();
        if (value == p.border)
            return;
        p.border = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    SizeRole ScrollWidget::getSizeHintRole() const
    {
        return _p->scrollArea->getSizeHintRole();
    }

    void ScrollWidget::setSizeHintRole(SizeRole value)
    {
        _p->scrollArea->setSizeHintRole(value);
    }

    void ScrollWidget::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        FTK_P();
        Box2I g = value;
        if (p.border || p.marginRole != SizeRole::None)
        {
            g = margin(g, -std::max(p.size.margin, p.size.border));
        }
        p.layout->setGeometry(g);
        _scrollBarsUpdate();
    }

    void ScrollWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();

        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale != event.displayScale))
        {
            p.size = Private::SizeData();
            p.size.displayScale = event.displayScale;
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.margin = event.style->getSizeRole(p.marginRole, event.displayScale);
        }

        Size2I sizeHint = _p->layout->getSizeHint();
        if (p.border || p.marginRole != SizeRole::None)
        {
            sizeHint = margin(sizeHint, std::max(p.size.margin, p.size.border));
        }
        setSizeHint(sizeHint);
    }

    void ScrollWidget::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IWidget::drawEvent(drawRect, event);
        FTK_P();
        if (p.border)
        {
            const Box2I& g = getGeometry();
            Box2I g2 = g;
            if (p.marginRole != SizeRole::None)
            {
                g2 = margin(g2, -(p.size.margin - p.size.border));
            }
            event.render->drawMesh(
                border(g2, p.size.border),
                event.style->getColorRole(ColorRole::Border));
        }
    }

    void ScrollWidget::scrollEvent(ScrollEvent& event)
    {
        IWidget::scrollEvent(event);
        FTK_P();
        if (p.scrollEventsEnabled)
        {
            event.accept = true;
            int lineStep = p.lineStep;
            if (p.size.displayScale.has_value())
            {
                lineStep *= p.size.displayScale.value();
            }
            V2I scrollPos = getScrollPos();
            switch (p.scrollArea->getScrollType())
            {
            case ScrollType::Horizontal:
                scrollPos.x -= event.value.y * lineStep;
                break;
            default:
                scrollPos.y -= event.value.y * lineStep;
                break;
            }
            setScrollPos(scrollPos);
        }
    }

    void ScrollWidget::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        if (0 == event.modifiers)
        {
            switch (event.key)
            {
            case Key::PageUp:
            {
                event.accept = true;
                V2I scrollPos = getScrollPos();
                scrollPos.y -= _getPageStep();
                setScrollPos(scrollPos);
                break;
            }
            case Key::PageDown:
            {
                event.accept = true;
                V2I scrollPos = getScrollPos();
                scrollPos.y += _getPageStep();
                setScrollPos(scrollPos);
                break;
            }
            default: break;
            }
        }
        if (!event.accept)
        {
            IWidget::keyPressEvent(event);
        }
    }

    void ScrollWidget::keyReleaseEvent(KeyEvent& event)
    {
        IWidget::keyReleaseEvent(event);
        event.accept = true;
    }

    int ScrollWidget::_getPageStep() const
    {
        FTK_P();
        const Size2I scrollAreaSize = p.scrollArea->getGeometry().size();
        return scrollAreaSize.h;
    }

    void ScrollWidget::_scrollBarsUpdate()
    {
        FTK_P();
        const Size2I scrollSize = p.scrollArea->getScrollSize();
        const Size2I scrollAreaSize = p.scrollArea->getGeometry().size();
        bool hVisible = p.scrollBarsVisible;
        bool vVisible = p.scrollBarsVisible;
        switch (p.scrollArea->getScrollType())
        {
        case ScrollType::Horizontal:
            vVisible = false;
            break;
        case ScrollType::Vertical:
        case ScrollType::Menu:
            hVisible = false;
            break;
        default: break;
        }
        if (p.scrollBarsAutoHide)
        {
            hVisible &= scrollSize.w > scrollAreaSize.w;
            vVisible &= scrollSize.h > scrollAreaSize.h;
        }
        p.hScrollBar->setVisible(hVisible);
        p.vScrollBar->setVisible(vVisible);
    }
}
