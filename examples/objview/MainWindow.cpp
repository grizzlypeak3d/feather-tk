// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "MainWindow.h"

#include "Actions.h"
#include "App.h"
#include "Document.h"
#include "DocumentTabs.h"
#include "MenuBar.h"
#include "SettingsModel.h"
#include "SettingsWidget.h"
#include "ToolBar.h"

#include <ftk/UI/Divider.h>
#include <ftk/UI/RowLayout.h>

using namespace ftk;

namespace objview
{
    void MainWindow::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const Size2I& size)
    {
        ftk::MainWindow::_init(context, app, size);

        // Save pointers to the application and settings.
        _app = app;
        _settingsModel = app->getSettingsModel();

        // Create observables.
        _currentView = Observable<std::shared_ptr<ObjView> >::create();

        // Create the actions.
        _actions = Actions::create(
            context,
            app,
            std::dynamic_pointer_cast<MainWindow>(shared_from_this()));

        // Create the widgets.
        _menuBar = MenuBar::create(context, app, _actions);
        setMenuBar(_menuBar);
        _tabs = DocumentTabs::create(context, app);
        _settingsWidget = SettingsWidget::create(context, app);
        _toolBar = ToolBar::create(context, app, _actions);

        // Layout the widgets.
        _layout = VerticalLayout::create(context, shared_from_this());
        _layout->setSpacingRole(SizeRole::None);
        _layout->setStretch(Stretch::Expanding);
        _toolBar->setParent(_layout);
        Divider::create(context, Orientation::Vertical, _layout);
        _splitter = Splitter::create(context, Orientation::Horizontal, _layout);
        _splitter->setSplit(.7F);
        _tabs->setParent(_splitter);
        _settingsWidget->setParent(_splitter);
        setWidget(_layout);

        // Observe the current view.
        _currentViewObserver = Observer<std::shared_ptr<ObjView>>::create(
            _tabs->observeCurrentView(),
            [this](const std::shared_ptr<ObjView>& value)
            {
                _currentView->setIfChanged(value);
            });

        // Observe window settings.
        _windowSettingsObserver = Observer<WindowSettings>::create(
            _settingsModel->observeWindow(),
            [this](const WindowSettings& value)
            {
                _splitter->setSplit(value.split);
                _settingsWidget->setVisible(value.settings);
            });
    }

    MainWindow::~MainWindow()
    {
        auto window = _settingsModel->getWindow();
        window.split = _splitter->getSplit();
        _settingsModel->setWindow(window);
    }

    std::shared_ptr<MainWindow> MainWindow::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const Size2I& size)
    {
        auto out = std::shared_ptr<MainWindow>(new MainWindow);
        out->_init(context, app, size);
        return out;
    }
        
    const std::shared_ptr<ObjView>& MainWindow::getCurrentView() const
    {
        return _currentView->get();
    }

    std::shared_ptr<ftk::IObservable<std::shared_ptr<ObjView> > > MainWindow::observeCurrentView() const
    {
        return _currentView;
    }

    void MainWindow::_drop(const std::vector<std::string>& drops)
    {
        if (auto app = _app.lock())
        {
            // Open dropped files.
            std::vector<std::filesystem::path> paths;
            for (const auto& drop : drops)
            {
                paths.push_back(std::filesystem::u8path(drop));
            }
            app->open(paths);
        }
    }
}
