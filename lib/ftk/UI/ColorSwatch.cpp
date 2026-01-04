// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ColorSwatch.h>

#include <ftk/UI/ColorPopup.h>
#include <ftk/UI/DrawUtil.h>

#include <optional>

namespace ftk
{
    struct ColorSwatch::Private
    {
        Color4F color;
        bool editable = false;
        std::function<void(void)> pressedCallback;
        std::function<void(const Color4F&)> colorCallback;
        SizeRole sizeRole = SizeRole::Swatch;
        std::shared_ptr<ColorPopup> popup;

        struct SizeData
        {
            std::optional<float> displayScale;
            int border = 0;
            Size2I sizeHint;
        };
        SizeData size;

        struct DrawData
        {
            TriMesh2F border;
            Box2I g2;
        };
        std::optional<DrawData> draw;
    };

    void ColorSwatch::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::ColorSwatch", parent);
    }

    ColorSwatch::ColorSwatch() :
        _p(new Private)
    {}

    ColorSwatch::~ColorSwatch()
    {}

    std::shared_ptr<ColorSwatch> ColorSwatch::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ColorSwatch>(new ColorSwatch);
        out->_init(context, parent);
        return out;
    }

    const Color4F& ColorSwatch::getColor() const
    {
        return _p->color;
    }

    void ColorSwatch::setColor(const Color4F& value)
    {
        FTK_P();
        if (value == p.color)
            return;
        p.color = value;
        setDrawUpdate();
    }

    void ColorSwatch::setPressedCallback(const std::function<void(void)>& value)
    {
        _p->pressedCallback = value;
    }

    bool ColorSwatch::isEditable() const
    {
        return _p->editable;
    }

    void ColorSwatch::setEditable(bool value)
    {
        FTK_P();
        if (value == p.editable)
            return;
        p.editable = value;
        _setMouseHoverEnabled(p.editable);
        _setMousePressEnabled(p.editable);
    }

    void ColorSwatch::setColorCallback(const std::function<void(const Color4F&)>& value)
    {
        _p->colorCallback = value;
    }

    SizeRole ColorSwatch::getSizeRole() const
    {
        return _p->sizeRole;
    }

    void ColorSwatch::setSizeRole(SizeRole value)
    {
        FTK_P();
        if (value == p.sizeRole)
            return;
        p.sizeRole = value;
        p.size.displayScale.reset();
        setSizeUpdate();
        setDrawUpdate();
    }
    
    Size2I ColorSwatch::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void ColorSwatch::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IMouseWidget::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.draw.reset();
        }
    }

    void ColorSwatch::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);

            p.size.sizeHint.w = p.size.sizeHint.h =
                event.style->getSizeRole(p.sizeRole, event.displayScale);

            p.draw.reset();
        }
    }

    void ColorSwatch::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IMouseWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void ColorSwatch::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IMouseWidget::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            const Box2I& g = getGeometry();
            p.draw->border = border(g, p.size.border);
            p.draw->g2 = margin(g, -p.size.border);
        }

        event.render->drawMesh(
            p.draw->border,
            event.style->getColorRole(ColorRole::Border));
        event.render->drawRect(p.draw->g2, Color4F(0.F, 0.F, 0.F));
        event.render->drawRect(p.draw->g2, p.color);
    }

    void ColorSwatch::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        if (p.editable)
        {
            _showPopup();
        }
        else if (p.pressedCallback)
        {
            p.pressedCallback();
        }
    }

    void ColorSwatch::_showPopup()
    {
        FTK_P();
        if (auto context = getContext())
        {
            if (!p.popup)
            {
                p.popup = ColorPopup::create(context, p.color);
                p.popup->open(getWindow(), getGeometry());
                p.popup->setCallback(
                    [this](const Color4F& value)
                    {
                        _p->color = value;
                        setDrawUpdate();
                        if (_p->colorCallback)
                        {
                            _p->colorCallback(value);
                        }
                    });
                p.popup->setCloseCallback(
                    [this]
                    {
                        _p->popup.reset();
                    });
            }
            else
            {
                p.popup->close();
                p.popup.reset();
            }
        }
    }
}