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
        FTK_API virtual ~Menu();

        //! Create a new widget.
        FTK_API static std::shared_ptr<Menu> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the actions.
        FTK_API const std::vector<std::shared_ptr<Action> >& getActions() const;

        //! Add an action to the menu.
        FTK_API void addAction(const std::shared_ptr<Action>&);

        //! Set whether a menu item is checked.
        //! 
        //! \todo Remove this function and replace it with an observer.
        FTK_API void setChecked(const std::shared_ptr<Action>&, bool);

        //! Set whether a menu item is enabled.
        //! 
        //! \todo Remove this function and replace it with an observer.
        FTK_API void setEnabled(const std::shared_ptr<Action>&, bool);

        //! Get the sub menus.
        FTK_API const std::vector<std::shared_ptr<Menu> >& getSubMenus() const;

        //! Add a sub menu.
        FTK_API std::shared_ptr<Menu> addSubMenu(const std::string&);

        //! Set whether a sub menu is enabled.
        FTK_API void setSubMenuEnabled(const std::shared_ptr<Menu>&, bool);

        //! Add a divider.
        FTK_API void addDivider();

        //! Clear the menu.
        FTK_API void clear();

        //! Get whether the menu is empty.
        FTK_API bool isEmpty() const;

        //! Handle keyboard shortcuts.
        FTK_API bool shortcut(Key, int);

        FTK_API void close() override;

        FTK_API void keyFocusEvent(bool) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

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
