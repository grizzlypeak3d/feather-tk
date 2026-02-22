// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/TextEdit.h>

namespace widgets
{
    class App;

    class SysLog : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        SysLog() = default;

    public:
        virtual ~SysLog();

        static std::shared_ptr<IWidget> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        ftk::Size2I getSizeHint() const override;
        void setGeometry(const ftk::Box2I&) override;

    private:
        std::shared_ptr<ftk::TextEdit> _textEdit;

        std::shared_ptr<ftk::ListObserver<std::string> > _logObserver;
    };
}
