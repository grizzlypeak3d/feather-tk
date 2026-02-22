// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ProgressDialog.h>
#include <ftk/UI/ScrollWidget.h>

#include <ftk/Core/Timer.h>

namespace widgets
{
    class App;

    class Dialogs : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        Dialogs() = default;

    public:
        virtual ~Dialogs();

        static std::shared_ptr<IWidget> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        ftk::Size2I getSizeHint() const override;
        void setGeometry(const ftk::Box2I&) override;

    private:
        std::shared_ptr<ftk::ScrollWidget> _scrollWidget;
        std::shared_ptr<ftk::Timer> _progressTimer;
        std::shared_ptr<ftk::ProgressDialog> _progressDialog;
    };
}
