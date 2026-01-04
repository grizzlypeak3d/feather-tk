// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "App.h"

#include "Document.h"
#include "MainWindow.h"
#include "SettingsModel.h"

#include <ftk/UI/DialogSystem.h>

using namespace ftk;

namespace objview
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
            "objview",
            "Object view example",
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

        // Open command line arguments.
        std::vector<std::filesystem::path> paths;
        for (const std::string& path : _cmdLine.paths->getList())
        {
            paths.push_back(std::filesystem::u8path(path));
        }
        if (!paths.empty())
        {
            open(paths);
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
        _documentModel->close(index);
    }

    void App::closeAll()
    {
        _documentModel->closeAll();
    }
}
