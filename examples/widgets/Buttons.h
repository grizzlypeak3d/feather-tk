// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ButtonGroup.h>
#include <ftk/UI/ScrollWidget.h>

using namespace ftk;

namespace widgets
{
    class Buttons : public ftk::IWidget
    {
    protected:
        void _init(const std::shared_ptr<Context>&);

        Buttons() = default;

    public:
        virtual ~Buttons();

        static std::shared_ptr<Buttons> create(const std::shared_ptr<Context>&);

        Size2I getSizeHint() const override;
        void setGeometry(const Box2I&) override;

    private:
        std::shared_ptr<ftk::ScrollWidget> _scrollWidget;
        std::shared_ptr<ftk::ButtonGroup> _toolButtonGroup;
        std::shared_ptr<ftk::ButtonGroup> _radioButtonGroup;
    };
}
