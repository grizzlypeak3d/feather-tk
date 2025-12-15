// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/MDICanvas.h>
#include <ftk/UI/MDINavigator.h>
#include <ftk/UI/ScrollWidget.h>

using namespace ftk;

namespace widgets
{
    class MDI : public IWidget
    {
    protected:
        void _init(const std::shared_ptr<Context>&);

        MDI() = default;

    public:
        virtual ~MDI();

        static std::shared_ptr<MDI> create(const std::shared_ptr<Context>&);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        std::shared_ptr<ScrollWidget> _scrollWidget;
        std::shared_ptr<MDICanvas> _canvas;
        std::shared_ptr<MDINavigator> _navigator;
    };
}
