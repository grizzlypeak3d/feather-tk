// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ScrollWidget.h>

namespace widgets
{
    class App;

    class Stack : public ftk::IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        Stack() = default;

    public:
        virtual ~Stack();

        static std::shared_ptr<IWidget> create(
            const std::shared_ptr<ftk::Context>&,
            const std::shared_ptr<App>&,
            const std::shared_ptr<IWidget>& parent);

        ftk::Size2I getSizeHint() const override;
        void setGeometry(const ftk::Box2I&) override;

    private:
        std::shared_ptr<ftk::ScrollWidget> _scrollWidget;

        std::shared_ptr<ftk::Observer<bool> > _hasNextObserver;
        std::shared_ptr<ftk::Observer<bool> > _hasPrevObserver;
    };
}
