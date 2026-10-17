// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ChoiceToolButton.h>

#include <ftk/UI/Action.h>
#include <ftk/UI/Menu.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ToolButton.h>

namespace ftk
{
    struct ChoiceToolButton::Private
    {
        std::vector<std::shared_ptr<Action> > actions;
        size_t last = 0;
        std::shared_ptr<ToolButton> button;
        std::shared_ptr<ToolButton> menuButton;
        std::shared_ptr<Menu> menu;
        std::vector<std::shared_ptr<Observer<bool> > > checkedObservers;
        std::vector<std::shared_ptr<Observer<bool> > > enabledObservers;
    };

    void ChoiceToolButton::_init(
        const std::shared_ptr<Context>& context,
        const std::vector<std::shared_ptr<Action> >& actions,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::ChoiceToolButton", parent);
        FTK_P();
        p.actions = actions;

        auto layout = HorizontalLayout::create(context);
        layout->setSpacingRole(SizeRole::None);
        _setWidget(layout);
        p.button = ToolButton::create(context, layout);
        p.button->setCheckable(true);
        p.menuButton = ToolButton::create(context, layout);
        p.menuButton->setPopupIcon(true);
        p.menuButton->setTooltip("Choose");

        p.button->setCheckedCallback(
            [this](bool value)
            {
                FTK_P();
                if (p.actions.empty())
                {
                    return;
                }
                if (value)
                {
                    p.actions[p.last]->doCheckedCallback(true);
                }
                else
                {
                    for (const auto& action : p.actions)
                    {
                        if (action->isChecked())
                        {
                            action->doCheckedCallback(false);
                        }
                    }
                }
                // The button shows what the actions say, which may not be
                // what was asked: a disabled action stays as it was.
                _update();
            });
        p.menuButton->setPressedCallback(
            [this]
            {
                _openMenu();
            });

        for (const auto& action : p.actions)
        {
            p.checkedObservers.push_back(Observer<bool>::create(
                action->observeChecked(),
                [this](bool)
                {
                    _update();
                }));
            p.enabledObservers.push_back(Observer<bool>::create(
                action->observeEnabled(),
                [this](bool)
                {
                    _update();
                }));
        }
        _update();
    }

    ChoiceToolButton::ChoiceToolButton() :
        _p(new Private)
    {}

    ChoiceToolButton::~ChoiceToolButton()
    {}

    std::shared_ptr<ChoiceToolButton> ChoiceToolButton::create(
        const std::shared_ptr<Context>& context,
        const std::vector<std::shared_ptr<Action> >& actions,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ChoiceToolButton>(new ChoiceToolButton);
        out->_init(context, actions, parent);
        return out;
    }

    const std::vector<std::shared_ptr<Action> >& ChoiceToolButton::getActions() const
    {
        return _p->actions;
    }

    std::shared_ptr<Action> ChoiceToolButton::getCurrent() const
    {
        FTK_P();
        return p.last < p.actions.size() ? p.actions[p.last] : nullptr;
    }

    const std::shared_ptr<ToolButton>& ChoiceToolButton::getButton() const
    {
        return _p->button;
    }

    const std::shared_ptr<ToolButton>& ChoiceToolButton::getMenuButton() const
    {
        return _p->menuButton;
    }

    void ChoiceToolButton::_update()
    {
        FTK_P();
        if (!p.button)
        {
            return;
        }
        bool checked = false;
        bool enabled = false;
        for (size_t i = 0; i < p.actions.size(); ++i)
        {
            if (p.actions[i]->isChecked())
            {
                p.last = i;
                checked = true;
            }
            enabled |= p.actions[i]->isEnabled();
        }
        if (p.last < p.actions.size())
        {
            const auto& action = p.actions[p.last];
            p.button->setIcon(action->getIcon());
            p.button->setTooltip(action->getTooltip());
            p.button->setEnabled(action->isEnabled());
        }
        p.button->setChecked(checked);
        p.menuButton->setEnabled(enabled);
    }

    void ChoiceToolButton::_openMenu()
    {
        FTK_P();
        if (p.menu)
        {
            p.menu->close();
            return;
        }
        auto context = getContext();
        auto window = getWindow();
        if (!context || !window || p.actions.empty())
        {
            return;
        }
        p.menu = Menu::create(context);
        for (const auto& action : p.actions)
        {
            p.menu->addAction(action);
        }
        std::weak_ptr<ChoiceToolButton> weak(
            std::dynamic_pointer_cast<ChoiceToolButton>(shared_from_this()));
        p.menu->setCloseCallback(
            [weak]
            {
                if (auto widget = weak.lock())
                {
                    widget->_p->menu.reset();
                }
            });
        p.menu->open(window, p.menuButton->getGeometry());
    }
}
