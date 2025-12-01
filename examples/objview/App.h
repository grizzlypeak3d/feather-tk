// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include "Settings.h"

#include <ftk/UI/App.h>
#include <ftk/UI/DialogSystem.h>
#include <ftk/UI/DocumentModel.h>
#include <ftk/UI/RecentFilesModel.h>

#include <ftk/Core/CmdLine.h>

#include <filesystem>

namespace objview
{
    class MainWindow;
    class SettingsModel;

    //! Application.
    class App : public ftk::App
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::vector<std::string>& argv);

        App() = default;

    public:
        ~App();

        //! Create a new application.
        static std::shared_ptr<App> create(
            const std::shared_ptr<ftk::Context>&,
            const std::vector<std::string>&);

        //! \name Models
        ///@{

        const std::shared_ptr<SettingsModel>& getSettingsModel() const;
        const std::shared_ptr<ftk::DocumentModel>& getDocumentModel() const;
        const std::shared_ptr<ftk::RecentFilesModel>& getRecentFilesModel() const;

        ///@}

        //! Get the main window.
        const std::shared_ptr<MainWindow>& getMainWindow() const;

        //! \name File I/O
        //! 
        //! These are wrappers methods that do error handling.
        //! 
        ///@{

        void open(const std::filesystem::path&);
        void open(const std::vector<std::filesystem::path>&);
        void close(int);
        void closeAll();

        ///@}

    private:

        //! Command line arguments and options.
        struct CmdLine
        {
            std::shared_ptr<ftk::CmdLineListArg<std::string> > paths;
        };
        CmdLine _cmdLine;

        std::shared_ptr<SettingsModel> _settingsModel;
        std::shared_ptr<ftk::DocumentModel> _documentModel;
        std::shared_ptr<ftk::RecentFilesModel> _recentFilesModel;

        std::shared_ptr<MainWindow> _mainWindow;
        std::weak_ptr<ftk::ConfirmDialog> _confirmDialog;

        std::shared_ptr<ftk::Observer<StyleSettings> > _styleSettingsObserver;
    };
}
