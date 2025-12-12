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
        FTK_API virtual ~MainWindow();

        //! Create a new widget.
        FTK_API static std::shared_ptr<MainWindow> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&,
            const Size2I& = Size2I(1280, 960));

        //! Get the menu bar.
        FTK_API const std::shared_ptr<MenuBar>& getMenuBar() const;

        //! Set the menu bar.
        FTK_API void setMenuBar(const std::shared_ptr<MenuBar>&);

        //! Get the central widget.
        FTK_API const std::shared_ptr<IWidget>& getWidget() const;

        //! Set the central widget.
        FTK_API void setWidget(const std::shared_ptr<IWidget>&);

        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
