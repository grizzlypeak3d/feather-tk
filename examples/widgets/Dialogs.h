// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ProgressDialog.h>
#include <ftk/UI/ScrollWidget.h>

#include <ftk/Core/Timer.h>

using namespace ftk;

namespace widgets
{
    class Dialogs : public ftk::IWidget
    {
    protected:
        void _init(const std::shared_ptr<Context>&);

        Dialogs() = default;

    public:
        virtual ~Dialogs();

        static std::shared_ptr<Dialogs> create(const std::shared_ptr<Context>&);

        Size2I getSizeHint() const override;
        void setGeometry(const Box2I&) override;

    private:
        std::shared_ptr<ftk::ScrollWidget> _scrollWidget;
        std::shared_ptr<ftk::Timer> _progressTimer;
        std::shared_ptr<ftk::ProgressDialog> _progressDialog;
    };
}
