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
        virtual ~MenuBar();

        //! Create a new widget.
        static std::shared_ptr<MenuBar> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Add a menu.
        void addMenu(
            const std::string& text,
            const std::shared_ptr<Menu>&);

        //! Get a menu.
        std::shared_ptr<Menu> getMenu(const std::string&) const;

        //! Clear the menus.
        void clear();

        //! Handle keyboard shortcuts.
        bool shortcut(Key, int);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void keyFocusEvent(bool) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

    private:
        void _setCurrent(int);
        void _currentUpdate();

        std::shared_ptr<Menu> _getOpenMenu() const;
        void _openMenu(int);

        FTK_PRIVATE();
    };

    ///@}
}
