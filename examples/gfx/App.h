// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/App.h>

namespace gfx
{
    class MainWindow;

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

        //! Get the main window.
        const std::shared_ptr<MainWindow>& getMainWindow() const;

        void run() override;

    private:
        std::shared_ptr<MainWindow> _mainWindow;
    };
}
