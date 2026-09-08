// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/MenuPrivate.h>

#include <ftk/UI/Divider.h>
#include <ftk/UI/IWindow.h>
#include <ftk/UI/RowLayout.h>

namespace ftk
{
    struct Menu::Private
    {
        std::vector<std::shared_ptr<Action> > actions;
        std::vector<std::shared_ptr<MenuButton> > buttons;
        std::map<std::shared_ptr<Action>, std::shared_ptr<MenuButton> > actionToButton;
        std::shared_ptr<MenuButton> current;
        std::weak_ptr<Menu> parentMenu;
        std::vector<std::shared_ptr<Menu> > subMenus;
        std::map<std::shared_ptr<Menu>, std::shared_ptr<MenuButton> > subMenuToButton;
        std::map<std::shared_ptr<MenuButton>, std::shared_ptr<Menu> > buttonToSubMenu;
        std::shared_ptr<VerticalLayout> layout;
        std::function<void(const std::shared_ptr<Action>&)> currentCallback;
        std::shared_ptr<Action> announced;
        V2I tickCursorPos;
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
            // The menu holds itself weakly for the work that happens after an
            // action's callback. Closing a menu from an action that rebuilds
            // the menu bar it lives in is an ordinary thing to write, and
            // would otherwise leave the menu reading its own freed members.
            auto menuWeak = std::weak_ptr<Menu>(
                std::dynamic_pointer_cast<Menu>(shared_from_this()));
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
                [this, menuWeak, action, buttonWeak]
                {
                    _setCurrent(buttonWeak.lock());
                    action->doCallback();
                    if (auto menu = menuWeak.lock())
                    {
                        if (!action->isCheckable())
                        {
                            menu->_accept();
                        }
                    }
                });
            button->setCheckedCallback(
                [this, menuWeak, action, buttonWeak](bool value)
                {
                    _setCurrent(buttonWeak.lock());
                    action->setChecked(value);
                    action->doCheckedCallback(value);
                    if (auto menu = menuWeak.lock())
                    {
                        menu->_accept();
                    }
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
        if (const auto i = p.actionToButton.find(action);
            i != p.actionToButton.end())
        {
            i->first->setChecked(value);
            i->second->setChecked(value);
        }
    }

    void Menu::setEnabled(const std::shared_ptr<Action>& action, bool value)
    {
        FTK_P();
        if (const auto i = p.actionToButton.find(action);
            i != p.actionToButton.end())
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
            out->_p->currentCallback = p.currentCallback;
            p.subMenus.push_back(out);

            auto button = MenuButton::create(context, nullptr, p.layout);
            button->setText(text);
            button->setIcon("Empty");
            button->setSubMenuIcon("SubMenuArrow");
            p.buttons.push_back(button);
            p.subMenuToButton[out] = button;
            p.buttonToSubMenu[button] = out;

            auto buttonWeak = std::weak_ptr<MenuButton>(button);
            // The menu holds itself weakly for the work that happens after an
            // action's callback. Closing a menu from an action that rebuilds
            // the menu bar it lives in is an ordinary thing to write, and
            // would otherwise leave the menu reading its own freed members.
            auto menuWeak = std::weak_ptr<Menu>(
                std::dynamic_pointer_cast<Menu>(shared_from_this()));
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
        if (const auto i = p.subMenuToButton.find(menu);
            i != p.subMenuToButton.end())
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
                    // In the order IButton::_click() runs them, and both of
                    // them: it calls the clicked callback and then, if the
                    // button is checkable, the checked one. Running only one
                    // of the two would make an action do different things
                    // depending on whether it is typed or clicked, catching
                    // out any action given a plain callback and then put in
                    // a radio group.
                    i.first->doCallback();
                    if (i.first->isCheckable())
                    {
                        // Toggled, including for a radio action, because that
                        // is what the button does and ActionGroup is written
                        // to expect it -- it turns a radio action straight
                        // back on and says nothing about the moment in
                        // between. Selecting here instead would be a second
                        // answer to a question already answered there.
                        setChecked(i.first, !i.first->isChecked());
                        i.first->doCheckedCallback(i.first->isChecked());
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

    void Menu::open(
        const std::shared_ptr<IWindow>& window,
        const Box2I& buttonGeometry)
    {
        FTK_P();
        IMenuPopup::open(window, buttonGeometry);
        // Announced rather than waited for: the first item is already
        // current when the menu opens, so no highlight change is coming
        // to say it.
        _announce(_getAction(p.current));
    }

    void Menu::close()
    {
        FTK_P();
        for (const auto& subMenu : p.subMenus)
        {
            subMenu->close();
        }
        IMenuPopup::close();
        _setCurrent(nullptr);
        _announce(nullptr);
    }

    void Menu::tickEvent(
        bool parentsVisible,
        bool parentsEnabled,
        const TickEvent& event)
    {
        IMenuPopup::tickEvent(parentsVisible, parentsEnabled, event);
        FTK_P();
        // The item under the cursor is found from here rather than from
        // mouse events: a disabled item takes no events -- the scroll
        // widget behind it does -- but it shows a tooltip, so it should
        // announce like one. This is the tooltip's own approach, a walk
        // from the cursor, at the menu's scale.
        if (isOpen())
        {
            if (auto window = getWindow())
            {
                // Only when the cursor arrives, not while it sits: a
                // parked cursor would otherwise talk over the keyboard.
                const V2I& pos = window->getCursorPos();
                if (pos != p.tickCursorPos)
                {
                    p.tickCursorPos = pos;
                    for (const auto& button : p.buttons)
                    {
                        if (contains(button->getGeometry(), pos) &&
                            !button->isClipped())
                        {
                            if (!button->isEnabled())
                            {
                                _announce(_getAction(button));
                            }
                            break;
                        }
                    }
                }
            }
        }
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
                    if (const auto i = p.buttonToSubMenu.find(button);
                        i != p.buttonToSubMenu.end())
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

    void Menu::setCurrentCallback(
        const std::function<void(const std::shared_ptr<Action>&)>& value)
    {
        FTK_P();
        p.currentCallback = value;
        // The sub menus report through the same callback: to the caller
        // the menu and its sub menus are one menu.
        for (const auto& subMenu : p.subMenus)
        {
            subMenu->setCurrentCallback(value);
        }
    }

    std::shared_ptr<Action> Menu::_getAction(
        const std::shared_ptr<MenuButton>& button) const
    {
        FTK_P();
        std::shared_ptr<Action> out;
        for (const auto& i : p.actionToButton)
        {
            if (i.second == button)
            {
                out = i.first;
                break;
            }
        }
        return out;
    }

    void Menu::_announce(const std::shared_ptr<Action>& action)
    {
        FTK_P();
        if (action == p.announced)
            return;
        p.announced = action;
        if (p.currentCallback)
        {
            p.currentCallback(action);
        }
    }

    void Menu::_setCurrent(const std::shared_ptr<MenuButton>& button)
    {
        FTK_P();
        if (button == p.current)
            return;
        p.current = button;
        _currentUpdate();
        _announce(_getAction(button));
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
        auto parentMenu = p.parentMenu.lock();
        close();
        if (parentMenu)
        {
            parentMenu->_accept();
        }
    }
}
