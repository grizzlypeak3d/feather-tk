// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Action.h>
#include <ftk/UI/Menu.h>
#include <ftk/UI/MenuBar.h>

#include <ftk/Core/Path.h>

namespace objview
{
    class Actions;
    class App;

    //! Menu bar.
    class MenuBar : public ftk::MenuBar
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<Actions>&,
            const std::shared_ptr<ftk::IWidget>& parent);

        MenuBar() = default;

    public:
        virtual ~MenuBar();

        //! Create a new menu bar.
        static std::shared_ptr<MenuBar> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<Actions>&,
            const std::shared_ptr<ftk::IWidget>& parent = nullptr);

    private:
        void _createFileMenu(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<Actions>&);
        void _createEditMenu(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<Actions>&);
        void _createWindowMenu(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<Actions>&);
        void _createViewMenu(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<Actions>&);
        void _createRenderMenu(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<Actions>&);

        std::map<std::string, std::shared_ptr<ftk::Menu> > _menus;

        std::shared_ptr<ftk::ListObserver<ftk::Path> > _recentFilesObserver;
    };
}
