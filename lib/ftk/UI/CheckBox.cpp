// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/CheckBox.h>

#include <ftk/UI/DrawUtil.h>

#include <optional>

namespace ftk
{
    struct CheckBox::Private
    {
        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
            int spacing = 0;
            int border = 0;
            int keyFocus = 0;
            int pad = 0;
            FontInfo fontInfo;
            FontMetrics fontMetrics;
            Size2I textSize;
            int checkBox = 0;
            Size2I sizeHint;
            Box2I g;
            Box2I g2;
            Box2I g3;
        };
        SizeData size;

        struct DrawData
        {
            TriMesh2F focus;
            TriMesh2F checkBox;
            std::vector<std::shared_ptr<Glyph> > glyphs;
        };
        std::optional<DrawData> draw;
    };

    void CheckBox::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IButton::_init(context, "ftk::CheckBox", parent);
        setCheckable(true);
        setAcceptsKeyFocus(true);
        _buttonRole = ColorRole::None;
    }

    CheckBox::CheckBox() :
        _p(new Private)
    {}

    CheckBox::~CheckBox()
    {}

    std::shared_ptr<CheckBox> CheckBox::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<CheckBox>(new CheckBox);
        out->_init(context, parent);
        return out;
    }

    std::shared_ptr<CheckBox> CheckBox::create(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = create(context, parent);
        out->setText(text);
        return out;
    }

    void CheckBox::setText(const std::string& value)
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

    void CheckBox::setFontRole(FontRole value)
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
    
    Size2I CheckBox::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void CheckBox::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IButton::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.size.g = value;
            p.size.g2 = margin(p.size.g, -(p.size.margin + p.size.keyFocus));
            p.size.g3 = Box2I(
                p.size.g2.x(),
                p.size.g2.y() + p.size.g2.h() / 2 - p.size.checkBox / 2,
                p.size.checkBox,
                p.size.checkBox);
            p.draw.reset();
        }
    }

    void CheckBox::sizeHintEvent(const SizeHintEvent& event)
    {
        IButton::sizeHintEvent(event);
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(SizeRole::MarginInside, event.displayScale);
            p.size.spacing = event.style->getSizeRole(SizeRole::SpacingSmall, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.pad = event.style->getSizeRole(SizeRole::LabelPad, event.displayScale);
            p.size.fontInfo = event.style->getFontRole(_fontRole, event.displayScale);
            p.size.fontMetrics = event.fontSystem->getMetrics(p.size.fontInfo);
            p.size.textSize = event.fontSystem->getSize(_text, p.size.fontInfo);
            p.size.checkBox = p.size.fontMetrics.lineHeight * .8F;

            p.size.sizeHint = Size2I();
            p.size.sizeHint.w += p.size.checkBox;
            p.size.sizeHint.w += p.size.spacing;
            p.size.sizeHint.w += p.size.textSize.w + p.size.pad * 2;
            p.size.sizeHint.h = p.size.fontMetrics.lineHeight;
            p.size.sizeHint = margin(p.size.sizeHint, p.size.margin + p.size.keyFocus);

            p.draw.reset();
        }
    }

    void CheckBox::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IButton::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void CheckBox::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IButton::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->focus = border(p.size.g, p.size.keyFocus);
            p.draw->checkBox = border(p.size.g3, p.size.border);
        }

        // Draw the focus.
        if (hasKeyFocus())
        {
            event.render->drawMesh(
                p.draw->focus,
                event.style->getColorRole(ColorRole::KeyFocus));
        }

        // Draw the mouse states.
        if (_isMousePressed())
        {
            event.render->drawRect(
                p.size.g,
                event.style->getColorRole(ColorRole::Pressed));
        }
        else if (_isMouseInside())
        {
            event.render->drawRect(
                p.size.g,
                event.style->getColorRole(ColorRole::Hover));
        }

        // Draw the check box.
        event.render->drawMesh(
            p.draw->checkBox,
            event.style->getColorRole(ColorRole::Border));
        event.render->drawRect(
            margin(p.size.g3, -p.size.border),
            event.style->getColorRole(_checked ? ColorRole::Checked : ColorRole::Base));

        // Draw the text.
        if (!_text.empty() && p.draw->glyphs.empty())
        {
            p.draw->glyphs = event.fontSystem->getGlyphs(_text, p.size.fontInfo);
        }
        event.render->drawText(
            p.draw->glyphs,
            p.size.fontMetrics,
            V2I(p.size.g2.x() + p.size.checkBox + p.size.spacing + p.size.pad,
                p.size.g2.y() + p.size.g2.h() / 2 - p.size.textSize.h / 2),
            event.style->getColorRole(isEnabled() ?
                ColorRole::Text :
                ColorRole::TextDisabled));
    }

    void CheckBox::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
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

    void CheckBox::keyReleaseEvent(KeyEvent& event)
    {
        IButton::keyReleaseEvent(event);
        event.accept = true;
    }
}