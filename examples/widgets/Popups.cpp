// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Popups.h"

#include <ftk/UI/Action.h>
#include <ftk/UI/ComboBox.h>
#include <ftk/UI/ColorSwatch.h>
#include <ftk/UI/GroupBox.h>
#include <ftk/UI/MenuBar.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Format.h>

using namespace ftk;

namespace widgets
{
    void Popups::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        ftk::IWidget::_init(context, "Popups", parent);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create combo boxes.
        auto groupBox = GroupBox::create(context, "Combo Boxes", layout);
        auto vLayout = VerticalLayout::create(context, groupBox);
        auto comboBox = ComboBox::create(context, vLayout);
        comboBox->setItems({
            ComboBoxItem("Stop", "PlaybackStop"),
            ComboBoxItem("Forward", "PlaybackForward"),
            ComboBoxItem("Reverse", "PlaybackReverse")
            });
        comboBox->setTooltip("Playback");
        comboBox = ComboBox::create(context, getImageTypeLabels(), vLayout);
        comboBox->setTooltip("Image types");
        comboBox = ComboBox::create(context, vLayout);

        // Create color swatches.
        groupBox = GroupBox::create(context, "Color Popups", layout);
        auto hLayout = HorizontalLayout::create(context, groupBox);
        const std::vector<Color4F> colors =
        {
            Color4F(1.F, .5F, .5F),
            Color4F(.5F, 1.F, .5F),
            Color4F(.5F, .5F, 1.F),
            Color4F(1.F, 1.F, .5F),
            Color4F(.5F, 1.F, 1.F),
            Color4F(1.F, .5F, 1.F)
        };
        for (const auto& color : colors)
        {
            auto colorSwatch = ColorSwatch::create(context, hLayout);
            colorSwatch->setColor(color);
            colorSwatch->setEditable(true);
            colorSwatch->setPressedCallback(
                [](const Color4F& value, bool pressed)
                {
                    std::cout << Format("Color: {0} {1}").
                        arg(value).
                        arg(pressed ? "(pressed)" : "") << std::endl;
                });
        }
    }

    Popups::~Popups()
    {}

    std::shared_ptr<IWidget> Popups::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Popups>(new Popups);
        out->_init(context, app, parent);
        return out;
    }
    
    Size2I Popups::getSizeHint() const
    {
        return _scrollWidget->getSizeHint();
    }

    void Popups::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }
}
