// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ScrollWidget.h>

using namespace ftk;

namespace widgets
{
    class Charts : public ftk::IWidget
    {
    protected:
        void _init(const std::shared_ptr<Context>&);

        Charts() = default;

    public:
        virtual ~Charts();

        static std::shared_ptr<Charts> create(const std::shared_ptr<Context>&);

        Size2I getSizeHint() const override;
        void setGeometry(const Box2I&) override;

    private:
        std::shared_ptr<ftk::ScrollWidget> _scrollWidget;
    };
}
