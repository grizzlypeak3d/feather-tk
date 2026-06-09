// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "App.h"

#include "MainWindow.h"

#include <ftk/UI/DialogSystem.h>

using namespace ftk;

namespace gfx
{
    void App::_init(
        const std::shared_ptr<Context>& context,
        const std::vector<std::string>& argv)
    {
        ftk::App::_init(
            context,
            argv,
            "gfx",
            "Graphics examples");
    }

    App::~App()
    {}

    std::shared_ptr<App> App::create(
        const std::shared_ptr<Context>& context,
        const std::vector<std::string>& argv)
    {
        auto out = std::shared_ptr<App>(new App);
        out->_init(context, argv);
        return out;
    }

    const std::shared_ptr<MainWindow>& App::getMainWindow() const
    {
        return _mainWindow;
    }

    void App::run()
    {
        _mainWindow = MainWindow::create(
            _context,
            std::dynamic_pointer_cast<App>(shared_from_this()),
            Size2I(1280, 960));

        ftk::App::run();
    }
}
