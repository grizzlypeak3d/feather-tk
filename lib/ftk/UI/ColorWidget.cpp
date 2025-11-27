// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ColorWidget.h>

#include <ftk/UI/ColorSwatch.h>
#include <ftk/UI/FloatEditSlider.h>
#include <ftk/UI/GridLayout.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/TabWidget.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <sstream>

namespace ftk
{
    FTK_ENUM_IMPL(
        ColorWidgetMode,
        "RGB",
        "HSV");

    struct ColorWidget::Private
    {
        Color4F color;
        float hsv[3] = { 0.F, 0.F, 0.F };
        ColorWidgetMode mode = ColorWidgetMode::RGB;

        std::shared_ptr<ColorSwatch> swatch;
        std::map<std::string, std::shared_ptr<FloatEditSlider> > sliders;
        std::shared_ptr<HorizontalLayout> layout;
        std::shared_ptr<TabWidget> tabWidget;
        std::shared_ptr<GridLayout> rgbLayout;
        std::shared_ptr<GridLayout> hsvLayout;

        std::function<void(const Color4F&)> callback;
    };

    void ColorWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::ColorWidget", parent);
        FTK_P();

        p.swatch = ColorSwatch::create(context);
        p.swatch->setSizeRole(SizeRole::SwatchLarge);

        p.sliders["RGB/R"] = FloatEditSlider::create(context);
        p.sliders["RGB/G"] = FloatEditSlider::create(context);
        p.sliders["RGB/B"] = FloatEditSlider::create(context);
        p.sliders["RGB/A"] = FloatEditSlider::create(context);

