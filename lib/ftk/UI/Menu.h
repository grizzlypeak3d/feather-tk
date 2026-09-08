// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/Action.h>
#include <ftk/UI/IMenuPopup.h>

namespace ftk
{
    class MenuButton;

    //! \name Menu Widgets
    ///@{

    //! Menu.
    class FTK_UI_API_TYPE Menu : public IMenuPopup
    {
    protected:
        FTK_UI_API void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        FTK_UI_API Menu();

    public:
        FTK_UI_API virtual ~Menu();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<Menu> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the actions.
        FTK_UI_API const std::vector<std::shared_ptr<Action> >& getActions() const;

        //! Add an action to the menu.
        FTK_UI_API void addAction(const std::shared_ptr<Action>&);

        //! Set whether a menu item is checked.
        //! 
        //! \todo Remove this function and replace it with an observer.
        FTK_UI_API void setChecked(const std::shared_ptr<Action>&, bool);

        //! Bring the base widget's setEnabled(bool) into scope; the
        //! item-oriented overload below would otherwise hide it.
        using IWidget::setEnabled;

        //! Set whether a menu item is enabled.
        //! 
        //! \todo Remove this function and replace it with an observer.
        FTK_UI_API void setEnabled(const std::shared_ptr<Action>&, bool);

        //! Get the sub menus.
        FTK_UI_API const std::vector<std::shared_ptr<Menu> >& getSubMenus() const;

        //! Add a sub menu.
        FTK_UI_API std::shared_ptr<Menu> addSubMenu(const std::string&);

        //! Set whether a sub menu is enabled.
        FTK_UI_API void setSubMenuEnabled(const std::shared_ptr<Menu>&, bool);

        //! Add a divider.
        FTK_UI_API void addDivider();

        //! Clear the menu.
        FTK_UI_API void clear();

        //! Get whether the menu is empty.
        FTK_UI_API bool isEmpty() const;

        //! Handle keyboard shortcuts.
        FTK_UI_API bool shortcut(Key, int);

        //! Set a callback for the item the user is on: the item under
        //! the cursor -- a disabled item included, the way a tooltip
        //! would show for it -- or the current item when navigating by
        //! keyboard, announced when the menu opens too. It fires with
        //! nothing when the menu closes, and it reaches into the sub
        //! menus.
        FTK_UI_API void setCurrentCallback(
            const std::function<void(const std::shared_ptr<Action>&)>&);

        //! Bring the position overload into scope; the override below
        //! would otherwise hide it.
        using IMenuPopup::open;

        FTK_UI_API void open(
            const std::shared_ptr<IWindow>&,
            const Box2I& buttonGeometry) override;
        FTK_UI_API void close() override;

        FTK_UI_API void tickEvent(bool, bool, const TickEvent&) override;
        FTK_UI_API void keyFocusEvent(bool) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    private:
        std::shared_ptr<Action> _getAction(const std::shared_ptr<MenuButton>&) const;
        void _announce(const std::shared_ptr<Action>&);
        void _setCurrent(const std::shared_ptr<MenuButton>&);
        void _currentUpdate();

        std::shared_ptr<Menu> _getOpenMenu() const;
        std::vector<std::shared_ptr<MenuButton> > _getEnabled() const;

        void _accept();

        FTK_PRIVATE();
    };

    ///@}
}
