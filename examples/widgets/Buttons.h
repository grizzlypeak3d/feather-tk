// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ButtonGroup.h>
#include <ftk/UI/ScrollWidget.h>

namespace widgets
{
    class App;

    class Buttons : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        Buttons() = default;

    public:
        virtual ~Buttons();

        static std::shared_ptr<IWidget> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        ftk::Size2I getSizeHint() const override;
        void setGeometry(const ftk::Box2I&) override;

    private:
        std::shared_ptr<ftk::ScrollWidget> _scrollWidget;
        std::shared_ptr<ftk::ButtonGroup> _toolButtonGroup;
        std::shared_ptr<ftk::ButtonGroup> _radioButtonGroup;
    };
}
