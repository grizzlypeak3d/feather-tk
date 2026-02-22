// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Buttons.h"

#include <ftk/UI/CheckBox.h>
#include <ftk/UI/GroupBox.h>
#include <ftk/UI/MainWindow.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/RadioButton.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ScrollWidget.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/Core/Format.h>

using namespace ftk;

namespace widgets
{
    void Buttons::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        ftk::IWidget::_init(context, "Buttons", parent);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create push buttons.
        auto groupBox = GroupBox::create(context, "Push Buttons", layout);
        auto hLayout = HorizontalLayout::create(context, groupBox);
        hLayout->setSpacingRole(SizeRole::SpacingSmall);
        auto pushButton = PushButton::create(context, "Click", hLayout);
        pushButton->setClickedCallback(
            []
            {
                std::cout << "Click" << std::endl;
            });
        pushButton = PushButton::create(context, "Text", hLayout);
        pushButton->setIcon("Settings");
        pushButton = PushButton::create(context, "Disabled", hLayout);
        pushButton->setEnabled(false);

        // Create tool buttons.
        groupBox = GroupBox::create(context, "Tool Buttons", layout);
        hLayout = HorizontalLayout::create(context, groupBox);
        hLayout->setSpacingRole(SizeRole::SpacingSmall);
        _toolButtonGroup = ButtonGroup::create(context, ButtonGroupType::Radio);
        _toolButtonGroup->setCheckedCallback(
            [](int index, bool value)
            {
                std::cout << Format("Tool {0}").arg(index) << std::endl;
            });
        const std::vector<std::string> toolIcons =
        {
            "PlaybackReverse",
            "PlaybackStop",
            "PlaybackForward"
        };
        const std::vector<std::string> toolTooltips =
        {
            "Reverse playback",
            "Stop playback",
            "Forward playback"
        };
        auto hLayout2 = HorizontalLayout::create(context, hLayout);
        hLayout2->setSpacingRole(SizeRole::SpacingTool);
        for (size_t i = 0; i < 3; ++i)
        {
            auto toolButton = ToolButton::create(context, hLayout2);
            toolButton->setCheckable(true);
            toolButton->setChecked(0 == i);
            toolButton->setIcon(toolIcons[i]);
            toolButton->setTooltip(toolTooltips[i]);
            _toolButtonGroup->addButton(toolButton);
        }
        auto playStopButton = ToolButton::create(context, hLayout);
        playStopButton->setIcon("PlaybackStop");
        playStopButton->setCheckedIcon("PlaybackForward");
        playStopButton->setCheckable(true);
        auto toolButton = ToolButton::create(context, "Text", hLayout);
        toolButton = ToolButton::create(context, "Text", hLayout);
        toolButton->setIcon("Settings");
        toolButton = ToolButton::create(context, "Disabled", hLayout);
        toolButton->setEnabled(false);
        _toolButtonGroup->addButton(toolButton);

        // Create check boxes.
        groupBox = GroupBox::create(context, "Check Boxes", layout);
        auto vLayout = VerticalLayout::create(context, groupBox);
        vLayout->setSpacingRole(SizeRole::SpacingSmall);
        auto checkBox = CheckBox::create(context, "Check 0", vLayout);
        checkBox->setCheckedCallback(
            [](bool value)
            {
                std::cout << Format("Checked 0: {0}").arg(value) << std::endl;
            });
        checkBox = CheckBox::create(context, "Check 1", vLayout);
        checkBox->setCheckedCallback(
            [](bool value)
            {
                std::cout << Format("Checked 1: {0}").arg(value) << std::endl;
            });
        checkBox = CheckBox::create(context, "Disabled", vLayout);
        checkBox->setEnabled(false);

        // Create radio buttons.
        groupBox = GroupBox::create(context, "Radio Buttons", layout);
        vLayout = VerticalLayout::create(context, groupBox);
        vLayout->setSpacingRole(SizeRole::SpacingSmall);
        _radioButtonGroup = ButtonGroup::create(context, ButtonGroupType::Radio);
        _radioButtonGroup->setCheckedCallback(
            [](int index, bool value)
            {
                std::cout << Format("Radio {0}").arg(index) << std::endl;
            });
        for (size_t i = 0; i < 3; ++i)
        {
            auto radioButton = RadioButton::create(context, vLayout);
            radioButton->setText(Format("Radio {0}").arg(i));
            radioButton->setChecked(0 == i);
            _radioButtonGroup->addButton(radioButton);
        }
        auto radioButton = RadioButton::create(context, "Disabled", vLayout);
        radioButton->setEnabled(false);
    }

    Buttons::~Buttons()
    {}

    std::shared_ptr<IWidget> Buttons::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Buttons>(new Buttons);
        out->_init(context, app, parent);
        return out;
    }
    
    Size2I Buttons::getSizeHint() const
    {
        return _scrollWidget->getSizeHint();
    }

    void Buttons::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }
}
