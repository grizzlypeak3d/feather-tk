// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "App.h"

#include "Document.h"
#include "MainWindow.h"
#include "SettingsModel.h"

#include <ftk/UI/DialogSystem.h>

using namespace ftk;

namespace textedit
{
    void App::_init(
        const std::shared_ptr<Context>& context,
        const std::vector<std::string>& argv)
    {
        // Create the command line arguments.
        _cmdLine.paths = CmdLineListArg<std::string>::create(
            "inputs",
            "Input paths.",
            true);

        // Initialize the base class.
        ftk::App::_init(
            context,
            argv,
            "textedit",
            "Text edit example",
            { _cmdLine.paths });

        // Create models.
        _settingsModel = SettingsModel::create(context, getDefaultDisplayScale());
        _documentModel = DocumentModel::create(context);
        _recentFilesModel = RecentFilesModel::create(context);
        _recentFilesModel->setRecent(_settingsModel->getRecentFiles());

        // Initialize the file browser.
        auto fileBrowserSystem = context->getSystem<FileBrowserSystem>();
        fileBrowserSystem->setNativeFileDialog(false);
        fileBrowserSystem->setRecentFilesModel(_recentFilesModel);

        // Create the main window.
        _mainWindow = MainWindow::create(
            context,
            std::dynamic_pointer_cast<App>(shared_from_this()),
            Size2I(1280, 960));

        // Observe style settings.
        _styleSettingsObserver = Observer<StyleSettings>::create(
            _settingsModel->observeStyle(),
            [this](const StyleSettings& value)
            {
                setColorStyle(value.colorStyle);
                setDisplayScale(value.displayScale);
            });

        // Open command line arguments or create a new document.
        std::vector<std::filesystem::path> paths;
        for (const std::string& path : _cmdLine.paths->getList())
        {
            paths.push_back(std::filesystem::u8path(path));
        }
        if (!paths.empty())
        {
            open(paths);
        }
        else
        {
            _documentModel->add(Document::create(context));
        }
    }

    App::~App()
    {
        _settingsModel->setRecentFiles(_recentFilesModel->getRecent());
    }

    std::shared_ptr<App> App::create(
        const std::shared_ptr<Context>& context,
        const std::vector<std::string>& argv)
    {
        auto out = std::shared_ptr<App>(new App);
        out->_init(context, argv);
        return out;
    }

    const std::shared_ptr<SettingsModel>& App::getSettingsModel() const
    {
        return _settingsModel;
    }

    const std::shared_ptr<DocumentModel>& App::getDocumentModel() const
    {
        return _documentModel;
    }

    const std::shared_ptr<ftk::RecentFilesModel>& App::getRecentFilesModel() const
    {
        return _recentFilesModel;
    }

    const std::shared_ptr<MainWindow>& App::getMainWindow() const
    {
        return _mainWindow;
    }

    void App::open(const std::filesystem::path& path)
    {
        try
        {
            auto doc = Document::create(_context, path);
            _documentModel->add(doc);
            _recentFilesModel->addRecent(path);
        }
        catch (const std::exception& e)
        {
            _context->getSystem<DialogSystem>()->message(
                "ERROR",
                e.what(),
                _mainWindow);
        }
    }

    void App::open(const std::vector<std::filesystem::path>& paths)
    {
        std::vector<std::string> errors;
        for (const auto& path : paths)
        {
            try
            {
                auto doc = Document::create(_context, path);
                _documentModel->add(doc);
                _recentFilesModel->addRecent(path);
            }
            catch (const std::exception& e)
            {
                errors.push_back(e.what());
            }
        }
        if (!errors.empty())
        {
            _context->getSystem<DialogSystem>()->message(
                "ERROR",
                join(errors, '\n'),
                _mainWindow);
        }
    }

    void App::close(int index)
    {
        auto& docs = _documentModel->getList();
        if (index >= 0 && index < docs.size())
        {
            auto doc = std::dynamic_pointer_cast<Document>(docs[index]);
            if (doc && doc->isChanged())
            {
                _context->getSystem<DialogSystem>()->confirm(
                    "Unsaved Changes",
                    "File has unsaved changes, are you sure you want to close it?",
                    _mainWindow,
                    [this, doc, index](bool value)
                    {
                        if (value)
                        {
                            _documentModel->close(index);
                        }
                    },
                    "Yes",
                    "No");
            }
            else
            {
                _documentModel->close(index);
            }
        }
    }

    void App::closeAll()
    {
        bool changed = false;
        for (const auto& idoc : _documentModel->getList())
        {
            if (auto doc = std::dynamic_pointer_cast<Document>(idoc))
            {
                changed |= doc->isChanged();
            }
        }
        if (changed)
        {
            _context->getSystem<DialogSystem>()->confirm(
                "Unsaved Changes",
                "Files have unsaved changes, are you sure you want to close them?",
                _mainWindow,
                [this](bool value)
                {
                    if (value)
                    {
                        _documentModel->closeAll();
                    }
                },
                "Yes",
                "No");
        }
        else
        {
            _documentModel->closeAll();
        }
    }

    void App::save()
    {
        if (auto doc = std::dynamic_pointer_cast<Document>(_documentModel->getCurrent()))
        {
            if (doc->getPath().empty())
            {
                auto fileBrowserSystem = _context->getSystem<FileBrowserSystem>();
                fileBrowserSystem->open(
                    _mainWindow,
                    [this, doc](const Path& path)
                    {
                        try
                        {
                            doc->saveAs(std::filesystem::u8path(path.get()));
                        }
                        catch (const std::exception& e)
                        {
                            _context->getSystem<DialogSystem>()->message(
                                "ERROR",
                                e.what(),
                                _mainWindow);
                        }
                    },
                    "Save");
            }
            else
            {
                try
                {
                    doc->save();
                }
                catch (const std::exception& e)
                {
                    _context->getSystem<DialogSystem>()->message(
                        "ERROR",
                        e.what(),
                        _mainWindow);
                }
            }
        }
    }

    void App::saveAs()
    {
        if (auto doc = std::dynamic_pointer_cast<Document>(_documentModel->getCurrent()))
        {
            auto fileBrowserSystem = _context->getSystem<FileBrowserSystem>();
            fileBrowserSystem->open(
                _mainWindow,
                [this, doc](const Path& path)
                {
                    try
                    {
                        doc->saveAs(std::filesystem::u8path(path.get()));
                    }
                    catch (const std::exception& e)
                    {
                        _context->getSystem<DialogSystem>()->message(
                            "ERROR",
                            e.what(),
                            _mainWindow);
                    }
                },
                "Save As");
        }
    }

    void App::exit()
    {
        bool changed = false;
        for (const auto& idoc : _documentModel->getList())
        {
            if (auto doc = std::dynamic_pointer_cast<Document>(idoc))
            {
                changed |= doc->isChanged();
            }
        }
        if (changed)
        {
            if (!_confirmDialog.lock())
            {
                _confirmDialog = _context->getSystem<DialogSystem>()->confirm(
                    "Unsaved Changes",
                    "Files have unsaved changes, are you sure you want to exit?",
                    _mainWindow,
                    [this](bool value)
                    {
                        if (value)
                        {
                            ftk::App::exit();
                        }
                    },
                    "Yes",
                    "No");
            }
        }
        else
        {
            ftk::App::exit();
        }
    }
}
