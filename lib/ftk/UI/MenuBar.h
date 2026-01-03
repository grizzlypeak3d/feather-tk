// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Menu.h>

namespace ftk
{
    class Action;

    //! \name Menu Widgets
    ///@{

    //! Menu bar.
    class FTK_API_TYPE MenuBar : public IWidget
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

        //! Get a menu.
        FTK_API std::shared_ptr<Menu> getMenu(const std::string&) const;

        //! Clear the menus.
        FTK_API void clear();

        //! Handle keyboard shortcuts.
        FTK_API bool shortcut(Key, int);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void keyFocusEvent(bool) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        void _setCurrent(int);
        void _currentUpdate();

        std::shared_ptr<Menu> _getOpenMenu() const;
        void _openMenu(int);

        FTK_PRIVATE();
    };

    ///@}
}