        p.sliders["HSV/H"] = FloatEditSlider::create(context);
        p.sliders["HSV/S"] = FloatEditSlider::create(context);
        p.sliders["HSV/V"] = FloatEditSlider::create(context);
        p.sliders["HSV/A"] = FloatEditSlider::create(context);

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingSmall);
        p.swatch->setParent(p.layout);

        p.tabWidget = TabWidget::create(context, p.layout);
        p.tabWidget->setHStretch(Stretch::Expanding);

        p.rgbLayout = GridLayout::create(context);
        p.rgbLayout->setMarginRole(SizeRole::MarginSmall);
        p.rgbLayout->setSpacingRole(SizeRole::SpacingSmall);
        p.tabWidget->addTab("RGB", p.rgbLayout);
        auto label = Label::create(context, "R:", p.rgbLayout);
        p.rgbLayout->setGridPos(label, 0, 0);
        p.sliders["RGB/R"]->setParent(p.rgbLayout);
        p.rgbLayout->setGridPos(p.sliders["RGB/R"], 0, 1);
        label = Label::create(context, "G:", p.rgbLayout);
        p.rgbLayout->setGridPos(label, 1, 0);
        p.sliders["RGB/G"]->setParent(p.rgbLayout);
        p.rgbLayout->setGridPos(p.sliders["RGB/G"], 1, 1);
        label = Label::create(context, "B:", p.rgbLayout);
        p.rgbLayout->setGridPos(label, 2, 0);
        p.sliders["RGB/B"]->setParent(p.rgbLayout);
        p.rgbLayout->setGridPos(p.sliders["RGB/B"], 2, 1);
        label = Label::create(context, "A:", p.rgbLayout);
        p.rgbLayout->setGridPos(label, 3, 0);
        p.sliders["RGB/A"]->setParent(p.rgbLayout);
        p.rgbLayout->setGridPos(p.sliders["RGB/A"], 3, 1);

        p.hsvLayout = GridLayout::create(context);
        p.hsvLayout->setMarginRole(SizeRole::MarginSmall);
        p.hsvLayout->setSpacingRole(SizeRole::SpacingSmall);
        p.tabWidget->addTab("HSV", p.hsvLayout);
        label = Label::create(context, "H:", p.hsvLayout);
        p.hsvLayout->setGridPos(label, 0, 0);
        p.sliders["HSV/H"]->setParent(p.hsvLayout);
        p.hsvLayout->setGridPos(p.sliders["HSV/H"], 0, 1);
        label = Label::create(context, "S:", p.hsvLayout);
        p.hsvLayout->setGridPos(label, 1, 0);
        p.sliders["HSV/S"]->setParent(p.hsvLayout);
        p.hsvLayout->setGridPos(p.sliders["HSV/S"], 1, 1);
        label = Label::create(context, "V:", p.hsvLayout);
        p.hsvLayout->setGridPos(label, 2, 0);
        p.sliders["HSV/V"]->setParent(p.hsvLayout);
        p.hsvLayout->setGridPos(p.sliders["HSV/V"], 2, 1);
        label = Label::create(context, "A:", p.hsvLayout);
        p.hsvLayout->setGridPos(label, 3, 0);
        p.sliders["HSV/A"]->setParent(p.hsvLayout);
        p.hsvLayout->setGridPos(p.sliders["HSV/A"], 3, 1);

        _modeUpdate();
        _colorUpdate();

        p.tabWidget->setCurrentTabCallback(
            [this](int value)
            {
                _p->mode = static_cast<ColorWidgetMode>(value);
            });

        p.sliders["RGB/R"]->setCallback(
            [this](float value)
            {
                FTK_P();
                if (p.rgbLayout == p.tabWidget->getCurrentWidget())
                {
                    p.color.r = value;
                    float rgb[3] = { p.color.r, p.color.g, p.color.b };
                    rgbToHSV(rgb, p.hsv);
                    _colorUpdate();
                    if (p.callback)
                    {
                        p.callback(p.color);
                    }
                }
            });
        p.sliders["RGB/G"]->setCallback(
            [this](float value)
            {
                FTK_P();
                if (p.rgbLayout == p.tabWidget->getCurrentWidget())
                {
                    p.color.g = value;
                    float rgb[3] = { p.color.r, p.color.g, p.color.b };
                    rgbToHSV(rgb, p.hsv);
                    _colorUpdate();
                    if (p.callback)
                    {
                        p.callback(p.color);
                    }
                }
            });
        p.sliders["RGB/B"]->setCallback(
            [this](float value)
            {
                FTK_P();
                if (p.rgbLayout == p.tabWidget->getCurrentWidget())
                {
                    p.color.b = value;
                    float rgb[3] = { p.color.r, p.color.g, p.color.b };
                    rgbToHSV(rgb, p.hsv);
                    _colorUpdate();
                    if (p.callback)
                    {
                        p.callback(p.color);
                    }
                }
            });
        p.sliders["RGB/A"]->setCallback(
            [this](float value)
            {
                FTK_P();
                if (p.rgbLayout == p.tabWidget->getCurrentWidget())
                {
                    p.color.a = value;
                    _colorUpdate();
                    if (p.callback)
                    {
                        p.callback(p.color);
                    }
                }
            });

        p.sliders["HSV/H"]->setCallback(
            [this](float value)
            {
                FTK_P();
                if (p.hsvLayout == p.tabWidget->getCurrentWidget())
                {
                    p.hsv[0] = value;
                    float rgb[3] = { 0.F, 0.F, 0.F };
                    hsvToRGB(p.hsv, rgb);
                    p.color.r = rgb[0];
                    p.color.g = rgb[1];
                    p.color.b = rgb[2];
                    _colorUpdate();
                    if (p.callback)
                    {
                        p.callback(p.color);
                    }
                }
            });
        p.sliders["HSV/S"]->setCallback(
            [this](float value)
            {
                FTK_P();
                if (p.hsvLayout == p.tabWidget->getCurrentWidget())
                {
                    p.hsv[1] = value;
                    float rgb[3] = { 0.F, 0.F, 0.F };
                    hsvToRGB(p.hsv, rgb);
                    p.color.r = rgb[0];
                    p.color.g = rgb[1];
                    p.color.b = rgb[2];
                    _colorUpdate();
                    if (p.callback)
                    {
                        p.callback(p.color);
                    }
                }
            });
        p.sliders["HSV/V"]->setCallback(
            [this](float value)
            {
                FTK_P();
                if (p.hsvLayout == p.tabWidget->getCurrentWidget())
                {
                    p.hsv[2] = value;
                    float rgb[3] = { 0.F, 0.F, 0.F };
                    hsvToRGB(p.hsv, rgb);
                    p.color.r = rgb[0];
                    p.color.g = rgb[1];
                    p.color.b = rgb[2];
                    _colorUpdate();
                    if (p.callback)
                    {
                        p.callback(p.color);
                    }
                }
            });
        p.sliders["HSV/A"]->setCallback(
            [this](float value)
            {
                FTK_P();
                if (p.hsvLayout == p.tabWidget->getCurrentWidget())
                {
                    p.color.a = value;
                    _colorUpdate();
                    if (p.callback)
                    {
                        p.callback(p.color);
                    }
                }
            });
    }

    ColorWidget::ColorWidget() :
        _p(new Private)
    {}

    ColorWidget::~ColorWidget()
    {}

    std::shared_ptr<ColorWidget> ColorWidget::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ColorWidget>(new ColorWidget);
        out->_init(context, parent);
        return out;
    }

    const Color4F& ColorWidget::getColor() const
    {
        return _p->color;
    }

    void ColorWidget::setColor(const Color4F& value)
    {
        FTK_P();
        if (value == p.color)
            return;
        p.color = value;
        float rgb[3] = { p.color.r, p.color.g, p.color.b };
        rgbToHSV(rgb, p.hsv);
        _colorUpdate();
        setDrawUpdate();
    }

    void ColorWidget::setCallback(const std::function<void(const Color4F&)>& value)
    {
        _p->callback = value;
    }

    ColorWidgetMode ColorWidget::getMode() const
    {
        return _p->mode;
    }

    void ColorWidget::setMode(ColorWidgetMode value)
    {
        FTK_P();
        if (value == p.mode)
            return;
        p.mode = value;
        _modeUpdate();
    }

    void ColorWidget::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    void ColorWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        setSizeHint(_p->layout->getSizeHint());
    }

    void ColorWidget::_modeUpdate()
    {
        FTK_P();
        p.tabWidget->setCurrentTab(static_cast<int>(p.mode));
    }

    void ColorWidget::_colorUpdate()
    {
        FTK_P();

        p.swatch->setColor(p.color);

        p.sliders["RGB/R"]->setValue(p.color.r);
        p.sliders["RGB/G"]->setValue(p.color.g);
        p.sliders["RGB/B"]->setValue(p.color.b);
        p.sliders["RGB/A"]->setValue(p.color.a);

        p.sliders["HSV/H"]->setValue(p.hsv[0]);
        p.sliders["HSV/S"]->setValue(p.hsv[1]);
        p.sliders["HSV/V"]->setValue(p.hsv[2]);
        p.sliders["HSV/A"]->setValue(p.color.a);
    }
}