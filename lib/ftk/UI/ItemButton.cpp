// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ItemButton.h>

#include <ftk/UI/DrawUtil.h>

namespace ftk
{
    struct ItemButton::Private
    {
        std::shared_ptr<IWidget> widget;
        Size2I sizeHint;
        int keyFocus = 0;
        std::function<void(bool)> focusCallback;
    };

    void ItemButton::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IButton::_init(context, "ftk::ItemButton", parent);
        // Like the list items: quiet until checked, hovered, or pressed.
        setButtonRole(ColorRole::None);
        setAcceptsKeyFocus(true);
    }

    ItemButton::ItemButton() :
        _p(new Private)
    {}

    ItemButton::~ItemButton()
    {}

    std::shared_ptr<ItemButton> ItemButton::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ItemButton>(new ItemButton);
        out->_init(context, parent);
        return out;
    }

    const std::shared_ptr<IWidget>& ItemButton::getWidget() const
    {
        return _p->widget;
    }

    void ItemButton::setWidget(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        if (value == p.widget)
            return;
        if (p.widget)
        {
            p.widget->setParent(nullptr);
        }
        p.widget = value;
        if (p.widget)
        {
            p.widget->setParent(shared_from_this());
        }
        setSizeUpdate();
        setDrawUpdate();
    }

    Size2I ItemButton::getSizeHint() const
    {
        return _p->sizeHint;
    }

    void ItemButton::setGeometry(const Box2I& value)
    {
        IButton::setGeometry(value);
        FTK_P();
        if (p.widget)
        {
            p.widget->setGeometry(value);
        }
    }

    void ItemButton::sizeHintEvent(const SizeHintEvent& event)
    {
        IButton::sizeHintEvent(event);
        FTK_P();
        p.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
        p.sizeHint = p.widget ? p.widget->getSizeHint() : Size2I();
    }

    void ItemButton::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        IButton::drawEvent(drawRect, event);
        const Box2I& g = getGeometry();

        // Draw the background.
        const ColorRole colorRole = _checked ? _checkedRole : _buttonRole;
        if (colorRole != ColorRole::None)
        {
            event.render->drawRect(
                g,
                event.style->getColorRole(colorRole));
        }

        // Draw the mouse state.
        if (_isMousePressed())
        {
            event.render->drawRect(
                g,
                event.style->getColorRole(ColorRole::Pressed));
        }
        else if (_isMouseInside())
        {
            event.render->drawRect(
                g,
                event.style->getColorRole(ColorRole::Hover));
        }

        // Draw the key focus. Inside the geometry rather than around it:
        // the content's own margin leaves the edges clear, and the item
        // does not grow when it is focused.
        if (hasKeyFocus())
        {
            event.render->drawMesh(
                border(g, _p->keyFocus),
                event.style->getColorRole(ColorRole::KeyFocus));
        }
    }

    void ItemButton::setFocusCallback(const std::function<void(bool)>& value)
    {
        _p->focusCallback = value;
    }

    void ItemButton::keyFocusEvent(bool value)
    {
        IButton::keyFocusEvent(value);
        setDrawUpdate();
        FTK_P();
        if (p.focusCallback)
        {
            p.focusCallback(value);
        }
    }

    void ItemButton::keyPressEvent(KeyEvent& event)
    {
        if (0 == event.modifiers)
        {
            switch (event.key)
            {
            case Key::Return:
                event.accept = true;
                click();
                break;
            case Key::Escape:
                if (hasKeyFocus())
                {
                    event.accept = true;
                    releaseKeyFocus();
                }
                break;
            default: break;
            }
        }
        if (!event.accept)
        {
            IButton::keyPressEvent(event);
        }
    }

    void ItemButton::keyReleaseEvent(KeyEvent& event)
    {
        IButton::keyReleaseEvent(event);
        event.accept = true;
    }
}
