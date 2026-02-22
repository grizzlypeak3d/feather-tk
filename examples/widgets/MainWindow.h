// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/MainWindow.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/TabBar.h>

namespace widgets
{
    class App;

    class MainWindow : public ftk::MainWindow
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&);

        MainWindow() = default;

    public:
        virtual ~MainWindow();

        static std::shared_ptr<MainWindow> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&);


    private:
        void _tabUpdate(int);

        std::weak_ptr<App> _app;
        std::map<std::string, std::function<std::shared_ptr<ftk::IWidget>(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<ftk::IWidget>&)> > _widgets;
        std::shared_ptr<ftk::TabBar> _tabBar;
        std::shared_ptr<ftk::VerticalLayout> _layout;
    };
}
