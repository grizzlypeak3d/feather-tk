// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/GraphWidget.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/ScrollWidget.h>

#include <ftk/Core/Timer.h>

using namespace ftk;

namespace widgets
{
    class Graphs : public IWidget
    {
    protected:
        void _init(const std::shared_ptr<Context>&);

        Graphs() = default;

    public:
        virtual ~Graphs();

        static std::shared_ptr<Graphs> create(const std::shared_ptr<Context>&);

        Size2I getSizeHint() const override;
        void setGeometry(const Box2I&) override;

    private:
        std::shared_ptr<Label> _imagesLabel;
        std::shared_ptr<GraphWidget> _imagesGraph;
        std::shared_ptr<Label> _imagesSizeLabel;
        std::shared_ptr<GraphWidget> _imageSizeGraph;
        std::shared_ptr<Label> _widgetsLabel;
        std::shared_ptr<GraphWidget> _widgetsGraph;
        std::shared_ptr<ScrollWidget> _scrollWidget;
        std::shared_ptr<Timer> _timer;
    };
}
