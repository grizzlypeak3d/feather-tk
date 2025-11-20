// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Actions.h"

#include "App.h"
#include "Document.h"
#include "MainWindow.h"
#include "SettingsModel.h"

using namespace ftk;

namespace textedit
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
        _actionsUpdate();

        // Observe the current document to update the state of the actions.
        _currentObserver = ValueObserver<std::shared_ptr<IDocument> >::create(
            app->getDocumentModel()->observeCurrent(),
            [this](const std::shared_ptr<IDocument>& idoc)
            {
                _current = idoc;
                if (auto doc = std::dynamic_pointer_cast<Document>(idoc))
                {
                    _changedObserver = ValueObserver<bool>::create(
                        doc->observeChanged(),
                        [this](bool)
                        {
                            _actionsUpdate();
                        });
                    _selectionObserver = ValueObserver<TextEditSelection>::create(
                        doc->getModel()->observeSelection(),
                        [this](const TextEditSelection&)
                        {
                            _actionsUpdate();
                        });
                }
                else
                {
                    _changedObserver.reset();
                    _selectionObserver.reset();
                    _actionsUpdate();
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
        _actions["File/New"] = Action::create(
            "New",
            "FileNew",
            Key::N,
            static_cast<int>(KeyModifier::Control),
            [appWeak]
            {
                auto app = appWeak.lock();
                auto document = Document::create(app->getContext());
                app->getDocumentModel()->add(document);
            });
        _actions["File/New"]->setTooltip("Create a new file");

        _actions["File/Open"] = Action::create(
            "Open",
            "FileOpen",
            Key::O,
            static_cast<int>(KeyModifier::Control),
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
            Key::E,
            static_cast<int>(KeyModifier::Control),
            [appWeak]
            {
                auto app = appWeak.lock();
                app->close(app->getDocumentModel()->getCurrentIndex());
            });
        _actions["File/Close"]->setTooltip("Close the current file");

        _actions["File/CloseAll"] = Action::create(
            "Close All",
            "FileCloseAll",
            Key::E,
            static_cast<int>(KeyModifier::Shift) |
            static_cast<int>(KeyModifier::Control),
            [appWeak]
            {
                auto app = appWeak.lock();
                app->closeAll();
            });
        _actions["File/CloseAll"]->setTooltip("Close all files");

        _actions["File/Save"] = Action::create(
            "Save",
            "FileSave",
            Key::S,
            static_cast<int>(KeyModifier::Control),
            [appWeak]
            {
                auto app = appWeak.lock();
                app->save();
            });
        _actions["File/Save"]->setTooltip("Save the current file");

        _actions["File/SaveAs"] = Action::create(
            "Save As",
            Key::S,
            static_cast<int>(KeyModifier::Shift) |
            static_cast<int>(KeyModifier::Control),
            [appWeak]
            {
                auto app = appWeak.lock();
                app->saveAs();
            });
        _actions["File/SaveAs"]->setTooltip("Save the current file with a new name");

        _actions["File/Exit"] = Action::create(
            "Exit",
            Key::Q,
            static_cast<int>(KeyModifier::Control),
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
        _actions["Edit/Undo"] = Action::create(
            "Undo",
            "Undo",
            Key::Z,
            static_cast<int>(KeyModifier::Control),
            [appWeak]
            {
                auto app = appWeak.lock();
                if (auto doc = std::dynamic_pointer_cast<Document>(app->getDocumentModel()->getCurrent()))
                {
                    doc->getModel()->undo();
                }
            });
        _actions["Edit/Undo"]->setTooltip("Undo");

        _actions["Edit/Redo"] = Action::create(
            "Redo",
            "Redo",
            Key::Y,
            static_cast<int>(KeyModifier::Control),
            [appWeak]
            {
                auto app = appWeak.lock();
                if (auto doc = std::dynamic_pointer_cast<Document>(app->getDocumentModel()->getCurrent()))
                {
                    doc->getModel()->redo();
                }
            });
        _actions["Edit/Redo"]->setTooltip("Redo");

        _actions["Edit/Cut"] = Action::create(
            "Cut",
            "Cut",
            Key::X,
            static_cast<int>(KeyModifier::Control),
            [appWeak]
            {
                auto app = appWeak.lock();
                if (auto doc = std::dynamic_pointer_cast<Document>(app->getDocumentModel()->getCurrent()))
                {
                    doc->getModel()->cut();
                }
            });
        _actions["Edit/Cut"]->setTooltip("Cut the selected text to the clipboard");

        _actions["Edit/Copy"] = Action::create(
            "Copy",
            "Copy",
            Key::C,
            static_cast<int>(KeyModifier::Control),
            [appWeak]
            {
                auto app = appWeak.lock();
                if (auto doc = std::dynamic_pointer_cast<Document>(app->getDocumentModel()->getCurrent()))
                {
                    doc->getModel()->copy();
                }
            });
        _actions["Edit/Copy"]->setTooltip("Copy the selected text to the clipboard");

        _actions["Edit/Paste"] = Action::create(
            "Paste",
            "Paste",
            Key::V,
            static_cast<int>(KeyModifier::Control),
            [appWeak]
            {
                auto app = appWeak.lock();
                if (auto doc = std::dynamic_pointer_cast<Document>(app->getDocumentModel()->getCurrent()))
                {
                    doc->getModel()->paste();
                }
            });
        _actions["Edit/Paste"]->setTooltip("Paste text from the clipboard");

        _actions["Edit/SelectAll"] = Action::create(
            "Select All",
            Key::A,
            static_cast<int>(KeyModifier::Control),
            [appWeak]
            {
                auto app = appWeak.lock();
                if (auto doc = std::dynamic_pointer_cast<Document>(app->getDocumentModel()->getCurrent()))
                {
                    doc->getModel()->selectAll();
                }
            });
        _actions["Edit/SelectAll"]->setTooltip("Select all of the text");

        _actions["Edit/ClearSelection"] = Action::create(
            "Clear Selection",
            Key::A,
            static_cast<int>(KeyModifier::Shift) |
            static_cast<int>(KeyModifier::Control),
            [appWeak]
            {
                auto app = appWeak.lock();
                if (auto doc = std::dynamic_pointer_cast<Document>(app->getDocumentModel()->getCurrent()))
                {
                    doc->getModel()->clearSelection();
                }
            });
        _actions["Edit/ClearSelection"]->setTooltip("Clear the selection");

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

        _windowSettingsObserver = ValueObserver<WindowSettings>::create(
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
            Key::U,
            static_cast<int>(KeyModifier::Control),
            [mainWindowWeak](bool value)
            {
                if (auto mainWindow = mainWindowWeak.lock())
                {
                    mainWindow->setFullScreen(value);
                }
            });
        _actions["Window/FullScreen"]->setTooltip("Toggle full screen mode");

        _fullScreenObserver = ValueObserver<bool>::create(
            mainWindow->observeFullScreen(),
            [this](bool value)
            {
                _actions["Window/FullScreen"]->setChecked(value);
            });
    }

    void Actions::_actionsUpdate()
    {
        const auto doc = std::dynamic_pointer_cast<Document>(_current.lock());
        const bool current = doc.get();
        TextEditSelection selection;
        if (doc)
        {
            selection = doc->getModel()->getSelection();
        }

        _actions["File/Close"]->setEnabled(current);
        _actions["File/CloseAll"]->setEnabled(current);
        _actions["File/Save"]->setEnabled(doc ? doc->isChanged() : false);
        _actions["File/SaveAs"]->setEnabled(current);

        _actions["Edit/Undo"]->setEnabled(false);
        _actions["Edit/Redo"]->setEnabled(false);
        _actions["Edit/Cut"]->setEnabled(current && selection.isValid());
        _actions["Edit/Copy"]->setEnabled(current && selection.isValid());
        _actions["Edit/Paste"]->setEnabled(current);
        _actions["Edit/SelectAll"]->setEnabled(current);
        _actions["Edit/ClearSelection"]->setEnabled(current && selection.isValid());
    }
}
