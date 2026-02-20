// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IWindow.h>

#include <ftk/UI/App.h>
#include <ftk/UI/IDialog.h>
#include <ftk/UI/IPopup.h>
#include <ftk/UI/Tooltip.h>

#include <ftk/GL/GL.h>
#include <ftk/GL/OffscreenBuffer.h>

namespace ftk
{
    FTK_ENUM_IMPL(
        WindowBufferType,
        "U8",
        "F32");

    struct IWindow::Private
    {
        std::weak_ptr<App> app;
        std::string title;
        Size2I windowSize;
        Size2I minSize;
        Size2I bufferSize;
        std::shared_ptr<Observable<bool> > fullScreen;
        std::shared_ptr<Observable<bool> > floatOnTop;
        std::shared_ptr<Observable<WindowBufferType> > bufferType;
        std::shared_ptr<Observable<float> > displayScale;
        std::function<void(void)> closeCallback;

        bool inside = false;
        V2I cursorPos;
        V2I cursorPosPrev;
        std::weak_ptr<IWidget> hover;
        std::weak_ptr<IWidget> mousePress;
        MouseClickEvent mouseClickEvent;
        std::weak_ptr<IWidget> keyFocus;
        std::weak_ptr<IWidget> keyPress;
        KeyEvent keyEvent;
        bool textInput = false;

        std::shared_ptr<IDragDropData> dragDropData;
        std::shared_ptr<Image> dragDropCursor;
        V2I dragDropCursorHotspot;
        std::weak_ptr<IWidget> dragDropHover;

        bool tooltipsEnabled = true;
        std::shared_ptr<Tooltip> tooltip;
        V2I tooltipPos;
        std::chrono::steady_clock::time_point tooltipTimer;

        struct SizeData
        {
            int dl = 0;
        };
        SizeData size;
    };

