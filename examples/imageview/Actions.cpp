// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Actions.h"

#include "App.h"
#include "Document.h"
#include "ImageView.h"
#include "MainWindow.h"
#include "SettingsModel.h"

using namespace ftk;

namespace imageview
{
    void Actions::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<MainWindow>& mainWindow)
    {
        // Create the actions.
        _createFileActions(context, app);
        _createEditActions(context, app);
        _createWindowActions(context, app, mainWindow);
        _createViewActions(context, app, mainWindow);
        _actionsUpdate();

        // Observe the current document to update the state of the actions.
        _currentObserver = Observer<std::shared_ptr<IDocument> >::create(
            app->getDocumentModel()->observeCurrent(),
            [this](const std::shared_ptr<IDocument>& doc)
            {
                _current = doc;
                _actionsUpdate();
            });
    }

    Actions::~Actions()
    {}

    std::shared_ptr<Actions> Actions::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<MainWindow>& mainWindow)
    {
        auto out = std::shared_ptr<Actions>(new Actions);
        out->_init(context, app, mainWindow);
        return out;
    }

    const std::map<std::string, std::shared_ptr<Action> > Actions::getActions() const
    {
        return _actions;
    }

    std::shared_ptr<Action> Actions::getAction(const std::string& name) const
    {
        const auto i = _actions.find(name);
        return i != _actions.end() ? i->second : nullptr;
    }

    void Actions::_createFileActions(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app)
    {
        auto appWeak = std::weak_ptr<App>(app);
        _actions["File/Open"] = Action::create(
            "Open",
            "FileOpen",
            KeyShortcut(Key::O, static_cast<int>(commandKeyModifier)),
            [this, appWeak]
            {
                auto app = appWeak.lock();
                auto fileBrowserSystem = app->getContext()->getSystem<FileBrowserSystem>();
                fileBrowserSystem->open(
                    app->getMainWindow(),
                    [appWeak](const Path& value)
                    {
                        if (auto app = appWeak.lock())
                        {
                            app->open(std::filesystem::u8path(value.get()));
                        }
                    });
            });
        _actions["File/Open"]->setTooltip("Open a file");

        _actions["File/Close"] = Action::create(
            "Close",
            "FileClose",
            KeyShortcut(Key::E, static_cast<int>(commandKeyModifier)),
            [appWeak]
            {
                auto app = appWeak.lock();
                app->close(app->getDocumentModel()->getCurrentIndex());
            });
        _actions["File/Close"]->setTooltip("Close the current file");

        _actions["File/CloseAll"] = Action::create(
            "Close All",
            "FileCloseAll",
            KeyShortcut(
                Key::E,
                static_cast<int>(KeyModifier::Shift) |
                static_cast<int>(commandKeyModifier)),
            [appWeak]
            {
                auto app = appWeak.lock();
                app->closeAll();
            });
        _actions["File/CloseAll"]->setTooltip("Close all files");

        _actions["File/Exit"] = Action::create(
            "Exit",
            KeyShortcut(Key::Q, static_cast<int>(commandKeyModifier)),
            [appWeak]
            {
                auto app = appWeak.lock();
                app->exit();
            });
    }

    void Actions::_createEditActions(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app)
    {
        auto appWeak = std::weak_ptr<App>(app);
        _actions["Edit/Settings"] = Action::create(
            "Settings",
            "Settings",
            [appWeak](bool value)
            {
                auto app = appWeak.lock();
                auto window = app->getSettingsModel()->getWindow();
                window.settings = value;
                app->getSettingsModel()->setWindow(window);
            });
        _actions["Edit/Settings"]->setTooltip("Toggle the settings");

        _windowSettingsObserver = Observer<WindowSettings>::create(
            app->getSettingsModel()->observeWindow(),
            [this](const WindowSettings& value)
            {
                _actions["Edit/Settings"]->setChecked(value.settings);
            });
    }

    void Actions::_createWindowActions(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<MainWindow>& mainWindow)
    {
        std::weak_ptr<MainWindow> mainWindowWeak(mainWindow);
        _actions["Window/FullScreen"] = Action::create(
            "Full Screen",
            "WindowFullScreen",
            KeyShortcut(Key::U, static_cast<int>(commandKeyModifier)),
            [mainWindowWeak](bool value)
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    mainWindow->setFullScreen(value);
                }
            });
        _actions["Window/FullScreen"]->setTooltip("Toggle full screen mode");

        _fullScreenObserver = Observer<bool>::create(
            mainWindow->observeFullScreen(),
            [this](bool value)
            {
                _actions["Window/FullScreen"]->setChecked(value);
            });
    }

    void Actions::_createViewActions(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<MainWindow>& mainWindow)
    {
        std::weak_ptr<MainWindow> mainWindowWeak(mainWindow);
        _actions["View/Frame"] = Action::create(
            "Frame",
            "ViewFrame",
            Key::Backspace,
            [mainWindowWeak]
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    mainWindow->getCurrentView()->frame();
                }
            });
        _actions["View/Frame"]->setTooltip("Frame the view");

        _actions["View/ZoomReset"] = Action::create(
            "Zoom Reset",
            "ViewZoomReset",
            Key::_0,
            [mainWindowWeak]
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    mainWindow->getCurrentView()->zoomReset();
                }
            });
        _actions["View/ZoomReset"]->setTooltip("Reset the view zoom");

        _actions["View/ZoomIn"] = Action::create(
            "Zoom In",
            "ViewZoomIn",
            Key::Equals,
            [mainWindowWeak]
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    if (auto view = mainWindow->getCurrentView())
                    {
                        view->zoomIn();
                    }
                }
            });
        _actions["View/ZoomIn"]->setTooltip("Zoom the view in");

        _actions["View/ZoomOut"] = Action::create(
            "Zoom Out",
            "ViewZoomOut",
            Key::Minus,
            [mainWindowWeak]
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    if (auto view = mainWindow->getCurrentView())
                    {
                        view->zoomOut();
                    }
                }
            });
        _actions["View/ZoomOut"]->setTooltip("Zoom the view out");

        _actions["View/Red"] = Action::create(
            "Red Channel",
            Key::R,
            [mainWindowWeak](bool value)
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    if (auto view = mainWindow->getCurrentView())
                    {
                        view->setChannelDisplay(value ? ChannelDisplay::Red : ChannelDisplay::Color);
                    }
                }
            });
        _actions["View/Red"]->setTooltip("View the red channel");

        _actions["View/Green"] = Action::create(
            "Green Channel",
            Key::G,
            [mainWindowWeak](bool value)
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    if (auto view = mainWindow->getCurrentView())
                    {
                        view->setChannelDisplay(value ? ChannelDisplay::Green : ChannelDisplay::Color);
                    }
                }
            });
        _actions["View/Green"]->setTooltip("View the green channel");

        _actions["View/Blue"] = Action::create(
            "Blue Channel",
            Key::B,
            [mainWindowWeak](bool value)
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    if (auto view = mainWindow->getCurrentView())
                    {
                        view->setChannelDisplay(value ? ChannelDisplay::Blue : ChannelDisplay::Color);
                    }
                }
            });
        _actions["View/Blue"]->setTooltip("View the blue channel");

        _actions["View/Alpha"] = Action::create(
            "Alpha Channel",
            Key::A,
            [mainWindowWeak](bool value)
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    if (auto view = mainWindow->getCurrentView())
                    {
                        view->setChannelDisplay(value ? ChannelDisplay::Alpha : ChannelDisplay::Color);
                    }
                }
            });
        _actions["View/Alpha"]->setTooltip("View the alpha channel");

        _viewObserver = Observer<std::shared_ptr<ImageView> >::create(
            mainWindow->observeCurrentView(),
            [this](const std::shared_ptr<ImageView>& view)
            {
                if (view)
                {
                    _channelDisplayObserver = Observer<ChannelDisplay>::create(
                        view->observeChannelDisplay(),
                        [this](ChannelDisplay value)
                        {
                            _actions["View/Red"]->setChecked(ChannelDisplay::Red == value);
                            _actions["View/Green"]->setChecked(ChannelDisplay::Green == value);
                            _actions["View/Blue"]->setChecked(ChannelDisplay::Blue == value);
                            _actions["View/Alpha"]->setChecked(ChannelDisplay::Alpha == value);
                        });
                }
                else
                {
                    _channelDisplayObserver.reset();
                    _actions["View/Red"]->setChecked(false);
                    _actions["View/Green"]->setChecked(false);
                    _actions["View/Blue"]->setChecked(false);
                    _actions["View/Alpha"]->setChecked(false);
                }
            });
    }

    void Actions::_actionsUpdate()
    {
        const auto doc = _current.lock();
        const bool current = doc.get();

        _actions["File/Close"]->setEnabled(current);
        _actions["File/CloseAll"]->setEnabled(current);

        _actions["View/ZoomReset"]->setEnabled(current);
        _actions["View/ZoomIn"]->setEnabled(current);
        _actions["View/ZoomOut"]->setEnabled(current);
        _actions["View/Red"]->setEnabled(current);
        _actions["View/Green"]->setEnabled(current);
        _actions["View/Blue"]->setEnabled(current);
        _actions["View/Alpha"]->setEnabled(current);
    }
}
