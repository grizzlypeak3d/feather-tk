// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ColorWidget.h>

#include <ftk/UI/ColorSlider.h>
#include <ftk/UI/ColorSwatch.h>
#include <ftk/UI/Divider.h>
#include <ftk/UI/FloatEdit.h>
#include <ftk/UI/GridLayout.h>
#include <ftk/UI/IntEdit.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/TabBar.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <sstream>

namespace ftk
{
    struct RGBColorWidget::Private
    {
        Color4F color;

        std::shared_ptr<ColorSwatch> swatch;
        std::map<std::string, std::shared_ptr<ColorFloatEditSlider> > sliders;
        std::shared_ptr<HorizontalLayout> layout;

        std::function<void(const Color4F&)> callback;
        std::function<void(const Color4F&, bool)> pressedCallback;
    };

    void RGBColorWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::RGBColorWidget", parent);
        FTK_P();

        p.swatch = ColorSwatch::create(context);
        p.swatch->setSizeRole(SizeRole::SwatchLarge);

        p.sliders["R"] = ColorFloatEditSlider::create(context);
        p.sliders["R"]->setColors({ V4F(0.F, 0.F, 0.F), V4F(1.F, 0.F, 0.F) });
        p.sliders["G"] = ColorFloatEditSlider::create(context);
        p.sliders["G"]->setColors({ V4F(0.F, 0.F, 0.F), V4F(0.F, 1.F, 0.F) });
        p.sliders["B"] = ColorFloatEditSlider::create(context);
        p.sliders["B"]->setColors({ V4F(0.F, 0.F, 0.F), V4F(0.F, 0.F, 1.F) });
        p.sliders["A"] = ColorFloatEditSlider::create(context);
        p.sliders["A"]->setColors({ V4F(0.F, 0.F, 0.F), V4F(1.F, 1.F, 1.F) });

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingSmall);
        p.swatch->setParent(p.layout);

        auto gridLayout = GridLayout::create(context, p.layout);
        gridLayout->setMarginRole(SizeRole::MarginSmall);
        gridLayout->setSpacingRole(SizeRole::SpacingSmall);
        gridLayout->setHStretch(Stretch::Expanding);

        auto label = Label::create(context, "R:", gridLayout);
        gridLayout->setGridPos(label, 0, 0);
        p.sliders["R"]->setParent(gridLayout);
        gridLayout->setGridPos(p.sliders["R"], 0, 1);

        label = Label::create(context, "G:", gridLayout);
        gridLayout->setGridPos(label, 1, 0);
        p.sliders["G"]->setParent(gridLayout);
        gridLayout->setGridPos(p.sliders["G"], 1, 1);

        label = Label::create(context, "B:", gridLayout);
        gridLayout->setGridPos(label, 2, 0);
        p.sliders["B"]->setParent(gridLayout);
        gridLayout->setGridPos(p.sliders["B"], 2, 1);

        label = Label::create(context, "A:", gridLayout);
        gridLayout->setGridPos(label, 3, 0);
        p.sliders["A"]->setParent(gridLayout);
        gridLayout->setGridPos(p.sliders["A"], 3, 1);

        _colorUpdate();

        p.sliders["R"]->setPressedCallback(
            [this](float value, bool pressed)
            {
                FTK_P();
                p.color.r = value;
                _colorUpdate();
                if (p.callback)
                {
                    p.callback(p.color);
                }
                if (p.pressedCallback)
                {
                    p.pressedCallback(p.color, pressed);
                }
            });

        p.sliders["G"]->setPressedCallback(
            [this](float value, bool pressed)
            {
                FTK_P();
                p.color.g = value;
                _colorUpdate();
                if (p.callback)
                {
                    p.callback(p.color);
                }
                if (p.pressedCallback)
                {
                    p.pressedCallback(p.color, pressed);
                }
            });

        p.sliders["B"]->setPressedCallback(
            [this](float value, bool pressed)
            {
                FTK_P();
                p.color.b = value;
                _colorUpdate();
                if (p.callback)
                {
                    p.callback(p.color);
                }
                if (p.pressedCallback)
                {
                    p.pressedCallback(p.color, pressed);
                }
            });

        p.sliders["A"]->setPressedCallback(
            [this](float value, bool pressed)
            {
                FTK_P();
                p.color.a = value;
                _colorUpdate();
                if (p.callback)
                {
                    p.callback(p.color);
                }
                if (p.pressedCallback)
                {
                    p.pressedCallback(p.color, pressed);
                }
            });
    }

    RGBColorWidget::RGBColorWidget() :
        _p(new Private)
    {}

    RGBColorWidget::~RGBColorWidget()
    {}

    std::shared_ptr<RGBColorWidget> RGBColorWidget::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<RGBColorWidget>(new RGBColorWidget);
        out->_init(context, parent);
        return out;
    }

    const Color4F& RGBColorWidget::getColor() const
    {
        return _p->color;
    }

    void RGBColorWidget::setColor(const Color4F& value)
    {
        FTK_P();
        if (value == p.color)
            return;
        p.color = value;
        _colorUpdate();
        setDrawUpdate();
    }

    void RGBColorWidget::setCallback(const std::function<void(const Color4F&)>& value)
    {
        _p->callback = value;
    }

    void RGBColorWidget::setPressedCallback(const std::function<void(const Color4F&, bool)>& value)
    {
        _p->pressedCallback = value;
    }

    SizeRole RGBColorWidget::getMarginRole() const
    {
        return _p->layout->getMarginRole();
    }

    void RGBColorWidget::setMarginRole(SizeRole value)
    {
        _p->layout->setMarginRole(value);
    }

    Size2I RGBColorWidget::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void RGBColorWidget::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    void RGBColorWidget::_colorUpdate()
    {
        FTK_P();

        p.swatch->setColor(p.color);

        p.sliders["R"]->setValue(p.color.r);
        p.sliders["G"]->setValue(p.color.g);
        p.sliders["B"]->setValue(p.color.b);
        p.sliders["A"]->setValue(p.color.a);
    }

    struct HSVColorWidget::Private
    {
        Color4F color;
        float hsv[3] = { 0.F, 0.F, 0.F };

        std::shared_ptr<ColorSwatch> swatch;
        std::shared_ptr<ColorIntEditSlider> hueSlider;
        std::map<std::string, std::shared_ptr<ColorFloatEditSlider> > sliders;
        std::shared_ptr<HorizontalLayout> layout;

        std::function<void(const Color4F&)> callback;
        std::function<void(const Color4F&, bool)> pressedCallback;
    };

    void HSVColorWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::HSVColorWidget", parent);
        FTK_P();

        p.swatch = ColorSwatch::create(context);
        p.swatch->setSizeRole(SizeRole::SwatchLarge);

        p.hueSlider = ColorIntEditSlider::create(context);
        p.hueSlider->setColors({
            hue(6.0 / 6.0) * V4F(1.F, 0.F, 0.F),
            hue(5.0 / 6.0) * V4F(1.F, 0.F, 0.F),
            hue(4.0 / 6.0) * V4F(1.F, 0.F, 0.F),
            hue(3.0 / 6.0) * V4F(1.F, 0.F, 0.F),
            hue(2.0 / 6.0) * V4F(1.F, 0.F, 0.F),
            hue(1.0 / 6.0) * V4F(1.F, 0.F, 0.F),
            hue(0.0 / 6.0) * V4F(1.F, 0.F, 0.F) });
        p.hueSlider->setRange(0, 360);
        p.hueSlider->getModel()->setStep(10);
        p.hueSlider->getModel()->setLargeStep(60);
        p.sliders["S"] = ColorFloatEditSlider::create(context);
        p.sliders["V"] = ColorFloatEditSlider::create(context);
        p.sliders["V"]->setColors({ V4F(0.F, 0.F, 0.F), V4F(1.F, 1.F, 1.F) });
        p.sliders["A"] = ColorFloatEditSlider::create(context);
        p.sliders["A"]->setColors({ V4F(0.F, 0.F, 0.F), V4F(1.F, 1.F, 1.F) });

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingSmall);
        p.swatch->setParent(p.layout);

        auto gridLayout = GridLayout::create(context, p.layout);
        gridLayout->setMarginRole(SizeRole::MarginSmall);
        gridLayout->setSpacingRole(SizeRole::SpacingSmall);
        gridLayout->setHStretch(Stretch::Expanding);

        auto label = Label::create(context, "H:", gridLayout);
        gridLayout->setGridPos(label, 0, 0);
        p.hueSlider->setParent(gridLayout);
        gridLayout->setGridPos(p.hueSlider, 0, 1);

        label = Label::create(context, "S:", gridLayout);
        gridLayout->setGridPos(label, 1, 0);
        p.sliders["S"]->setParent(gridLayout);
        gridLayout->setGridPos(p.sliders["S"], 1, 1);

        label = Label::create(context, "V:", gridLayout);
        gridLayout->setGridPos(label, 2, 0);
        p.sliders["V"]->setParent(gridLayout);
        gridLayout->setGridPos(p.sliders["V"], 2, 1);

        label = Label::create(context, "A:", gridLayout);
        gridLayout->setGridPos(label, 3, 0);
        p.sliders["A"]->setParent(gridLayout);
        gridLayout->setGridPos(p.sliders["A"], 3, 1);

        _colorUpdate();

        p.hueSlider->setPressedCallback(
            [this](int value, bool pressed)
            {
                FTK_P();
                p.hsv[0] = value / 360.F;
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
                if (p.pressedCallback)
                {
                    p.pressedCallback(p.color, pressed);
                }
            });

        p.sliders["S"]->setPressedCallback(
            [this](float value, bool pressed)
            {
                FTK_P();
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
                if (p.pressedCallback)
                {
                    p.pressedCallback(p.color, pressed);
                }
            });

        p.sliders["V"]->setPressedCallback(
            [this](float value, bool pressed)
            {
                FTK_P();
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
                if (p.pressedCallback)
                {
                    p.pressedCallback(p.color, pressed);
                }
            });

        p.sliders["A"]->setPressedCallback(
            [this](float value, bool pressed)
            {
                FTK_P();
                p.color.a = value;
                _colorUpdate();
                if (p.callback)
                {
                    p.callback(p.color);
                }
                if (p.pressedCallback)
                {
                    p.pressedCallback(p.color, pressed);
                }
            });
    }

    HSVColorWidget::HSVColorWidget() :
        _p(new Private)
    {}

    HSVColorWidget::~HSVColorWidget()
    {}

    std::shared_ptr<HSVColorWidget> HSVColorWidget::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<HSVColorWidget>(new HSVColorWidget);
        out->_init(context, parent);
        return out;
    }

    const Color4F& HSVColorWidget::getColor() const
    {
        return _p->color;
    }

    void HSVColorWidget::setColor(const Color4F& value)
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

    void HSVColorWidget::setCallback(const std::function<void(const Color4F&)>& value)
    {
        _p->callback = value;
    }

    void HSVColorWidget::setPressedCallback(const std::function<void(const Color4F&, bool)>& value)
    {
        _p->pressedCallback = value;
    }

    SizeRole HSVColorWidget::getMarginRole() const
    {
        return _p->layout->getMarginRole();
    }

    void HSVColorWidget::setMarginRole(SizeRole value)
    {
        _p->layout->setMarginRole(value);
    }

    Size2I HSVColorWidget::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void HSVColorWidget::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    void HSVColorWidget::_colorUpdate()
    {
        FTK_P();

        p.swatch->setColor(p.color);

        p.hueSlider->setValue(p.hsv[0] * 360);
        float hsv[3] = { p.hsv[0], 1.F, 1.F };
        float rgb[3] = { 0.F, 0.F, 0.F };
        hsvToRGB(hsv, rgb);
        p.sliders["S"]->setColors({ V4F(1.F, 1.F, 1.F), V4F(rgb[0], rgb[1], rgb[2])});
        p.sliders["S"]->setValue(p.hsv[1]);
        p.sliders["V"]->setValue(p.hsv[2]);
        p.sliders["A"]->setValue(p.color.a);
    }

    FTK_ENUM_IMPL(
        ColorWidgetMode,
        "RGB",
        "HSV");

    struct ColorWidget::Private
    {
        Color4F color;
        ColorWidgetMode mode = ColorWidgetMode::RGB;

        std::shared_ptr<RGBColorWidget> rgbWidget;
        std::shared_ptr<HSVColorWidget> hsvWidget;
        std::shared_ptr<TabBar> tabBar;
        std::shared_ptr<VerticalLayout> layout;

        std::function<void(const Color4F&)> callback;
        std::function<void(const Color4F&, bool)> pressedCallback;
    };

    void ColorWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::ColorWidget", parent);
        FTK_P();

        p.tabBar = TabBar::create(context);
        for (const auto& mode : getColorWidgetModeLabels())
        {
            p.tabBar->addTab(mode);
        }

        p.layout = VerticalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::None);
        p.tabBar->setParent(p.layout);
        Divider::create(context, Orientation::Vertical, p.layout);

        _modeUpdate();
        _colorUpdate();

        p.tabBar->setCallback(
            [this](int value)
            {
                _p->mode = static_cast<ColorWidgetMode>(value);
                _modeUpdate();
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
        _colorUpdate();
        setDrawUpdate();
    }

    void ColorWidget::setCallback(const std::function<void(const Color4F&)>& value)
    {
        _p->callback = value;
    }

    void ColorWidget::setPressedCallback(const std::function<void(const Color4F&, bool)>& value)
    {
        _p->pressedCallback = value;
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

    Size2I ColorWidget::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void ColorWidget::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    void ColorWidget::_modeUpdate()
    {
        FTK_P();
        p.tabBar->setCurrent(static_cast<int>(p.mode));
        if (p.rgbWidget)
        {
            p.rgbWidget->setParent(nullptr);
            p.rgbWidget.reset();
        }
        if (p.hsvWidget)
        {
            p.hsvWidget->setParent(nullptr);
            p.hsvWidget.reset();
        }
        auto context = getContext();
        switch (p.mode)
        {
        case ColorWidgetMode::RGB:
            p.rgbWidget = RGBColorWidget::create(context, p.layout);
            p.rgbWidget->setColor(p.color);
            p.rgbWidget->setCallback(
                [this](const Color4F& value)
                {
                    FTK_P();
                    p.color = value;
                    if (p.callback)
                    {
                        p.callback(value);
                    }
                });
            p.rgbWidget->setPressedCallback(
                [this](const Color4F& value, bool pressed)
                {
                    FTK_P();
                    p.color = value;
                    if (p.pressedCallback)
                    {
                        p.pressedCallback(value, pressed);
                    }
                });
            p.rgbWidget->setMarginRole(SizeRole::MarginInside);
            break;
        case ColorWidgetMode::HSV:
            p.hsvWidget = HSVColorWidget::create(context, p.layout);
            p.hsvWidget->setColor(p.color);
            p.hsvWidget->setCallback(
                [this](const Color4F& value)
                {
                    FTK_P();
                    p.color = value;
                    if (p.callback)
                    {
                        p.callback(value);
                    }
                });
            p.hsvWidget->setPressedCallback(
                [this](const Color4F& value, bool pressed)
                {
                    FTK_P();
                    p.color = value;
                    if (p.pressedCallback)
                    {
                        p.pressedCallback(value, pressed);
                    }
                });
            p.hsvWidget->setMarginRole(SizeRole::MarginInside);
            break;
        default: break;
        }
    }

    void ColorWidget::_colorUpdate()
    {
        FTK_P();
        if (p.rgbWidget)
        {
            p.rgbWidget->setColor(p.color);
        }
        if (p.hsvWidget)
        {
            p.hsvWidget->setColor(p.color);
        }
    }
}
