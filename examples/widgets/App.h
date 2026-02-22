// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/App.h>
#include <ftk/UI/DiagModel.h>
#include <ftk/UI/SysLogModel.h>

namespace widgets
{
    class MainWindow;

    class App : public ftk::App
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::vector<std::string>&);

        App() = default;

    public:
        virtual ~App();

        static std::shared_ptr<App> create(
            const std::shared_ptr<ftk::Context>&,
            const std::vector<std::string>&);

        const std::shared_ptr<ftk::SysLogModel>& getSysLogModel() const;
        const std::shared_ptr<ftk::DiagModel>& getDiagModel() const;

        void run() override;

    private:
        std::shared_ptr<ftk::SysLogModel> _sysLogModel;
        std::shared_ptr<ftk::DiagModel> _diagModel;
        std::shared_ptr<MainWindow> _mainWindow;
    };
}
