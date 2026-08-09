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

        //! Get a menu.
        FTK_API std::shared_ptr<Menu> getMenu(const std::string&) const;

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
