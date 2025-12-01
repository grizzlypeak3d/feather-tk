// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include "Settings.h"

#include <ftk/UI/MainWindow.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/Splitter.h>

namespace textedit
{
    class Actions;
    class App;
    class Document;
    class DocumentTabs;
    class MenuBar;
    class SettingsModel;
    class SettingsWidget;
    class StatusBar;
    class ToolBar;

    //! Main window.
    class MainWindow : public ftk::MainWindow
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const ftk::Size2I&);

        MainWindow() = default;

    public:
        virtual ~MainWindow();

        //! Create a new window.
        static std::shared_ptr<MainWindow> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const ftk::Size2I&);

        void close() override;

    protected:
        void _drop(const std::vector<std::string>&) override;
 
    private:

        std::weak_ptr<App> _app;
        std::shared_ptr<SettingsModel> _settingsModel;

        std::shared_ptr<Actions> _actions;
        std::shared_ptr<MenuBar> _menuBar;
        std::shared_ptr<ToolBar> _toolBar;
        std::shared_ptr<DocumentTabs> _tabs;
        std::shared_ptr<SettingsWidget> _settingsWidget;
        std::shared_ptr<StatusBar> _statusBar;
        std::shared_ptr<ftk::Splitter> _splitter;
        std::shared_ptr<ftk::VerticalLayout> _layout;

        std::shared_ptr<ftk::Observer<WindowSettings> > _windowSettingsObserver;
    };
}
