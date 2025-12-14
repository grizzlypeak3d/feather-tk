// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "MDI.h"

#include <ftk/UI/ColorWidget.h>
#include <ftk/UI/MDICanvas.h>
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
        auto canvas = MDICanvas::create(context);
        canvas->setSize(Size2I(8192, 8192));
        _scrollWidget->setWidget(canvas);

        // Create MDI widgets.
        Random random;
        for (size_t i = 0; i < 10; ++i)
        {
            auto colorWidget = ColorWidget::create(context);
            colorWidget->setColor(Color4F(random.getF(), random.getF(), random.getF()));
            canvas->addWidget(
                Format("Color {0}").arg(i),
                V2I(random.getI(0, 1600), random.getI(0, 900)),
                colorWidget);
        }
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
