// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/PushButton.h>

#include <ftk/UI/DrawUtil.h>

#include <optional>

namespace ftk
{
    struct PushButton::Private
    {
        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
            int border = 0;
            int keyFocus = 0;
            int pad = 0;
            FontInfo fontInfo;
            FontMetrics fontMetrics;
            Size2I textSize;
            Size2I sizeHint;
            Box2I g;
            Box2I g2;
        };
        SizeData size;

        struct DrawData
        {
            TriMesh2F background;
            TriMesh2F border;
            TriMesh2F keyFocus;
            std::vector<std::shared_ptr<Glyph> > glyphs;
        };
        std::optional<DrawData> draw;
    };

    void PushButton::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IButton::_init(context, "ftk::PushButton", parent);
        setAcceptsKeyFocus(true);
    }

    PushButton::PushButton() :
        _p(new Private)
    {}

    PushButton::~PushButton()
    {}

    std::shared_ptr<PushButton> PushButton::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<PushButton>(new PushButton);
        out->_init(context, parent);
        return out;
    }

    std::shared_ptr<PushButton> PushButton::create(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = create(context, parent);
        out->setText(text);
        return out;
    }

    void PushButton::setText(const std::string& value)
    {
        const bool changed = value != _text;
        IButton::setText(value);
        FTK_P();
        if (changed)
        {
            p.size.displayScale.reset();
            setSizeUpdate();
            setDrawUpdate();
        }
    }

    void PushButton::setFontRole(FontRole value)
    {
        const bool changed = value != _fontRole;
        IButton::setFontRole(value);
        FTK_P();
        if (changed)
        {
            p.size.displayScale.reset();
            setSizeUpdate();
            setDrawUpdate();
        }
    }

    Size2I PushButton::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void PushButton::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IButton::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.size.g = value;
            p.size.g2 = margin(
                p.size.g,
                -(p.size.margin + p.size.pad + p.size.keyFocus),
                -(p.size.margin + p.size.keyFocus),
                -(p.size.margin + p.size.pad + p.size.keyFocus),
                -(p.size.margin + p.size.keyFocus));
            p.draw.reset();
        }
    }

    void PushButton::sizeHintEvent(const SizeHintEvent& event)
    {
        IButton::sizeHintEvent(event);
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(SizeRole::MarginInside, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.pad = event.style->getSizeRole(SizeRole::LabelPad, event.displayScale);
            p.size.fontInfo = event.style->getFontRole(_fontRole, event.displayScale);
            p.size.fontMetrics = event.fontSystem->getMetrics(p.size.fontInfo);
            p.size.textSize = event.fontSystem->getSize(_text, p.size.fontInfo);

            p.size.sizeHint = Size2I();
            if (!_text.empty())
            {
                p.size.sizeHint.w = p.size.textSize.w + p.size.pad * 2;
                p.size.sizeHint.h = p.size.fontMetrics.lineHeight;
            }
            if (_iconImage)
            {
                p.size.sizeHint.w += _iconImage->getWidth();
                p.size.sizeHint.h = std::max(p.size.sizeHint.h, _iconImage->getHeight());
            }
            p.size.sizeHint = margin(
                p.size.sizeHint,
                p.size.margin + p.size.pad + p.size.keyFocus,
                p.size.margin + p.size.keyFocus);

            p.draw.reset();
        }
    }

    void PushButton::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void PushButton::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IButton::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->background = rect(p.size.g);
            p.draw->border = border(p.size.g, p.size.border);
            p.draw->keyFocus = border(p.size.g, p.size.keyFocus);
        }

        // Draw the background.
        const ColorRole colorRole = _checked ? _checkedRole : _buttonRole;
        if (colorRole != ColorRole::None)
        {
            event.render->drawMesh(
                p.draw->background,
                event.style->getColorRole(colorRole));
        }

        // Draw the focus and border.
        const bool keyFocus = hasKeyFocus();
        event.render->drawMesh(
            keyFocus ? p.draw->keyFocus : p.draw->border,
            event.style->getColorRole(keyFocus ? ColorRole::KeyFocus : ColorRole::Border));

        // Draw the mouse states.
        if (_isMousePressed())
        {
            event.render->drawMesh(
                p.draw->background,
                event.style->getColorRole(ColorRole::Pressed));
        }
        else if (_isMouseInside())
        {
            event.render->drawMesh(
                p.draw->background,
                event.style->getColorRole(ColorRole::Hover));
        }

        // Draw the icon and text.
        int x = p.size.g2.x();
        if (_iconImage && !_text.empty())
        {
            const Size2I& iconSize = _iconImage->getSize();
            x += p.size.g2.w() / 2 - (iconSize.w + p.size.textSize.w + p.size.pad * 2) / 2;
        }
        else if (_iconImage)
        {
            const Size2I& iconSize = _iconImage->getSize();
            x += p.size.g2.w() / 2 - iconSize.w / 2;
        }
        else if (!_text.empty())
        {
            x += p.size.g2.w() / 2 - (p.size.textSize.w + p.size.pad * 2) / 2;
        }
        if (_iconImage)
        {
            const Size2I& iconSize = _iconImage->getSize();
            event.render->drawImage(
                _iconImage,
                Box2I(
                    x,
                    p.size.g2.y() + p.size.g2.h() / 2 - iconSize.h / 2,
                    iconSize.w,
                    iconSize.h),
                event.style->getColorRole(isEnabled() ?
                    _textRole :
                    ColorRole::TextDisabled));
            x += iconSize.w;
        }
        if (!_text.empty())
        {
            if (p.draw->glyphs.empty())
            {
                p.draw->glyphs = event.fontSystem->getGlyphs(_text, p.size.fontInfo);
            }
            event.render->drawText(
                p.draw->glyphs,
                p.size.fontMetrics,
                V2I(x + p.size.pad,
                    p.size.g2.y() + p.size.g2.h() / 2 - p.size.textSize.h / 2),
                event.style->getColorRole(isEnabled() ?
                    _textRole :
                    ColorRole::TextDisabled));
        }
    }

    void PushButton::keyPressEvent(KeyEvent& event)
    {
        if (0 == event.modifiers)
        {
            switch (event.key)
            {
            case Key::Return:
                event.accept = true;
                click();
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
        if (!event.accept)
        {
            IButton::keyPressEvent(event);
        }
    }

    void PushButton::keyReleaseEvent(KeyEvent& event)
    {
        IButton::keyReleaseEvent(event);
        event.accept = true;
    }
}