// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/App.h>
#include <ftk/UI/DiagModel.h>
#include <ftk/UI/MainWindow.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/TabBar.h>

using namespace ftk;

namespace widgets
{
    class MainWindow : public ftk::MainWindow
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&);

        MainWindow() = default;

    public:
        virtual ~MainWindow();

        static std::shared_ptr<MainWindow> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<App>&);

        std::shared_ptr<DiagModel> getDiagModel() const;

    private:
        void _tabUpdate(int);

        std::shared_ptr<DiagModel> _diagModel;
        std::map<std::string, std::function<std::shared_ptr<IWidget>(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>&)> > _widgets;
        std::shared_ptr<TabBar> _tabBar;
        std::shared_ptr<VerticalLayout> _layout;
    };
}
