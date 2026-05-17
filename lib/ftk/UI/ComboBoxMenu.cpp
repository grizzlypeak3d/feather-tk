// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ComboBoxPrivate.h>

#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ScrollWidget.h>

namespace ftk
{
    struct ComboBoxMenu::Private
    {
        std::vector<std::shared_ptr<ComboBoxButton> > buttons;
        std::function<void(int)> callback;
        std::shared_ptr<VerticalLayout> layout;
        int radio = -1;
        int current = -1;
        bool scrollInit = true;
    };

    void ComboBoxMenu::_init(
        const std::shared_ptr<Context>& context,
        const std::vector<ComboBoxItem>& items,
        int currentIndex,
        const std::shared_ptr<IWidget>& parent)
    {
        IMenuPopup::_init(context, "ftk::ComboBoxMenu", parent);
        FTK_P();

        setAcceptsKeyFocus(true);

        p.layout = VerticalLayout::create(context, parent);
        p.layout->setSpacingRole(SizeRole::None);
        setWidget(p.layout);

        p.radio = currentIndex;
        p.current = currentIndex;
        for (size_t i = 0; i < items.size(); ++i)
        {
            auto button = ComboBoxButton::create(context, items[i], p.layout);
            button->setCheckable(true);
            const int index = static_cast<int>(i);
            button->setChecked(index == currentIndex);
            button->setCurrent(index == currentIndex);
            p.buttons.push_back(button);
            button->setCheckedCallback(
                [this, index](bool value)
                {
                    if (_p->radio >= 0 && _p->radio < _p->buttons.size())
                    {
                        _p->buttons[_p->radio]->setChecked(false);
                    }
                    _p->radio = index;
                    if (_p->radio >= 0 && _p->radio < _p->buttons.size())
                    {
                        _p->buttons[_p->radio]->setChecked(true);
                    }
                    if (_p->callback)
                    {
                        _p->callback(index);
                    }
                });
        }
    }

    ComboBoxMenu::ComboBoxMenu() :
        _p(new Private)
    {}

    ComboBoxMenu::~ComboBoxMenu()
    {}

    std::shared_ptr<ComboBoxMenu> ComboBoxMenu::create(
        const std::shared_ptr<Context>& context,
        const std::vector<ComboBoxItem>& items,
        int currentIndex,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ComboBoxMenu>(new ComboBoxMenu);
        out->_init(context, items, currentIndex, parent);
        return out;
    }

    void ComboBoxMenu::setCallback(const std::function<void(int)>& value)
    {
        _p->callback = value;
    }

    void ComboBoxMenu::setGeometry(const Box2I& value)
    {
        IMenuPopup::setGeometry(value);
        FTK_P();
        if (p.scrollInit)
        {
            p.scrollInit = false;
            _scrollToCurrent();
        }
    }

    void ComboBoxMenu::keyFocusEvent(bool value)
    {
        IMenuPopup::keyFocusEvent(value);
        _currentUpdate();
    }

    void ComboBoxMenu::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        if (0 == event.modifiers)
        {
            switch (event.key)
            {
            case Key::Return:
                event.accept = true;
                takeKeyFocus();
                if (p.current >= 0 && p.current < p.buttons.size())
                {
                    p.buttons[p.current]->click();
                }
                break;
            case Key::Up:
                event.accept = true;
                takeKeyFocus();
                _setCurrent(p.current - 1);
                break;
            case Key::Down:
                event.accept = true;
                takeKeyFocus();
                _setCurrent(p.current + 1);
                break;
            case Key::Home:
                event.accept = true;
                takeKeyFocus();
                _setCurrent(0);
                break;
            case Key::End:
                event.accept = true;
                takeKeyFocus();
                _setCurrent(static_cast<int>(p.buttons.size()) - 1);
                break;
            default: break;
            }
        }
        if (!event.accept)
        {
            IMenuPopup::keyPressEvent(event);
        }
    }

    void ComboBoxMenu::keyReleaseEvent(KeyEvent& event)
    {
        IMenuPopup::keyReleaseEvent(event);
        event.accept = true;
    }

    void ComboBoxMenu::_setCurrent(int value)
    {
        FTK_P();
        const int tmp = clamp(value, 0, static_cast<int>(p.buttons.size()) - 1);
        if (tmp == p.current)
            return;
        p.current = tmp;
        _currentUpdate();
        _scrollToCurrent();
    }

    void ComboBoxMenu::_currentUpdate()
    {
        FTK_P();
        const bool focus = hasKeyFocus();
        for (int i = 0; i < p.buttons.size(); ++i)
        {
            p.buttons[i]->setCurrent(p.current == i && focus);
        }
    }

    void ComboBoxMenu::_scrollToCurrent()
    {
        FTK_P();
        if (p.current >= 0 && p.current < p.buttons.size())
        {
            auto button = p.buttons[p.current];
            Box2I g = button->getGeometry();
            g = move(g, -p.layout->getGeometry().min);
            _getScrollWidget()->scrollTo(g);
        }
    }
}
