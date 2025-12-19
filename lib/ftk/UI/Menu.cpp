// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/MenuPrivate.h>

#include <ftk/UI/Divider.h>
#include <ftk/UI/RowLayout.h>

namespace ftk
{
    struct Menu::Private
    {
        std::vector<std::shared_ptr<Action> > actions;
        std::vector<std::shared_ptr<MenuButton> > buttons;
        std::map< std::shared_ptr<Action>, std::shared_ptr<MenuButton> > actionToButton;
        std::shared_ptr<MenuButton> current;
        std::weak_ptr<Menu> parentMenu;
        std::vector<std::shared_ptr<Menu> > subMenus;
        std::map<std::shared_ptr<Menu>, std::shared_ptr<MenuButton> > subMenuToButton;
        std::map<std::shared_ptr<MenuButton>, std::shared_ptr<Menu> > buttonToSubMenu;
        std::shared_ptr<VerticalLayout> layout;
    };

    void Menu::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IMenuPopup::_init(context, "ftk::Menu", parent);
        FTK_P();
        setAcceptsKeyFocus(true);
        p.layout = VerticalLayout::create(context);
        p.layout->setSpacingRole(SizeRole::None);
        setWidget(p.layout);
    }

    Menu::Menu() :
        _p(new Private)
    {}

    Menu::~Menu()
    {}

    std::shared_ptr<Menu> Menu::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Menu>(new Menu);
        out->_init(context, parent);
        return out;
    }

    const std::vector<std::shared_ptr<Action> >& Menu::getActions() const
    {
        return _p->actions;
    }

    void Menu::addAction(const std::shared_ptr<Action>& action)
    {
        FTK_P();
        if (auto context = getContext())
        {
            p.actions.push_back(action);

            auto button = MenuButton::create(context, action, p.layout);
            p.buttons.push_back(button);
            p.actionToButton[action] = button;

            auto buttonWeak = std::weak_ptr<MenuButton>(button);
            button->setHoveredCallback(
                [this, buttonWeak](bool value)
                {
                    if (value)
                    {
                        if (auto menu = _getOpenMenu())
                        {
                            menu->close();
                        }
                        _setCurrent(buttonWeak.lock());
                    }
                });
            button->setClickedCallback(
                [this, action, buttonWeak]
                {
                    _setCurrent(buttonWeak.lock());
                    _accept();
                    action->doCallback();
                });
            button->setCheckedCallback(
                [this, action, buttonWeak](bool value)
                {
                    action->setChecked(value);
                    _setCurrent(buttonWeak.lock());
                    _accept();
                    action->doCheckedCallback(value);
                });
            button->setEnabledCallback(
                [this, buttonWeak](bool value)
                {
                    if (!value && buttonWeak.lock() == _p->current)
                    {
                        _setCurrent(nullptr);
                    }
                });

            if (!p.current && action->isEnabled())
            {
                p.current = button;
                _currentUpdate();
            }
        }
    }

    void Menu::setChecked(const std::shared_ptr<Action>& action, bool value)
    {
        FTK_P();
        const auto i = p.actionToButton.find(action);
        if (i != p.actionToButton.end())
        {
            i->first->setChecked(value);
            i->second->setChecked(value);
        }
    }

    void Menu::setEnabled(const std::shared_ptr<Action>& action, bool value)
    {
        FTK_P();
        const auto i = p.actionToButton.find(action);
        if (i != p.actionToButton.end())
        {
            i->second->setEnabled(value);
        }
    }

    const std::vector<std::shared_ptr<Menu> >& Menu::getSubMenus() const
    {
        return _p->subMenus;
    }

    std::shared_ptr<Menu> Menu::addSubMenu(const std::string& text)
    {
        FTK_P();
        std::shared_ptr<Menu> out;
        if (auto context = getContext())
        {
            out = Menu::create(context);
            out->setPopup(MenuPopup::SubMenu);
            out->_p->parentMenu = std::dynamic_pointer_cast<Menu>(shared_from_this());
            p.subMenus.push_back(out);

            auto button = MenuButton::create(context, nullptr, p.layout);
            button->setText(text);
            button->setIcon("Empty");
            button->setSubMenuIcon("SubMenuArrow");
            p.buttons.push_back(button);
            p.subMenuToButton[out] = button;
            p.buttonToSubMenu[button] = out;

            auto buttonWeak = std::weak_ptr<MenuButton>(button);
            button->setHoveredCallback(
                [this, out, buttonWeak](bool value)
                {
                    if (value)
                    {
                        if (!out->isOpen())
                        {
                            if (auto openMenu = _getOpenMenu())
                            {
                                openMenu->close();
                            }
                            if (auto button = buttonWeak.lock())
                            {
                                _setCurrent(button);
                                if (!out->isEmpty())
                                {
                                    out->open(getWindow(), button->getGeometry());
                                }
                            }
                        }
                    }
                });
            button->setPressedCallback(
                [this, out, buttonWeak]
                {
                    _setCurrent(buttonWeak.lock());
                    if (!out->isOpen())
                    {
                        if (auto openMenu = _getOpenMenu())
                        {
                            openMenu->close();
                        }
                        if (auto button = buttonWeak.lock())
                        {
                            _setCurrent(button);
                            if (!out->isEmpty())
                            {
                                out->open(getWindow(), button->getGeometry());
                            }
                        }
                    }
                });

            if (!p.current)
            {
                p.current = button;
                _currentUpdate();
            }
        }
        return out;
    }

    void Menu::setSubMenuEnabled(const std::shared_ptr<Menu>& menu, bool value)
    {
        FTK_P();
        const auto i = p.subMenuToButton.find(menu);
        if (i != p.subMenuToButton.end())
        {
            i->second->setEnabled(value);
        }
    }

    void Menu::addDivider()
    {
        FTK_P();
        if (auto context = getContext())
        {
            Divider::create(context, Orientation::Horizontal, p.layout);
        }
    }

    void Menu::clear()
    {
        FTK_P();
        p.actions.clear();
        for (const auto& button : p.buttons)
        {
            button->setParent(nullptr);
        }
        p.buttons.clear();
        p.actionToButton.clear();
        p.current.reset();
        p.subMenus.clear();
        p.subMenuToButton.clear();
        p.buttonToSubMenu.clear();
    }

    bool Menu::isEmpty() const
    {
        return _p->actions.empty() && _p->subMenus.empty();
    }

    bool Menu::shortcut(Key shortcut, int modifiers)
    {
        FTK_P();
        bool out = false;
        if (shortcut != Key::Unknown)
        {
            for (const auto& i : p.actionToButton)
            {
                const auto& shortcuts = i.first->getShortcuts();
                const auto j = std::find(
                    shortcuts.begin(),
                    shortcuts.end(),
                    KeyShortcut(shortcut, modifiers));
                if (i.first->isEnabled() && j != shortcuts.end())
                {
                    if (i.first->isCheckable())
                    {
                        setChecked(i.first, !i.first->isChecked());
                        i.first->doCheckedCallback(i.first->isChecked());
                    }
                    else
                    {
                        i.first->doCallback();
                    }
                    out = true;
                }
            }
        }
        if (!out)
        {
            for (const auto& subMenu : p.subMenus)
            {
                if (subMenu->isEnabled())
                {
                    if (subMenu->shortcut(shortcut, modifiers))
                    {
                        out = true;
                        break;
                    }
                }
            }
        }
        return out;
    }

    void Menu::close()
    {
        FTK_P();
        for (const auto& subMenu : p.subMenus)
        {
            subMenu->close();
        }
        IMenuPopup::close();
    }

    void Menu::keyFocusEvent(bool value)
    {
        IMenuPopup::keyFocusEvent(value);
        _currentUpdate();
    }

    void Menu::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        if (0 == event.modifiers)
        {
            switch (event.key)
            {
            case Key::Return:
                event.accept = true;
                takeKeyFocus();
                if (p.current)
                {
                    auto button = p.current;
                    const auto i = p.buttonToSubMenu.find(button);
                    if (i != p.buttonToSubMenu.end())
                    {
                        _setCurrent(button);
                        if (!i->second->isEmpty())
                        {
                            i->second->open(getWindow(), button->getGeometry());
                        }
                    }
                    else
                    {
                        button->click();
                    }
                }
                break;
            case Key::Up:
            {
                event.accept = true;
                takeKeyFocus();
                const auto enabled = _getEnabled();
                auto i = std::find(enabled.begin(), enabled.end(), p.current);
                if (i == enabled.end() && !enabled.empty())
                {
                    i = enabled.begin();
                }
                if (i != enabled.end() && i > enabled.begin())
                {
                    --i;
                }
                _setCurrent(i != enabled.end() && (*i)->isEnabled() ? *i : nullptr);
                break;
            }
            case Key::Down:
            {
                event.accept = true;
                takeKeyFocus();
                const auto enabled = _getEnabled();
                auto i = std::find(enabled.begin(), enabled.end(), p.current);
                if (i == enabled.end() && !enabled.empty())
                {
                    i = enabled.begin();
                }
                if (i != enabled.end() && i < enabled.end() - 1)
                {
                    ++i;
                }
                _setCurrent(i != enabled.end() && (*i)->isEnabled() ? *i : nullptr);
                break;
            }
            case Key::Home:
            {
                event.accept = true;
                takeKeyFocus();
                const auto enabled = _getEnabled();
                _setCurrent(!enabled.empty() ? enabled.front() : nullptr);
                break;
            }
            case Key::End:
            {
                event.accept = true;
                takeKeyFocus();
                const auto enabled = _getEnabled();
                _setCurrent(!enabled.empty() ? enabled.back() : nullptr);
                break;
            }
            default: break;
            }
        }
        if (!event.accept)
        {
            IMenuPopup::keyPressEvent(event);
        }
    }

    void Menu::keyReleaseEvent(KeyEvent& event)
    {
        IMenuPopup::keyReleaseEvent(event);
        event.accept = true;
    }

    void Menu::_setCurrent(const std::shared_ptr<MenuButton>& button)
    {
        FTK_P();
        if (button == p.current)
            return;
        p.current = button;
        _currentUpdate();
    }

    void Menu::_currentUpdate()
    {
        FTK_P();
        const bool focus = hasKeyFocus();
        for (const auto& button : p.buttons)
        {
            button->setCurrent(p.current == button && focus);
        }
    }

    std::shared_ptr<Menu> Menu::_getOpenMenu() const
    {
        FTK_P();
        std::shared_ptr<Menu> out;
        for (const auto& subMenu : p.subMenus)
        {
            if (subMenu->isOpen())
            {
                out = subMenu;
                break;
            }
        }
        return out;
    }

    std::vector<std::shared_ptr<MenuButton> > Menu::_getEnabled() const
    {
        FTK_P();
        std::vector<std::shared_ptr<MenuButton> > out;
        for (const auto& button : p.buttons)
        {
            if (button->isEnabled(false))
            {
                out.push_back(button);
            }
        }
        return out;
    }

    void Menu::_accept()
    {
        FTK_P();
        if (auto parentMenu = p.parentMenu.lock())
        {
            parentMenu->_accept();
        }
        close();
    }
}
