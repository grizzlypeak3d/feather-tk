// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "Sliders.h"

#include <ftk/UI/GroupBox.h>
#include <ftk/UI/DoubleEditSlider.h>
#include <ftk/UI/FloatEditSlider.h>
#include <ftk/UI/IntEditSlider.h>
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
        auto vLayout = VerticalLayout::create(context, groupBox);
        auto intEditSlider = IntEditSlider::create(context, vLayout);
        intEditSlider->setPressedCallback(
            [](int value, bool pressed)
            {
                std::cout << Format("Int: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        intEditSlider = IntEditSlider::create(context, vLayout);
        intEditSlider->setRange(100, 200);
        intEditSlider->setDefaultValue(100);
        intEditSlider->setPressedCallback(
            [](int value, bool pressed)
            {
                std::cout << Format("Int: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        intEditSlider = IntEditSlider::create(context, vLayout);
        intEditSlider->setRange(-100, 100);
        intEditSlider->setDefaultValue(-100);
        intEditSlider->setPressedCallback(
            [](int value, bool pressed)
            {
                std::cout << Format("Int: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });

        // Create float sliders.
        groupBox = GroupBox::create(context, "Float Sliders", layout);
        vLayout = VerticalLayout::create(context, groupBox);
        auto floatEditSlider = FloatEditSlider::create(context, vLayout);
        floatEditSlider->setPressedCallback(
            [](float value, bool pressed)
            {
                std::cout << Format("Float: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        floatEditSlider = FloatEditSlider::create(context, vLayout);
        floatEditSlider->setRange(100.F, 200.F);
        floatEditSlider->setDefaultValue(100.F);
        floatEditSlider->setStep(1.F);
        floatEditSlider->setLargeStep(10.F);
        floatEditSlider->setPressedCallback(
            [](float value, bool pressed)
            {
                std::cout << Format("Float: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        floatEditSlider = FloatEditSlider::create(context, vLayout);
        floatEditSlider->setRange(-100.F, 100.F);
        floatEditSlider->setDefaultValue(-100.F);
        floatEditSlider->setStep(1.F);
        floatEditSlider->setLargeStep(10.F);
        floatEditSlider->setPressedCallback(
            [](float value, bool pressed)
            {
                std::cout << Format("Float: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });

        // Create double sliders.
        groupBox = GroupBox::create(context, "Double Sliders", layout);
        vLayout = VerticalLayout::create(context, groupBox);
        auto doubleEditSlider = DoubleEditSlider::create(context, vLayout);
        doubleEditSlider->setPressedCallback(
            [](double value, bool pressed)
            {
                std::cout << Format("Double: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        doubleEditSlider = DoubleEditSlider::create(context, vLayout);
        doubleEditSlider->setRange(100.0, 200.0);
        doubleEditSlider->setDefaultValue(100.0);
        doubleEditSlider->setStep(1.F);
        doubleEditSlider->setLargeStep(10.F);
        doubleEditSlider->setPressedCallback(
            [](double value, bool pressed)
            {
                std::cout << Format("Double: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
        doubleEditSlider = DoubleEditSlider::create(context, vLayout);
        doubleEditSlider->setRange(-100.0, 100.0);
        doubleEditSlider->setDefaultValue(-100.0);
        doubleEditSlider->setStep(1.F);
        doubleEditSlider->setLargeStep(10.F);
        doubleEditSlider->setPressedCallback(
            [](double value, bool pressed)
            {
                std::cout << Format("Double: {0} {1}").
                    arg(value).
                    arg(pressed ? "(pressed)" : "") << std::endl;
            });
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
