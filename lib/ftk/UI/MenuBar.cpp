// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/MenuBarPrivate.h>

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/RowLayout.h>

namespace ftk
{
    struct MenuBar::Private
    {
        std::vector<std::shared_ptr<Menu> > menus;
        std::vector<std::shared_ptr<MenuBarButton> > buttons;
        std::shared_ptr<HorizontalLayout> layout;
        int current = -1;
    };

    void MenuBar::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::MenuBar", parent);
        FTK_P();

        setAcceptsKeyFocus(true);

        p.layout = HorizontalLayout::create(context);
        _setWidget(p.layout);
        p.layout->setSpacingRole(SizeRole::None);
    }

    MenuBar::MenuBar() :
        _p(new Private)
    {}

    MenuBar::~MenuBar()
    {}

    std::shared_ptr<MenuBar> MenuBar::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<MenuBar>(new MenuBar);
        out->_init(context, parent);
        return out;
    }

    void MenuBar::addMenu(
        const std::string& text,
        const std::shared_ptr<Menu>& menu)
    {
        FTK_P();
        p.menus.push_back(menu);
        if (auto context = getContext())
        {
            auto button = MenuBarButton::create(context, text, p.layout);

            p.buttons.push_back(button);

            // The index is looked up when the callback runs rather than
            // captured when it is made, so that removing a menu does not leave
            // the buttons after it pointing at their old positions.
            auto buttonWeak = std::weak_ptr<MenuBarButton>(button);
            button->setHoveredCallback(
                [this, buttonWeak](bool value)
                {
                    if (value)
                    {
                        if (_getOpenMenu() || hasKeyFocus())
                        {
                            _setCurrent(_getIndex(buttonWeak.lock()));
                        }
                    }
                });

            auto menuWeak = std::weak_ptr<Menu>(menu);
            button->setPressedCallback(
                [this, menuWeak, buttonWeak]
                {
                    FTK_P();
                    auto menu = _getOpenMenu();
                    if (menu && menu == menuWeak.lock())
                    {
                        menu->close();
                    }
                    else
                    {
                        const int index = _getIndex(buttonWeak.lock());
                        takeKeyFocus();
                        _setCurrent(index);
                        _openMenu(index);
                    }
                });

            if (-1 == p.current)
            {
                p.current = 0;
                _currentUpdate();
            }
        }
    }

    std::shared_ptr<Menu> MenuBar::addMenu(const std::string& text)
    {
        auto menu = Menu::create(getContext());
        addMenu(text, menu);
        return menu;
    }

    std::shared_ptr<Menu> MenuBar::getMenu(const std::string& text) const
    {
        FTK_P();
        std::shared_ptr<Menu> out;
        for (int index = 0; index < static_cast<int>(p.menus.size()) && index < static_cast<int>(p.buttons.size()); ++index)
        {
            if (text == p.buttons[index]->getText())
            {
                out = p.menus[index];
                break;
            }
        }
        return out;
    }

    void MenuBar::removeMenu(const std::string& text)
    {
        FTK_P();
        for (int index = 0;
            index < static_cast<int>(p.menus.size()) &&
            index < static_cast<int>(p.buttons.size());
            ++index)
        {
            if (text == p.buttons[index]->getText())
            {
                if (auto menu = _getOpenMenu())
                {
                    menu->close();
                }
                p.menus[index]->setParent(nullptr);
                p.buttons[index]->setParent(nullptr);
                p.menus.erase(p.menus.begin() + index);
                p.buttons.erase(p.buttons.begin() + index);
                if (p.current >= static_cast<int>(p.buttons.size()))
                {
                    p.current = static_cast<int>(p.buttons.size()) - 1;
                }
                _currentUpdate();
                break;
            }
        }
    }

    int MenuBar::_getIndex(const std::shared_ptr<MenuBarButton>& button) const
    {
        FTK_P();
        for (int index = 0; index < static_cast<int>(p.buttons.size()); ++index)
        {
            if (button == p.buttons[index])
                return index;
        }
        return -1;
    }

    void MenuBar::clear()
    {
        FTK_P();
        for (const auto& menu : p.menus)
        {
            menu->setParent(nullptr);
        }
        p.menus.clear();
        for (const auto& button : p.buttons)
        {
            button->setParent(nullptr);
        }
        p.buttons.clear();
        p.current = -1;
    }

    bool MenuBar::shortcut(Key shortcut, int modifiers)
    {
        FTK_P();
        bool out = false;
        for (const auto& menu : p.menus)
        {
            if (menu->isEnabled())
            {
                if (menu->shortcut(shortcut, modifiers))
                {
                    out = true;
                    break;
                }
            }
        }
        return out;
    }
    
    

    

    void MenuBar::keyFocusEvent(bool value)
    {
        IWidget::keyFocusEvent(value);
        _currentUpdate();
    }

    void MenuBar::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        if (0 == event.modifiers)
        {
            switch (event.key)
            {
            case Key::Left:
                if (!_getOpenMenu())
                {
                    event.accept = true;
                    takeKeyFocus();
                    _setCurrent(p.current - 1);
                }
                break;
            case Key::Right:
                if (!_getOpenMenu())
                {
                    event.accept = true;
                    takeKeyFocus();
                    _setCurrent(p.current + 1);
                }
                break;
            case Key::Home:
                if (!_getOpenMenu())
                {
                    event.accept = true;
                    takeKeyFocus();
                    _setCurrent(0);
                }
                break;
            case Key::End:
                if (!_getOpenMenu())
                {
                    event.accept = true;
                    takeKeyFocus();
                    _setCurrent(static_cast<int>(p.buttons.size()) - 1);
                }
                break;
            case Key::Return:
                if (p.current >= 0 && p.current < static_cast<int>(p.menus.size()))
                {
                    event.accept = true;
                    takeKeyFocus();
                    _openMenu(p.current);
                }
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
            IWidget::keyPressEvent(event);
        }
    }

    void MenuBar::keyReleaseEvent(KeyEvent& event)
    {
        IWidget::keyReleaseEvent(event);
        event.accept = true;
    }

    void MenuBar::_setCurrent(int value)
    {
        FTK_P();
        const int tmp = !p.buttons.empty() ?
            clamp(value, 0, static_cast<int>(p.buttons.size()) - 1) :
            -1;
        if (tmp == p.current)
            return;
        p.current = tmp;
        _currentUpdate();
        if (auto openMenu = _getOpenMenu())
        {
            openMenu->close();
            _openMenu(p.current);
        }
    }

    void MenuBar::_currentUpdate()
    {
        FTK_P();
        const bool focus = hasKeyFocus();
        for (int i = 0; i < static_cast<int>(p.buttons.size()); ++i)
        {
            p.buttons[i]->setCurrent(p.current == i && focus);
        }
    }

    std::shared_ptr<Menu> MenuBar::_getOpenMenu() const
    {
        FTK_P();
        std::shared_ptr<Menu> out;
        for (auto& menu : p.menus)
        {
            if (menu->isOpen())
            {
                out = menu;
                break;
            }
        }
        return out;
    }

    void MenuBar::_openMenu(int value)
    {
        FTK_P();
        if (value >= 0 && value < static_cast<int>(p.buttons.size()))
        {
            auto menu = p.menus[value];
            auto openMenu = _getOpenMenu();
            if (menu != openMenu)
            {
                if (openMenu)
                {
                    openMenu->close();
                }
                if (!menu->isEmpty())
                {
                    menu->open(getWindow(), p.buttons[value]->getGeometry());
                }
            }
        }
    }
}
