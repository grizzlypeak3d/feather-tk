// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IContainer.h>
#include <ftk/UI/Menu.h>

namespace ftk
{
    class MenuBarButton;
    class Action;

    //! \name Menu Widgets
    ///@{

    //! Menu bar.
    class FTK_API_TYPE MenuBar : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        MenuBar();

    public:
        FTK_API virtual ~MenuBar();

        //! Create a new widget.
        FTK_API static std::shared_ptr<MenuBar> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Add a menu.
        FTK_API void addMenu(
            const std::string& text,
            const std::shared_ptr<Menu>&);

        //! Add a menu.
        FTK_API std::shared_ptr<Menu> addMenu(const std::string& text);

        //! Insert a menu at the given position, clamped to the end.
        //!
        //! For replacing one of the menus a framework put there: remove it and
        //! insert the application's own where it was, rather than appending it
        //! after everything else and leaving File on the right hand side.
        FTK_API void insertMenu(
            size_t index,
            const std::string& text,
            const std::shared_ptr<Menu>&);

        //! Get a menu.
        FTK_API std::shared_ptr<Menu> getMenu(const std::string&) const;

        //! Set a menu's title.
        //!
        //! Taken by menu rather than by title, because the use for this is a
        //! title that changes: a menu whose name is the thing it currently has
        //! selected reads at a glance, where one with a fixed name has to be
        //! opened to find out. Note that getMenu() and removeMenu() then follow
        //! the current title.
        FTK_API void setMenuText(
            const std::shared_ptr<Menu>&,
            const std::string&);

        //! Remove a menu, leaving the rest alone.
        //!
        //! Without this a bar whose menus come and go has to be cleared and
        //! rebuilt, which throws away the menus that were not changing and
        //! destroys the button that may be dispatching the click asking for it.
        FTK_API void removeMenu(const std::string&);

        //! Clear the menus.
        FTK_API void clear();

        //! Handle keyboard shortcuts.
        FTK_API bool shortcut(Key, int);

        FTK_API void keyFocusEvent(bool) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        int _getIndex(const std::shared_ptr<MenuBarButton>&) const;
        void _setCurrent(int);
        void _currentUpdate();

        std::shared_ptr<Menu> _getOpenMenu() const;
        void _openMenu(int);

        FTK_PRIVATE();
    };

    ///@}
}
