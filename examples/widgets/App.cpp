// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "App.h"

#include "MainWindow.h"

using namespace ftk;

namespace widgets
{
    void App::_init(
        const std::shared_ptr<Context>& context,
        const std::vector<std::string>& args)
    {
        ftk::App::_init(
            context,
            args,
            "widgets",
            "Widgets example");
    }

    App::~App()
    {}

    std::shared_ptr<App> App::create(
        const std::shared_ptr<Context>& context,
        const std::vector<std::string>& args)
    {
        auto out = std::shared_ptr<App>(new App);
        out->_init(context, args);
        return out;
    }

    const std::shared_ptr<SysLogModel>& App::getSysLogModel() const
    {
        return _sysLogModel;
    }

    const std::shared_ptr<DiagModel>& App::getDiagModel() const
    {
        return _diagModel;
    }

    void App::run()
    {
        _sysLogModel = SysLogModel::create(_context);

        _diagModel = DiagModel::create(_context);

        _mainWindow = MainWindow::create(
            _context,
            std::dynamic_pointer_cast<App>(shared_from_this()));

        ftk::App::run();
    }
}
