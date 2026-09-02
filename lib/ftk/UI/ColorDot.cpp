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
            int hit = 0;
            int pad = 0;
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
        // An editable dot is a button that opens the picker, so it takes
        // the key focus the way a button does. A caller whose focus unit
        // is a containing row opts out with setAcceptsKeyFocus(false).
        setAcceptsKeyFocus(value);
    }

    void ColorDot::setCallback(const std::function<void(const Color4F&)>& value)
    {
        _p->callback = value;
    }

    Size2I ColorDot::getSizeHint() const
    {
        FTK_P();
        // The hit box is bigger than the dot: the subtlety is visual, and
        // must not shrink the click target with it. The dot sits in text
        // rows, so it wears the label's pad and lines up with the text
        // rather than sitting closer to the edge.
        return Size2I(p.size.diameter + p.size.pad * 2, p.size.hit);
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
            const int lineHeight =
                event.fontSystem->getMetrics(fontInfo).lineHeight;
            p.size.diameter = lineHeight * .8F;
            p.size.hit = lineHeight;
            p.size.pad = event.style->getSizeRole(
                SizeRole::LabelPad, event.displayScale);
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
        if (showKeyFocus())
        {
            event.render->drawMesh(
                border(g, p.size.border * 2),
                event.style->getColorRole(ColorRole::KeyFocus));
        }
    }

    void ColorDot::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        if (p.editable)
        {
            takeKeyFocus();
            _showPopup();
        }
    }

    void ColorDot::keyPressEvent(KeyEvent& event)
    {
        IMouseWidget::keyPressEvent(event);
        FTK_P();
        if (!event.accept && p.editable && 0 == event.modifiers)
        {
            switch (event.key)
            {
            case Key::Return:
                event.accept = true;
                _showPopup();
                break;
            case Key::Escape:
                if (hasKeyFocus())
                {
                    event.accept = true;
                    releaseKeyFocus();
                }
                break;
            default: break;
            }
        }
    }

    void ColorDot::keyReleaseEvent(KeyEvent& event)
    {
        IMouseWidget::keyReleaseEvent(event);
        event.accept = true;
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
