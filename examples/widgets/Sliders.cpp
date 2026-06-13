// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Sliders.h"

#include <ftk/UI/GroupBox.h>
#include <ftk/UI/DoubleEdit.h>
#include <ftk/UI/DoubleSlider.h>
#include <ftk/UI/FloatEdit.h>
#include <ftk/UI/FloatSlider.h>
#include <ftk/UI/GridLayout.h>
#include <ftk/UI/IntEdit.h>
#include <ftk/UI/IntSlider.h>
#include <ftk/UI/MainWindow.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Format.h>

using namespace ftk;

namespace widgets
{
    void Sliders::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        ftk::IWidget::_init(context, "Sliders", parent);

        // Create a layout.
        auto layout = VerticalLayout::create(context);
        layout->setMarginRole(SizeRole::Margin);
        _scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        _scrollWidget->setBorder(false);
        _scrollWidget->setWidget(layout);

        // Create integer sliders.
        auto groupBox = GroupBox::create(context, "Integer Sliders", layout);
        auto gridLayout = GridLayout::create(context, groupBox);

        auto intEdit = IntEdit::create(context, gridLayout);
        gridLayout->setGridPos(intEdit, 0, 0);
        auto intSlider = IntSlider::create(context, intEdit->getModel(), gridLayout);
        intSlider->setPressedCallback(
            [](int value, bool pressed)
            {
                std::cout << Format("Int: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        gridLayout->setGridPos(intSlider, 0, 1);
        auto intReset = IntResetButton::create(context, intEdit->getModel(), gridLayout);
        gridLayout->setGridPos(intReset, 0, 2);

        intEdit = IntEdit::create(context, gridLayout);
        intEdit->setRange(100, 200);
        intEdit->setDefault(100);
        gridLayout->setGridPos(intEdit, 1, 0);
        intSlider = IntSlider::create(context, intEdit->getModel(), gridLayout);
        intSlider->setPressedCallback(
            [](int value, bool pressed)
            {
                std::cout << Format("Int: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        gridLayout->setGridPos(intSlider, 1, 1);
        intReset = IntResetButton::create(context, intEdit->getModel(), gridLayout);
        gridLayout->setGridPos(intReset, 1, 2);

        intEdit = IntEdit::create(context, gridLayout);
        intEdit->setRange(-100, 100);
        intEdit->setDefault(-100);
        gridLayout->setGridPos(intEdit, 2, 0);
        intSlider = IntSlider::create(context, intEdit->getModel(), gridLayout);
        intSlider->setPressedCallback(
            [](int value, bool pressed)
            {
                std::cout << Format("Int: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        gridLayout->setGridPos(intSlider, 2, 1);
        intReset = IntResetButton::create(context, intEdit->getModel(), gridLayout);
        gridLayout->setGridPos(intReset, 2, 2);

        // Create float sliders.
        groupBox = GroupBox::create(context, "Float Sliders", layout);
        gridLayout = GridLayout::create(context, groupBox);

        auto floatEdit = FloatEdit::create(context, gridLayout);
        gridLayout->setGridPos(floatEdit, 0, 0);
        auto floatSlider = FloatSlider::create(context, floatEdit->getModel(), gridLayout);
        floatSlider->setPressedCallback(
            [](float value, bool pressed)
            {
                std::cout << Format("Float: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        gridLayout->setGridPos(floatSlider, 0, 1);
        auto floatReset = FloatResetButton::create(context, floatEdit->getModel(), gridLayout);
        gridLayout->setGridPos(floatReset, 0, 2);

        floatEdit = FloatEdit::create(context, gridLayout);
        floatEdit->setRange(100.F, 200.F);
        floatEdit->setDefault(100.F);
        floatEdit->setStep(1.F);
        floatEdit->setLargeStep(10.F);
        gridLayout->setGridPos(floatEdit, 1, 0);
        floatSlider = FloatSlider::create(context, floatEdit->getModel(), gridLayout);
        floatSlider->setPressedCallback(
            [](float value, bool pressed)
            {
                std::cout << Format("Float: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        gridLayout->setGridPos(floatSlider, 1, 1);
        floatReset = FloatResetButton::create(context, floatEdit->getModel(), gridLayout);
        gridLayout->setGridPos(floatReset, 1, 2);

        floatEdit = FloatEdit::create(context, gridLayout);
        floatEdit->setRange(-100.F, 100.F);
        floatEdit->setDefault(-100.F);
        floatEdit->setStep(1.F);
        floatEdit->setLargeStep(10.F);
        gridLayout->setGridPos(floatEdit, 2, 0);
        floatSlider = FloatSlider::create(context, floatEdit->getModel(), gridLayout);
        floatSlider->setPressedCallback(
            [](float value, bool pressed)
            {
                std::cout << Format("Float: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        gridLayout->setGridPos(floatSlider, 2, 1);
        floatReset = FloatResetButton::create(context, floatEdit->getModel(), gridLayout);
        gridLayout->setGridPos(floatReset, 2, 2);

        // Create double sliders.
        groupBox = GroupBox::create(context, "Double Sliders", layout);
        gridLayout = GridLayout::create(context, groupBox);
        
        auto doubleEdit = DoubleEdit::create(context, gridLayout);
        gridLayout->setGridPos(doubleEdit, 0, 0);
        auto doubleSlider = DoubleSlider::create(context, doubleEdit->getModel(), gridLayout);
        doubleSlider->setPressedCallback(
            [](double value, bool pressed)
            {
                std::cout << Format("Double: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        gridLayout->setGridPos(doubleSlider, 0, 1);
        auto doubleReset = DoubleResetButton::create(context, doubleEdit->getModel(), gridLayout);
        gridLayout->setGridPos(doubleReset, 0, 2);

        doubleEdit = DoubleEdit::create(context, gridLayout);
        doubleEdit->setRange(100.0, 200.0);
        doubleEdit->setDefault(100.0);
        doubleEdit->setStep(1.F);
        doubleEdit->setLargeStep(10.F);
        gridLayout->setGridPos(doubleEdit, 1, 0);
        doubleSlider = DoubleSlider::create(context, doubleEdit->getModel(), gridLayout);
        doubleSlider->setPressedCallback(
            [](double value, bool pressed)
            {
                std::cout << Format("Double: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        gridLayout->setGridPos(doubleSlider, 1, 1);
        doubleReset = DoubleResetButton::create(context, doubleEdit->getModel(), gridLayout);
        gridLayout->setGridPos(doubleReset, 1, 2);

        doubleEdit = DoubleEdit::create(context, gridLayout);
        doubleEdit->setRange(-100.0, 100.0);
        doubleEdit->setDefault(-100.0);
        doubleEdit->setStep(1.F);
        doubleEdit->setLargeStep(10.F);
        gridLayout->setGridPos(doubleEdit, 2, 0);
        doubleSlider = DoubleSlider::create(context, doubleEdit->getModel(), gridLayout);
        doubleSlider->setPressedCallback(
            [](double value, bool pressed)
            {
                std::cout << Format("Double: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        gridLayout->setGridPos(doubleSlider, 2, 1);
        doubleReset = DoubleResetButton::create(context, doubleEdit->getModel(), gridLayout);
        gridLayout->setGridPos(doubleReset, 2, 2);
    }

    Sliders::~Sliders()
    {}

    std::shared_ptr<IWidget> Sliders::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<App>& app,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Sliders>(new Sliders);
        out->_init(context, app, parent);
        return out;
    }
    
    Size2I Sliders::getSizeHint() const
    {
        return _scrollWidget->getSizeHint();
    }

    void Sliders::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _scrollWidget->setGeometry(value);
    }
}
