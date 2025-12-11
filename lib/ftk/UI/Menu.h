// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Action.h>
#include <ftk/UI/IMenuPopup.h>

namespace ftk
{
    class MenuButton;

    //! \name Menu Widgets
    ///@{

    //! Menu.
    class FTK_API_TYPE Menu : public IMenuPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        Menu();

    public:
        virtual ~Menu();

        //! Create a new widget.
        static std::shared_ptr<Menu> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the actions.
        const std::vector<std::shared_ptr<Action> >& getActions() const;

        //! Add an action to the menu.
        void addAction(const std::shared_ptr<Action>&);

        //! Set whether a menu item is checked.
        //! 
        //! \todo Remove this function and replace it with an observer.
        void setChecked(const std::shared_ptr<Action>&, bool);

        //! Set whether a menu item is enabled.
        //! 
        //! \todo Remove this function and replace it with an observer.
        void setEnabled(const std::shared_ptr<Action>&, bool);

        //! Get the sub menus.
        const std::vector<std::shared_ptr<Menu> >& getSubMenus() const;

        //! Add a sub menu.
        std::shared_ptr<Menu> addSubMenu(const std::string&);

        //! Set whether a sub menu is enabled.
        void setSubMenuEnabled(const std::shared_ptr<Menu>&, bool);

        //! Add a divider.
        void addDivider();

        //! Clear the menu.
        void clear();

        //! Get whether the menu is empty.
        bool isEmpty() const;

        //! Handle keyboard shortcuts.
        bool shortcut(Key, int);

        void close() override;

        void keyFocusEvent(bool) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

    private:
        void _setCurrent(const std::shared_ptr<MenuButton>&);
        void _currentUpdate();

        std::shared_ptr<Menu> _getOpenMenu() const;
        std::vector<std::shared_ptr<MenuButton> > _getEnabled() const;

        void _accept();

        FTK_PRIVATE();
    };

    ///@}
}
