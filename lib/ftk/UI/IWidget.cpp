// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IWidget.h>

#include <ftk/UI/IWindow.h>

#include <ftk/Core/String.h>

namespace ftk
{
    void IWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::string& objectName,
        const std::shared_ptr<IWidget>& parent)
    {
        _context = context;
        _objectName = objectName;
        _parent = parent;
        if (parent)
        {
            parent->_children.push_back(
                std::static_pointer_cast<IWidget>(shared_from_this()));

            ChildAddEvent event(shared_from_this());
            parent->childAddEvent(event);
        }
    }

    namespace
    {
        std::atomic<size_t> objectCount = 0;
    }

    IWidget::IWidget()
    {
        ++objectCount;
    }

    IWidget::~IWidget()
    {
        --objectCount;
    }

    void IWidget::setObjectName(const std::string& value)
    {
        _objectName = value;
    }

    std::string IWidget::getObjectPath() const
    {
        std::vector<std::string> names;
        auto widget = shared_from_this();
        while (widget)
        {
            names.push_back(widget->getObjectName());
            widget = widget->getParent();
        }
        std::reverse(names.begin(), names.end());
        return join(names, '/');
    }

    void IWidget::setParent(const std::shared_ptr<IWidget>& value)
    {
        if (!value)
        {
            releaseKeyFocus();
        }
        auto widget = shared_from_this();
        if (auto parent = _parent.lock())
        {
            auto i = parent->_children.begin();
            int j = 0;
            for (; i != parent->_children.end(); ++i, ++j)
            {
                if (*i == widget)
                {
                    break;
                }
            }
            if (i != parent->_children.end())
            {
                ChildRemoveEvent event(*i, j);
                parent->_children.erase(i);
                parent->childRemoveEvent(event);
                parent->setSizeUpdate();
                parent->setDrawUpdate();
            }
        }
        _parent = value;
        if (value)
        {
            value->_children.push_back(
                std::static_pointer_cast<IWidget>(shared_from_this()));
            ChildAddEvent event(shared_from_this());
            value->childAddEvent(event);
            value->setSizeUpdate();
            value->setDrawUpdate();
        }
    }

    int IWidget::getChildIndex(const std::shared_ptr<IWidget>& value) const
    {
        int out = -1;
        int i = 0;
        auto j = _children.begin();
        for (; *j != value && j != _children.end(); ++i, ++j)
            ;
        if (j != _children.end())
        {
            out = i;
        }
        return out;
    }

    void IWidget::moveToFront(const std::shared_ptr<IWidget>& value)
    {
        auto i = std::find(_children.begin(), _children.end(), value);
        if (i != _children.end())
        {
            auto child = *i;
            _children.erase(i);
            _children.push_back(child);
            setSizeUpdate();
            setDrawUpdate();
        }
    }

    void IWidget::moveToBack(const std::shared_ptr<IWidget>& value)
    {
        auto i = std::find(_children.begin(), _children.end(), value);
        if (i != _children.end())
        {
            auto child = *i;
            _children.erase(i);
            _children.push_front(child);
            setSizeUpdate();
            setDrawUpdate();
        }
    }

    void IWidget::moveToIndex(const std::shared_ptr<IWidget>& value, size_t index)
    {
        auto i = _children.begin();
        for (; i != _children.end() && *i != value; ++i)
            ;
        if (i != _children.end())
        {
            _children.erase(i);
            i = _children.begin();
            int j = 0;
            for (; i != _children.end() && j < index; ++i, ++j)
                ;
            if (i != _children.end())
            {
                _children.insert(i, value);
            }
            else
            {
                _children.push_back(value);
            }
            setSizeUpdate();
            setDrawUpdate();
        }
    }

    std::shared_ptr<IWindow> IWidget::getWindow()
    {
        std::shared_ptr<IWidget> out = shared_from_this();
        auto parent = out->_parent.lock();
        while (parent)
        {
            out = parent;
            parent = parent->_parent.lock();
        }
        return std::dynamic_pointer_cast<IWindow>(out);
    }

    Size2I IWidget::getSizeHint() const
    {
        return Size2I();
    }

    void IWidget::setHStretch(Stretch value)
    {
        if (value == _hStretch)
            return;
        _hStretch = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    void IWidget::setVStretch(Stretch value)
    {
        if (value == _vStretch)
            return;
        _vStretch = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    void IWidget::setStretch(Stretch horizontal, Stretch vertical)
    {
        if (horizontal == _hStretch && vertical == _vStretch)
            return;
        _hStretch = horizontal;
        _vStretch = vertical;
        setSizeUpdate();
        setDrawUpdate();
    }

    void IWidget::setStretch(Stretch value)
    {
        setStretch(value, value);
    }

    void IWidget::setHAlign(HAlign value)
    {
        if (value == _hAlign)
            return;
        _hAlign = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    void IWidget::setVAlign(VAlign value)
    {
        if (value == _vAlign)
            return;
        _vAlign = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    void IWidget::setAlign(HAlign hAlign, VAlign vAlign)
    {
        if (hAlign == _hAlign && vAlign == _vAlign)
            return;
        _hAlign = hAlign;
        _vAlign = vAlign;
        setSizeUpdate();
        setDrawUpdate();
    }

    void IWidget::setGeometry(const Box2I& value)
    {
        if (value == _geometry)
            return;
        _geometry = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    void IWidget::setPos(const V2I& value)
    {
        setGeometry(Box2I(value, _geometry.size()));
    }

    void IWidget::setSize(const Size2I& value)
    {
        setGeometry(Box2I(_geometry.min, value));
    }

    void IWidget::setVisible(bool value)
    {
        if (value == _visible)
            return;
        _visible = value;
        if (!_visible)
        {
            releaseKeyFocus();
        }
        setSizeUpdate();
        setDrawUpdate();
        if (auto parent = _parent.lock())
        {
            parent->setSizeUpdate();
            parent->setDrawUpdate();
        }
    }

    void IWidget::show()
    {
        setVisible(true);
    }

    void IWidget::hide()
    {
        setVisible(false);
    }

    Box2I IWidget::getChildrenClipRect() const
    {
        return _geometry;
    }

    void IWidget::setEnabled(bool value)
    {
        if (value == _enabled)
            return;
        _enabled = value;
        if (!_enabled)
        {
            releaseKeyFocus();
        }
        setSizeUpdate();
        setDrawUpdate();
    }

    void IWidget::setBackgroundRole(ColorRole value)
    {
        if (value == _backgroundRole)
            return;
        _backgroundRole = value;
        setDrawUpdate();
    }

    void IWidget::setAcceptsKeyFocus(bool value)
    {
        _acceptsKeyFocus = value;
    }

    void IWidget::takeKeyFocus()
    {
        if (isEnabled())
        {
            if (auto window = getWindow())
            {
                window->setKeyFocus(shared_from_this());
            }
        }
    }

    void IWidget::releaseKeyFocus()
    {
        if (_keyFocus)
        {
            if (auto window = getWindow())
            {
                window->setKeyFocus(nullptr);
            }
        }
        auto children = _children;
        for (const auto& child : children)
        {
            child->releaseKeyFocus();
        }
    }

    void IWidget::setTooltip(const std::string& value)
    {
        _tooltip = value;
    }

    void IWidget::childAddEvent(const ChildAddEvent&)
    {}

    void IWidget::childRemoveEvent(const ChildRemoveEvent&)
    {}

    void IWidget::tickEvent(
        bool parentsVisible,
        bool parentsEnabled,
        const TickEvent& event)
    {
        _parentsVisible = parentsVisible;
        _parentsEnabled = parentsEnabled;
    }

    void IWidget::sizeHintEvent(const SizeHintEvent& event)
    {}

    void IWidget::clipEvent(const Box2I&, bool clipped)
    {
        if (clipped && clipped != _clipped)
        {
            releaseKeyFocus();
        }
        _clipped = clipped;
    }

    void IWidget::drawEvent(
        const Box2I&,
        const DrawEvent& event)
    {
        if (_backgroundRole != ColorRole::None)
        {
            event.render->drawRect(
                _geometry,
                event.style->getColorRole(_backgroundRole));
        }
    }

    void IWidget::drawOverlayEvent(
        const Box2I&,
        const DrawEvent&)
    {}

    void IWidget::mouseEnterEvent(MouseEnterEvent& event)
    {}

    void IWidget::mouseLeaveEvent()
    {}

    void IWidget::mouseMoveEvent(MouseMoveEvent& event)
    {}

    void IWidget::mousePressEvent(MouseClickEvent& event)
    {}

    void IWidget::mouseReleaseEvent(MouseClickEvent& event)
    {}

    void IWidget::scrollEvent(ScrollEvent&)
    {}

    void IWidget::keyFocusEvent(bool value)
    {
        _keyFocus = value;
    }

    void IWidget::keyPressEvent(KeyEvent&)
    {}

    void IWidget::keyReleaseEvent(KeyEvent&)
    {}

    void IWidget::textEvent(TextEvent&)
    {}

    void IWidget::dragEnterEvent(DragDropEvent&)
    {}

    void IWidget::dragLeaveEvent(DragDropEvent&)
    {}

    void IWidget::dragMoveEvent(DragDropEvent&)
    {}

    void IWidget::dropEvent(DragDropEvent&)
    {}

    size_t IWidget::getObjectCount()
    {
        return objectCount;
    }
}
