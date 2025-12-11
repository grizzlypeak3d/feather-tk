// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Window.h>

namespace ftk
{
    class App;
    class MenuBar;

    //! Main window.
    class FTK_API_TYPE MainWindow : public Window
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const Size2I&);

        MainWindow();

    public:
        virtual ~MainWindow();

        //! Create a new widget.
        static std::shared_ptr<MainWindow> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const Size2I& = Size2I(1280, 960));

        //! Get the menu bar.
        const std::shared_ptr<MenuBar>& getMenuBar() const;

        //! Set the menu bar.
        void setMenuBar(const std::shared_ptr<MenuBar>&);

        //! Get the central widget.
        const std::shared_ptr<IWidget>& getWidget() const;

        //! Set the central widget.
        void setWidget(const std::shared_ptr<IWidget>&);

        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
