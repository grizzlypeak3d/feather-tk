// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Layouts.h"

#include <ftk/UI/ColorSwatch.h>
#include <ftk/UI/FormLayout.h>
#include <ftk/UI/GridLayout.h>
#include <ftk/UI/GroupBox.h>
#include <ftk/UI/IntEdit.h>
#include <ftk/UI/IntEditSlider.h>
#include <ftk/UI/RowLayout.h>

using namespace ftk;

namespace widgets
{
    namespace
    {
        // Colored blocks instead of labels, so the structure the layout
        // creates -- the cells, the spacing, the spacers -- can be seen.
        std::shared_ptr<ColorSwatch> createBlock(
            const std::shared_ptr<Context>& context,
            size_t index,
            const std::shared_ptr<IWidget>& parent)
        {
            const std::vector<Color4F> colors =
            {
                Color4F(.9F, .4F, .3F),
                Color4F(.9F, .7F, .2F),
                Color4F(.4F, .7F, .3F),
                Color4F(.3F, .6F, .8F)
            };
            auto out = ColorSwatch::create(context, parent);
            out->setColor(colors[index % colors.size()]);
            out->setSizeRole(SizeRole::SwatchLarge);
            out->setVAlign(VAlign::Center);
            return out;
        }
    }

    void Layouts::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        ftk::IWidget::_init(context, "Layouts", parent);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create row layouts.
        auto groupBox = GroupBox::create(context, "Row Layouts", layout);
        auto groupLayout = HorizontalLayout::create(context, groupBox);
        auto hLayout = HorizontalLayout::create(context, groupLayout);
        createBlock(context, 0, hLayout);
        createBlock(context, 1, hLayout);
        hLayout->addSpacer();
        createBlock(context, 2, hLayout);
        auto vLayout = VerticalLayout::create(context, groupLayout);
        createBlock(context, 0, vLayout);
        createBlock(context, 1, vLayout);
        vLayout->addSpacer();
        createBlock(context, 2, vLayout);

        // Create grid layouts.
        groupBox = GroupBox::create(context, "Grid Layouts", layout);
        groupLayout = HorizontalLayout::create(context, groupBox);
        auto gridLayout = GridLayout::create(context, groupLayout);
        auto block = createBlock(context, 0, gridLayout);
        gridLayout->setGridPos(block, 0, 0);
        block = createBlock(context, 1, gridLayout);
        gridLayout->setGridPos(block, 0, 1);
        block = createBlock(context, 2, gridLayout);
        gridLayout->setGridPos(block, 1, 0);
        block = createBlock(context, 3, gridLayout);
        gridLayout->setGridPos(block, 1, 1);

        // Create form layouts.
        groupBox = GroupBox::create(context, "Form Layouts", layout);
        groupLayout = HorizontalLayout::create(context, groupBox);
        auto formLayout = FormLayout::create(context, groupLayout);
        formLayout->setHStretch(Stretch::Expanding);
        formLayout->addRow("One:", IntEditSlider::create(context));
        formLayout->addRow("Two:", IntEditSlider::create(context));
        formLayout->addSpacer();
        formLayout->addRow("Three:", IntEdit::create(context));
    }

    Layouts::~Layouts()
    {}

    std::shared_ptr<IWidget> Layouts::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Layouts>(new Layouts);
        out->_init(context, app, parent);
        return out;
    }

    Size2I Layouts::getSizeHint() const
    {
        return _scrollWidget->getSizeHint();
    }

    void Layouts::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }
}
