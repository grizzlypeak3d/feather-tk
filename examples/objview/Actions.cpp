// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Actions.h"

#include "App.h"
#include "Document.h"
#include "MainWindow.h"
#include "ObjView.h"
#include "SettingsModel.h"

using namespace ftk;

namespace objview
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
        _createRenderActions(context, app, mainWindow);

        // Observe the current document to update the state of the actions.
        _currentObserver = Observer<std::shared_ptr<IDocument> >::create(
            app->getDocumentModel()->observeCurrent(),
            [this](const std::shared_ptr<IDocument>& doc)
            {
                const bool current = doc.get();
                _actions["File/Close"]->setEnabled(current);
                _actions["File/CloseAll"]->setEnabled(current);
                _actions["View/Frame"]->setEnabled(current);
                _actions["View/ZoomIn"]->setEnabled(current);
                _actions["View/ZoomOut"]->setEnabled(current);
                _actions["View/OrbitLeft"]->setEnabled(current);
                _actions["View/OrbitRight"]->setEnabled(current);
                _actions["View/OrbitUp"]->setEnabled(current);
                _actions["View/OrbitDown"]->setEnabled(current);
                for (const auto label : getRenderModeLabels())
                {
                    const std::string key = "Render/" + label;
                    _actions[key]->setEnabled(current);
                }
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
                fileBrowserSystem->getModel()->setExts({ ".obj" });
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
                    if (auto view = mainWindow->getCurrentView())
                    {
                        view->frame();
                    }
                }
            });
        _actions["View/Frame"]->setTooltip("Frame the view");

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

        _actions["View/OrbitLeft"] = Action::create(
            "Orbit Left",
            "ViewLeft",
            Key::Left,
            [mainWindowWeak]
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    if (auto view = mainWindow->getCurrentView())
                    {
                        view->orbitLeft();
                    }
                }
            });
        _actions["View/OrbitLeft"]->setTooltip("Orbit the view left");

        _actions["View/OrbitRight"] = Action::create(
            "Orbit Right",
            "ViewRight",
            Key::Right,
            [mainWindowWeak]
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    if (auto view = mainWindow->getCurrentView())
                    {
                        view->orbitRight();
                    }
                }
            });
        _actions["View/OrbitRight"]->setTooltip("Orbit the view right");

        _actions["View/OrbitUp"] = Action::create(
            "Orbit Up",
            "ViewUp",
            Key::Up,
            [mainWindowWeak]
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    if (auto view = mainWindow->getCurrentView())
                    {
                        view->orbitUp();
                    }
                }
            });
        _actions["View/OrbitUp"]->setTooltip("Orbit the view up");

        _actions["View/OrbitDown"] = Action::create(
            "Orbit Down",
            "ViewDown",
            Key::Down,
            [mainWindowWeak]
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    if (auto view = mainWindow->getCurrentView())
                    {
                        view->orbitDown();
                    }
                }
            });
        _actions["View/OrbitDown"]->setTooltip("Orbit the view down");
    }

    void Actions::_createRenderActions(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<MainWindow>& mainWindow)
    {
        std::map<RenderMode, Key> shortcuts =
        {
            { RenderMode::Shaded, Key::_1 },
            { RenderMode::Flat, Key::_2 },
            { RenderMode::Texture, Key::_3 },
            { RenderMode::Normals, Key::_4 }
        };
        std::weak_ptr<App> appWeak(app);
        for (auto e : getRenderModeEnums())
        {
            const std::string label = getLabel(e);
            const std::string key = "Render/" + label;
            _actions[key] = Action::create(
                label,
                shortcuts[e],
                [appWeak, e](bool value)
                {
                    if (auto app = appWeak.lock())
                    {
                        auto render = app->getSettingsModel()->getRender();
                        render.mode = e;
                        app->getSettingsModel()->setRender(render);
                    }
                });
            _actions[key]->setTooltip(label);
            _renderModeActions[e] = _actions[key];
        }

        _actions["Render/Grid"] = Action::create(
            "Grid",
            [appWeak](bool value)
            {
                if (auto app = appWeak.lock())
                {
                    auto render = app->getSettingsModel()->getRender();
                    render.grid = value;
                    app->getSettingsModel()->setRender(render);
                }
            });
        _actions["Render/Grid"]->setTooltip("Toggle the grid");

        _actions["Render/Cull"] = Action::create(
            "Cull",
            [appWeak](bool value)
            {
                if (auto app = appWeak.lock())
                {
                    auto render = app->getSettingsModel()->getRender();
                    render.cull = value;
                    app->getSettingsModel()->setRender(render);
                }
            });
        _actions["Render/Cull"]->setTooltip("Cull back-facing triangles");

        _renderSettingsObserver = Observer<RenderSettings>::create(
            app->getSettingsModel()->observeRender(),
            [this](const RenderSettings& value)
            {
                for (const auto e : getRenderModeEnums())
                {
                    const std::string key = "Render/" + getLabel(e);
                    _actions[key]->setChecked(e == value.mode);
                }
                _actions["Render/Grid"]->setChecked(value.grid);
                _actions["Render/Cull"]->setChecked(value.cull);
            });
    }
}
