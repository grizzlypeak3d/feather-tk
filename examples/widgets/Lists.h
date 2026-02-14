// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ScrollWidget.h>

using namespace ftk;

namespace widgets
{
    class Lists : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        Lists() = default;

    public:
        virtual ~Lists();

        static std::shared_ptr<IWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        Size2I getSizeHint() const override;
        void setGeometry(const Box2I&) override;

    private:
        std::shared_ptr<ftk::ScrollWidget> _scrollWidget;
    };
}
