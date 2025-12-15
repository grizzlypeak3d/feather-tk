// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "MDI.h"

#include <ftk/UI/ColorWidget.h>
#include <ftk/UI/MDIWidget.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Format.h>
#include <ftk/Core/Random.h>

using namespace ftk;

namespace widgets
{
    void MDI::_init(const std::shared_ptr<Context>& context)
    {
        ftk::IWidget::_init(context, "MDI", nullptr);

        // Create a scroll widget.
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setAreaResizable(false);

        // Create a MDI canvas.
        _canvas = MDICanvas::create(context);
        const Size2I canvasSize(8192, 8192);
        _canvas->setSize(canvasSize);
        _scrollWidget->setWidget(_canvas);

        // Create MDI widgets.
        Random random;
        for (size_t i = 0; i < 50; ++i)
        {
            auto colorWidget = ColorWidget::create(context);
            colorWidget->setColor(Color4F(random.getF(), random.getF(), random.getF()));
            _canvas->addWidget(
                Format("Color {0}").arg(i),
                V2I(random.getI(0, canvasSize.w - 200), random.getI(0, canvasSize.h - 200)),
                colorWidget);
        }

        // Create a MDI mini-map.
        _miniMap = MDIMiniMap::create(context);
        _scrollWidget->setViewportWidget(_miniMap);

        // Setup callbacks.
        _scrollWidget->setScrollSizeCallback(
            [this](const Size2I& value)
            {
                _miniMap->setScrollSize(value);
            });
        _scrollWidget->setScrollPosCallback(
            [this](const V2I& value)
            {
                _miniMap->setScrollPos(value);
            });
        _scrollWidget->setViewportCallback(
            [this](const Box2I& value)
            {
                _miniMap->setViewportSize(value.size());
            });

        _canvas->setWidgetGeometryCallback(
            [this](const std::vector<Box2I>& value)
            {
                _miniMap->setWidgetGeometry(value);
            });

        _miniMap->setCallback(
            [this](const V2I& value)
            {
                _scrollWidget->setScrollPos(value);
            });
    }

    MDI::~MDI()
    {}

    std::shared_ptr<MDI> MDI::create(const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<MDI>(new MDI);
        out->_init(context);
        return out;
    }

    void MDI::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }

    void MDI::sizeHintEvent(const SizeHintEvent& event)
    {
        setSizeHint(_scrollWidget->getSizeHint());
    }
}