    void IWindow::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::string& title)
    {
        IWidget::_init(context, "ftk::IWindow", nullptr);
        FTK_P();

        p.app = app;
        p.title = title;
        p.fullScreen = Observable<bool>::create(false);
        p.floatOnTop = Observable<bool>::create(false);
        p.bufferType = Observable<WindowBufferType>::create(windowBufferTypeDefault);
        p.displayScale = Observable<float>::create(1.F);

        setBackgroundRole(ColorRole::Window);

        if (app)
        {
            app->_addWindow(std::dynamic_pointer_cast<IWindow>(shared_from_this()));
        }
    }

    IWindow::IWindow() :
        _p(new Private)
    {}

    IWindow::~IWindow()
    {}

    std::shared_ptr<App> IWindow::getApp() const
    {
        return _p->app.lock();
    }

    const std::string& IWindow::getTitle() const
    {
        return _p->title;
    }

    void IWindow::setTitle(const std::string& value)
    {
        _p->title = value;
    }

    const Size2I& IWindow::getSize() const
    {
        return _p->windowSize;
    }

    void IWindow::setSize(const Size2I& value)
    {
        _p->windowSize = value;
    }

    Size2I IWindow::getMinSize() const
    {
        return _p->minSize;
    }

    void IWindow::setMinSize(const Size2I& value)
    {
        _p->minSize = value;
    }

    bool IWindow::isFullScreen() const
    {
        return _p->fullScreen->get();
    }

    std::shared_ptr<IObservable<bool> > IWindow::observeFullScreen() const
    {
        return _p->fullScreen;
    }

    void IWindow::setFullScreen(bool value)
    {
        _p->fullScreen->setIfChanged(value);
    }

    bool IWindow::isFloatOnTop() const
    {
        return _p->floatOnTop->get();
    }

    std::shared_ptr<IObservable<bool> > IWindow::observeFloatOnTop() const
    {
        return _p->floatOnTop;
    }

    void IWindow::setFloatOnTop(bool value)
    {
        _p->floatOnTop->setIfChanged(value);
    }

    const Size2I& IWindow::getBufferSize() const
    {
        return _p->bufferSize;
    }

    WindowBufferType IWindow::getBufferType() const
    {
        return _p->bufferType->get();
    }

    std::shared_ptr<IObservable<WindowBufferType> > IWindow::observeBufferType() const
    {
        return _p->bufferType;
    }

    void IWindow::setBufferType(WindowBufferType value)
    {
        if (_p->bufferType->setIfChanged(value))
        {
            setDrawUpdate();
        }
    }

    float IWindow::getDisplayScale() const
    {
        return _p->displayScale->get();
    }

    std::shared_ptr<IObservable<float> > IWindow::observeDisplayScale() const
    {
        return _p->displayScale;
    }

    void IWindow::setDisplayScale(float value)
    {
        FTK_P();
        if (p.displayScale->setIfChanged(value))
        {
            setSizeUpdate();
            setDrawUpdate();
        }
    }

    float IWindow::getContentScale() const
    {
        FTK_P();
        return p.windowSize.w > 0 ?
            (p.bufferSize.w / static_cast<float>(p.windowSize.w)) :
            0.F;
    }

    std::shared_ptr<IWidget> IWindow::getKeyFocus() const
    {
        return _p->keyFocus.lock();
    }

    void IWindow::setKeyFocus(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        auto prev = p.keyFocus.lock();
        if (value == prev)
            return;
        p.keyFocus = value;
        if (prev)
        {
            prev->keyFocusEvent(false);
            setDrawUpdate();
        }
        if (value)
        {
            value->keyFocusEvent(true);
            setDrawUpdate();
        }
    }

    std::shared_ptr<IWidget> IWindow::getNextKeyFocus(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        std::shared_ptr<IWidget> out;
        const auto& children = getChildren();
        if (!children.empty())
        {
            std::list<std::shared_ptr<IWidget> > widgets;
            _getKeyFocus(children.back(), widgets);
            if (!widgets.empty())
            {
                auto i = std::find(widgets.begin(), widgets.end(), value);
                if (i != widgets.end())
                {
                    ++i;
                    if (i != widgets.end())
                    {
                        out = *i;
                    }
                }
                if (!out)
                {
                    out = widgets.front();
                }
            }
        }
        return out;
    }

    std::shared_ptr<IWidget> IWindow::getPrevKeyFocus(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        std::shared_ptr<IWidget> out;
        const auto& children = getChildren();
        if (!children.empty())
        {
            std::list<std::shared_ptr<IWidget> > widgets;
            _getKeyFocus(children.back(), widgets);
            if (!widgets.empty())
            {
                auto i = std::find(widgets.rbegin(), widgets.rend(), value);
                if (i != widgets.rend())
                {
                    ++i;
                    if (i != widgets.rend())
                    {
                        out = *i;
                    }
                }
                if (!out)
                {
                    out = widgets.back();
                }
            }
        }
        return out;
    }

    bool IWindow::hasTextInput() const
    {
        return _p->textInput;
    }

    void IWindow::setTextInput(bool value)
    {
        _p->textInput = value;
    }

    bool IWindow::getTooltipsEnabled() const
    {
        return _p->tooltipsEnabled;
    }

    void IWindow::setTooltipsEnabled(bool value)
    {
        FTK_P();
        if (value == p.tooltipsEnabled)
            return;
        p.tooltipsEnabled = value;
        if (!value)
        {
            _closeTooltip();
        }
    }

    void IWindow::setIcon(const std::shared_ptr<Image>&)
    {}

    std::shared_ptr<Image> IWindow::screenshot(const Box2I&)
    {
        return nullptr;
    }

    void IWindow::close()
    {
        FTK_P();
        if (auto app = p.app.lock())
        {
            app->_removeWindow(std::dynamic_pointer_cast<IWindow>(shared_from_this()));
        }
        if (p.closeCallback)
        {
            p.closeCallback();
        }
    }

    void IWindow::setCloseCallback(const std::function<void(void)>& value)
    {
        _p->closeCallback = value;
    }

    std::vector<std::pair<std::string, std::string> > IWindow::getWindowInfo() const
    {
        return {};
    }

    void IWindow::setVisible(bool value)
    {
        const bool changed = value != isVisible(false);
        IWidget::setVisible(value);
        FTK_P();
        if (changed && !isVisible(false))
        {
            if (auto hover = p.hover.lock())
            {
                p.hover.reset();
                hover->mouseLeaveEvent();
            }
            if (auto pressed = p.mousePress.lock())
            {
                p.mousePress.reset();
                p.mouseClickEvent.pos = p.cursorPos;
                p.mouseClickEvent.accept = false;
                pressed->mouseReleaseEvent(p.mouseClickEvent);
            }
            if (auto focus = p.keyFocus.lock())
            {
                p.keyFocus.reset();
                focus->keyFocusEvent(false);
            }
            if (auto keyPress = p.keyPress.lock())
            {
                p.keyPress.reset();
                p.keyEvent.pos = p.cursorPos;
                p.keyEvent.accept = false;
                keyPress->keyReleaseEvent(p.keyEvent);
            }
            if (auto dragDropWidget = p.dragDropHover.lock())
            {
                p.dragDropHover.reset();
                DragDropEvent event(
                    p.cursorPos,
                    p.cursorPosPrev,
                    p.dragDropData);
                dragDropWidget->dragLeaveEvent(event);
            }
            p.dragDropData.reset();
            p.dragDropCursor.reset();
            _clipEventRecursive(shared_from_this(), getGeometry(), true);
        }
    }

    void IWindow::tickEvent(
        bool parentsVisible,
        bool parentsEnabled,
        const TickEvent& event)
    {
        IWidget::tickEvent(parentsVisible, parentsEnabled, event);
        FTK_P();
        if (p.inside)
        {
            if (!p.mousePress.lock())
            {
                MouseMoveEvent mouseMoveEvent(p.cursorPos, p.cursorPos);
                _hoverUpdate(mouseMoveEvent);
            }

            if (p.tooltipsEnabled)
            {
                const auto tooltipTime = std::chrono::steady_clock::now();
                const auto tooltipDiff = std::chrono::duration_cast<std::chrono::milliseconds>(tooltipTime - p.tooltipTimer);
                if (tooltipDiff > tooltipTimeout && !p.tooltip && !p.mousePress.lock())
                {
                    if (auto context = getContext())
                    {
                        std::string text;
                        const auto widgets = _getUnderCursor(UnderCursor::Tooltip, p.cursorPos);
                        for (const auto& widget : widgets)
                        {
                            text = widget->getTooltip();
                            if (!text.empty())
                            {
                                break;
                            }
                        }
                        if (!text.empty())
                        {
                            p.tooltip = Tooltip::create(
                                context,
                                text,
                                p.cursorPos,
                                shared_from_this());
                            p.tooltipPos = p.cursorPos;
                        }
                    }
                }
            }
        }
    }

    void IWindow::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        p.size.dl = event.style->getSizeRole(SizeRole::DragLength, event.displayScale);
    }

    void IWindow::drawOverlayEvent(const Box2I& clipRect, const DrawEvent& event)
    {
        FTK_P();
        if (p.dragDropCursor)
        {
            event.render->drawImage(
                p.dragDropCursor,
                Box2F(
                    p.cursorPos.x - p.dragDropCursorHotspot.x,
                    p.cursorPos.y - p.dragDropCursorHotspot.y,
                    p.dragDropCursor->getWidth(),
                    p.dragDropCursor->getHeight()),
                Color4F(1.F, 1.F, 1.F));
        }
    }

    void IWindow::_sizeUpdate(
        const Size2I& windowSize,
        const Size2I& bufferSize)
    {
        FTK_P();
        p.windowSize = windowSize;
        p.bufferSize = bufferSize;
        setSizeUpdate();
        setDrawUpdate();
    }

    bool IWindow::_hasSizeUpdate(const std::shared_ptr<IWidget>& widget) const
    {
        bool out = widget->hasSizeUpdate();
        if (widget->isVisible(false))
        {
            if (out)
            {
                //std::cout << "Size update: " << widget->getObjectName() << std::endl;
            }
            else
            {
                for (const auto& child : widget->getChildren())
                {
                    out |= _hasSizeUpdate(child);
                }
            }
        }
        return out;
    }

    void IWindow::_sizeHintEventRecursive(
        const std::shared_ptr<IWidget>& widget,
        const SizeHintEvent& event)
    {
        for (const auto& child : widget->getChildren())
        {
            _sizeHintEventRecursive(child, event);
        }
        widget->sizeHintEvent(event);
        widget->setSizeUpdate(false);
    }

    bool IWindow::_hasDrawUpdate(const std::shared_ptr<IWidget>& widget) const
    {
        bool out = false;
        if (!widget->isClipped())
        {
            out = widget->hasDrawUpdate();
            if (out)
            {
                //std::cout << "Draw update: " << widget->getObjectName() << std::endl;
            }
            else
            {
                for (const auto& child : widget->getChildren())
                {
                    out |= _hasDrawUpdate(child);
                }
            }
        }
        return out;
    }

    void IWindow::_drawEventRecursive(
        const std::shared_ptr<IWidget>& widget,
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        const Box2I& g = widget->getGeometry();
        if (!widget->isClipped() && g.w() > 0 && g.h() > 0)
        {
            event.render->setClipRect(drawRect);
            widget->drawEvent(drawRect, event);
            widget->setDrawUpdate(false);
            const Box2I childrenClipRect = intersect(
                widget->getChildrenClipRect(),
                drawRect);
            for (const auto& child : widget->getChildren())
            {
                const Box2I& childGeometry = child->getGeometry();
                if (intersects(childGeometry, childrenClipRect))
                {
                    _drawEventRecursive(
                        child,
                        intersect(childGeometry, childrenClipRect),
                        event);
                }
            }
            event.render->setClipRect(drawRect);
            widget->drawOverlayEvent(drawRect, event);
        }
    }

    bool IWindow::_key(
        Key key,
        bool press,
        int modifiers)
    {
        FTK_P();
        if (p.mousePress.lock())
            return false;
        _closeTooltip();
        p.keyEvent = KeyEvent(key, modifiers, p.cursorPos);
        if (press)
        {
            // Send event to the focused widget or parent.
            if (auto widget = p.keyFocus.lock())
            {
                while (widget)
                {
                    widget->keyPressEvent(p.keyEvent);
                    if (p.keyEvent.accept)
                    {
                        p.keyPress = widget;
                        break;
                    }
                    if (auto dialog = std::dynamic_pointer_cast<IDialog>(widget))
                    {
                        break;
                    }
                    widget = widget->getParent();
                }
            }

            // Send event to the hovered widget.
            if (!p.keyEvent.accept)
            {
                auto widgets = _getUnderCursor(UnderCursor::Hover, p.cursorPos);
                for (auto i = widgets.begin(); i != widgets.end(); ++i)
                {
                    (*i)->keyPressEvent(p.keyEvent);
                    if (p.keyEvent.accept)
                    {
                        p.keyPress = *i;
                        break;
                    }
                }
            }

            // Handle tab key navigation.
            if (!p.keyEvent.accept && Key::Tab == key)
            {
                auto keyFocus = p.keyFocus.lock();
                if (modifiers == static_cast<int>(KeyModifier::Shift))
                {
                    keyFocus = getPrevKeyFocus(keyFocus);
                }
                else
                {
                    keyFocus = getNextKeyFocus(keyFocus);
                }
                setKeyFocus(keyFocus);
            }
        }
        else if (auto widget = p.keyPress.lock())
        {
            widget->keyReleaseEvent(p.keyEvent);
        }
        return p.keyEvent.accept;
    }

    void IWindow::_text(const std::string& value)
    {
        FTK_P();
        if (p.mousePress.lock())
            return;
        _closeTooltip();
        TextEvent event(value);
        if (auto widget = p.keyFocus.lock())
        {
            while (widget)
            {
                widget->textEvent(event);
                if (event.accept)
                {
                    break;
                }
                widget = widget->getParent();
            }
        }
    }

    void IWindow::_cursorEnter(bool enter)
    {
        FTK_P();
        p.inside = enter;
        if (!p.inside)
        {
            if (auto hover = p.hover.lock())
            {
                hover->mouseLeaveEvent();
            }
            p.hover.reset();
        }
    }

    void IWindow::_cursorPos(const V2I& pos)
    {
        FTK_P();

        p.cursorPosPrev = p.cursorPos;
        p.cursorPos = pos;

        MouseMoveEvent event(p.cursorPos, p.cursorPosPrev);
        auto widget = p.mousePress.lock();
        if (widget)
        {
            if (p.dragDropData)
            {
                // Find the drag and drop hover widget.
                DragDropEvent event(
                    p.cursorPos,
                    p.cursorPosPrev,
                    p.dragDropData);
                auto hover = p.dragDropHover.lock();
                auto widgets = _getUnderCursor(UnderCursor::Hover, p.cursorPos);
                std::shared_ptr<IWidget> widget;
                while (!widgets.empty())
                {
                    if (hover == widgets.front())
                    {
                        break;
                    }
                    widgets.front()->dragEnterEvent(event);
                    if (event.accept)
                    {
                        widget = widgets.front();
                        break;
                    }
                    widgets.pop_front();
                }
                if (widget)
                {
                    if (hover)
                    {
                        hover->dragLeaveEvent(event);
                    }
                    p.dragDropHover = widget;
                }
                else if (widgets.empty() && hover)
                {
                    p.dragDropHover.reset();
                    hover->dragLeaveEvent(event);
                }
                hover = p.dragDropHover.lock();
                if (hover)
                {
                    DragDropEvent event(
                        p.cursorPos,
                        p.cursorPosPrev,
                        p.dragDropData);
                    hover->dragMoveEvent(event);
                }
            }
            else
            {
                widget->mouseMoveEvent(event);

                p.dragDropData = event.dragDropData;
                p.dragDropCursor = event.dragDropCursor;
                p.dragDropCursorHotspot = event.dragDropCursorHotspot;
                if (p.dragDropData)
                {
                    // Start a drag and drop.
                    widget->mouseReleaseEvent(p.mouseClickEvent);
                    widget->mouseLeaveEvent();
                }
            }
        }
        else
        {
            _hoverUpdate(event);
        }

        if (widget && p.dragDropCursor)
        {
            setDrawUpdate();
        }

        if (length(p.cursorPos - p.tooltipPos) > p.size.dl)
        {
            _closeTooltip();
        }
    }

    void IWindow::_mouseButton(MouseButton button, bool press, int modifiers)
    {
        FTK_P();
        _closeTooltip();
        p.mouseClickEvent = MouseClickEvent(button, modifiers, p.cursorPos);
        if (press)
        {
            auto widgets = _getUnderCursor(UnderCursor::Hover, p.cursorPos);
            auto i = widgets.begin();
            for (; i != widgets.end(); ++i)
            {
                (*i)->mousePressEvent(p.mouseClickEvent);
                if (p.mouseClickEvent.accept)
                {
                    p.mousePress = *i;
                    break;
                }
                if (auto popup = std::dynamic_pointer_cast<IPopup>(*i))
                {
                    popup->close();
                }
            }
            if (!p.mouseClickEvent.accept)
            {
                setKeyFocus(nullptr);
            }
        }
        else
        {
            if (auto widget = p.mousePress.lock())
            {
                p.mousePress.reset();
                if (auto hover = p.dragDropHover.lock())
                {
                    // Finish a drag and drop.
                    p.dragDropHover.reset();
                    DragDropEvent event(
                        p.cursorPos,
                        p.cursorPosPrev,
                        p.dragDropData);
                    hover->dropEvent(event);
                    hover->dragLeaveEvent(event);
                }
                else
                {
                    widget->mouseReleaseEvent(p.mouseClickEvent);
                }
                p.dragDropData.reset();
                p.dragDropCursor.reset();
                setDrawUpdate();
            }

            MouseMoveEvent event(
                p.cursorPos,
                p.cursorPosPrev);
            _hoverUpdate(event);
        }
    }

    void IWindow::_scroll(const V2F& value, int modifiers)
    {
        FTK_P();
        _closeTooltip();

        // Send event to the focused widget or parent.
        ScrollEvent event(value, modifiers, p.cursorPos);
        if (auto widget = p.keyFocus.lock())
        {
            while (widget)
            {
                widget->scrollEvent(event);
                if (event.accept)
                {
                    break;
                }
                if (auto dialog = std::dynamic_pointer_cast<IDialog>(widget))
                {
                    break;
                }
                widget = widget->getParent();
            }
        }

        // Send event to the hovered widget.
        if (!event.accept)
        {
            auto widgets = _getUnderCursor(UnderCursor::Hover, p.cursorPos);
            for (auto i = widgets.begin(); i != widgets.end(); ++i)
            {
                (*i)->scrollEvent(event);
                if (event.accept)
                {
                    break;
                }
            }
        }
    }

    void IWindow::_clipEventRecursive(
        const std::shared_ptr<IWidget>& widget,
        const Box2I& clipRect,
        bool clipped)
    {
        const Box2I& g = widget->getGeometry();
        clipped |= !intersects(g, clipRect);
        clipped |= !widget->isVisible(false);
        const Box2I intersectedClipRect = intersect(g, clipRect);
        widget->clipEvent(intersectedClipRect, clipped);
        const Box2I childrenClipRect = intersect(
            widget->getChildrenClipRect(), intersectedClipRect);
        for (const auto& child : widget->getChildren())
        {
            const Box2I& childGeometry = child->getGeometry();
            _clipEventRecursive(
                child,
                intersect(childGeometry, childrenClipRect),
                clipped);
        }
    }

    std::list<std::shared_ptr<IWidget> > IWindow::_getUnderCursor(
        UnderCursor type,
        const V2I& pos)
    {
        FTK_P();
        std::list<std::shared_ptr<IWidget> > out;
        _getUnderCursor(type, shared_from_this(), pos, out);
        if (UnderCursor::Tooltip == type)
        {
            auto i = out.begin();
            for (; i != out.end() && !std::dynamic_pointer_cast<IPopup>(*i); ++i)
                ;
            if (i != out.end())
            {
                out.erase(i, out.end());
            }
        }
        return out;
    }

    void IWindow::_getUnderCursor(
        UnderCursor type,
        const std::shared_ptr<IWidget>& widget,
        const V2I& pos,
        std::list<std::shared_ptr<IWidget> >& out)
    {
        if (!widget->isClipped() &&
            (UnderCursor::Tooltip == type ? true : widget->isEnabled()) &&
            contains(widget->getGeometry(), pos))
        {
            for (auto i = widget->getChildren().rbegin();
                i != widget->getChildren().rend();
                ++i)
            {
                _getUnderCursor(type, *i, pos, out);
            }
            out.push_back(widget);
        }
    }

    void IWindow::_hoverUpdate(MouseMoveEvent& event)
    {
        FTK_P();
        const auto widgets = _getUnderCursor(UnderCursor::Hover, p.cursorPos);
        std::shared_ptr<IWidget> hover;
        auto prev = p.hover.lock();
        for (const auto& widget : widgets)
        {
            if (widget == prev)
            {
                widget->mouseMoveEvent(event);
                hover = widget;
                break;
            }
            MouseEnterEvent enterEvent(p.cursorPos);
            widget->mouseEnterEvent(enterEvent);
            if (enterEvent.accept)
            {
                if (prev)
                {
                    prev->mouseLeaveEvent();
                }
                widget->mouseMoveEvent(event);
                hover = widget;
                break;
            }
        }
        if (!hover && prev)
        {
            prev->mouseLeaveEvent();
        }
        p.hover = hover;
    }

    void IWindow::_getKeyFocus(
        const std::shared_ptr<IWidget>& widget,
        std::list<std::shared_ptr<IWidget> >& out)
    {
        if (widget->acceptsKeyFocus())
        {
            out.push_back(widget);
        }
        for (const auto& child : widget->getChildren())
        {
            if (!child->isClipped() && child->isEnabled())
            {
                _getKeyFocus(child, out);
            }
        }
    }

    void IWindow::_closeTooltip()
    {
        FTK_P();
        if (p.tooltip)
        {
            p.tooltip->close();
            p.tooltip.reset();
        }
        p.tooltipTimer = std::chrono::steady_clock::now();
        p.tooltipPos = p.cursorPos;
    }
}
