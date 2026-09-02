// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ColorDot.h>

#include <ftk/UI/ColorPopup.h>
#include <ftk/UI/DrawUtil.h>

namespace ftk
{
    struct ColorDot::Private
    {
        Color4F color;
        bool editable = false;
        std::function<void(const Color4F&)> callback;
        std::shared_ptr<ColorPopup> popup;

        struct SizeData
        {
            bool init = true;
            int diameter = 0;
            int border = 0;
        };
        SizeData size;
    };

    void ColorDot::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::ColorDot", parent);
    }

    ColorDot::ColorDot() :
        _p(new Private)
    {}

    ColorDot::~ColorDot()
    {}

    std::shared_ptr<ColorDot> ColorDot::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ColorDot>(new ColorDot);
        out->_init(context, parent);
        return out;
    }

    const Color4F& ColorDot::getColor() const
    {
        return _p->color;
    }

    void ColorDot::setColor(const Color4F& value)
    {
        FTK_P();
        if (value == p.color)
            return;
        p.color = value;
        setDrawUpdate();
    }

    bool ColorDot::isEditable() const
    {
        return _p->editable;
    }

    void ColorDot::setEditable(bool value)
    {
        FTK_P();
        if (value == p.editable)
            return;
        p.editable = value;
        _setMousePressEnabled(value);
    }

    void ColorDot::setCallback(const std::function<void(const Color4F&)>& value)
    {
        _p->callback = value;
    }

    Size2I ColorDot::getSizeHint() const
    {
        FTK_P();
        return Size2I(p.size.diameter, p.size.diameter);
    }

    void ColorDot::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            // The radio button's dot size: small enough to sit in a text
            // row without setting its height.
            const FontInfo fontInfo = event.style->getFont(
                FontType::Regular, event.displayScale);
            p.size.diameter =
                event.fontSystem->getMetrics(fontInfo).lineHeight * .8F;
            p.size.border = event.style->getSizeRole(
                SizeRole::Border, event.displayScale);
        }
    }

    void ColorDot::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IMouseWidget::drawEvent(drawRect, event);
        FTK_P();
        const Box2I& g = getGeometry();
        // A border ring first, so a color near the row's own reads as a
        // dot rather than vanishing.
        event.render->drawMesh(
            circle(center(g), p.size.diameter / 2),
            event.style->getColorRole(ColorRole::Border));
        event.render->drawMesh(
            circle(center(g), p.size.diameter / 2 - p.size.border),
            p.color);
    }

    void ColorDot::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        if (p.editable)
        {
            _showPopup();
        }
    }

    void ColorDot::_showPopup()
    {
        FTK_P();
        if (!p.popup)
        {
            auto context = getContext();
            p.popup = ColorPopup::create(context, p.color);
            p.popup->open(getWindow(), getGeometry());
            // The popup can outlive the dot that opened it, the same as the
            // color swatch: the captures must not assume the dot is still
            // there.
            auto weak = std::weak_ptr<ColorDot>(
                std::dynamic_pointer_cast<ColorDot>(shared_from_this()));
            p.popup->setPressedCallback(
                [weak](const Color4F& value, bool)
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->color = value;
                        widget->setDrawUpdate();
                        if (widget->_p->callback)
                        {
                            widget->_p->callback(value);
                        }
                    }
                });
            p.popup->setCloseCallback(
                [weak]
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->popup.reset();
                    }
                });
        }
        else
        {
            p.popup->close();
        }
    }
}
