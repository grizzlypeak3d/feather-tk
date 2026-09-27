// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/MenuBarPrivate.h>

#include <ftk/UI/DrawUtil.h>

#include <optional>

namespace ftk
{
    struct MenuBarButton::Private
    {
        bool current = false;

        struct SizeData
        {
            bool init = true;
            int margin = 0;
            int keyFocus = 0;
            int cornerRadius = 0;
            int pad = 0;
            FontInfo fontInfo;
            FontMetrics fontMetrics;
            Size2I textSize;
            Size2I sizeHint;
        };
        SizeData size;

        struct DrawData
        {
            Box2I g;
            Box2I g2;
            TriMesh2F bgMesh;
            TriMesh2F keyFocus;
            std::vector<std::shared_ptr<Glyph> > glyphs;
        };
        std::optional<DrawData> draw;
    };

    void MenuBarButton::_init(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::MenuBarButton", parent);
        FTK_P();
        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);
        setText(text);
        setButtonRole(ColorRole::None);
    }

    MenuBarButton::MenuBarButton() :
        _p(new Private)
    {}

    MenuBarButton::~MenuBarButton()
    {}

    std::shared_ptr<MenuBarButton> MenuBarButton::create(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<MenuBarButton>(new MenuBarButton);
        out->_init(context, text, parent);
        return out;
    }

    void MenuBarButton::setCurrent(bool value)
    {
        FTK_P();
        if (p.current == value)
            return;
        p.current = value;
        setDrawUpdate();
    }
    
    Size2I MenuBarButton::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void MenuBarButton::setGeometry(const Box2I& value)
    {
        if (value != getGeometry())
        {
            _p->draw.reset();
        }
        IButton::setGeometry(value);
    }

    void MenuBarButton::styleEvent(const StyleEvent& event)
    {
        IButton::styleEvent(event);
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
            p.draw.reset();
        }
    }

    void MenuBarButton::_sizeDirty()
    {
        _p->size.init = true;
    }

    void MenuBarButton::sizeHintEvent(const SizeHintEvent& event)
    {
        IButton::sizeHintEvent(event);
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.margin = event.style->getSizeRole(SizeRole::MarginInside, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.cornerRadius = event.style->getSizeRole(SizeRole::CornerRadius, event.displayScale);
            p.size.pad = event.style->getSizeRole(SizeRole::LabelPad, event.displayScale);
            p.size.fontInfo = event.style->getFont(FontType::Regular, event.displayScale);
            p.size.fontMetrics = event.fontSystem->getMetrics(p.size.fontInfo);
            p.size.textSize = event.fontSystem->getSize(_text, p.size.fontInfo);

            p.size.sizeHint = Size2I(
                p.size.textSize.w + p.size.pad * 2,
                p.size.fontMetrics.lineHeight);
            p.size.sizeHint = margin(p.size.sizeHint, p.size.margin + p.size.keyFocus);

            p.draw.reset();
        }
    }

    void MenuBarButton::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IButton::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void MenuBarButton::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        IButton::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->g = getGeometry();
            p.draw->g2 = margin(p.draw->g, -(p.size.margin + p.size.keyFocus));
            // Rounded and inset from the edges of the bar, like the items of
            // the menu it opens.
            const Box2I g3 = margin(p.draw->g, -p.size.margin);
            p.draw->bgMesh = rect(g3, p.size.cornerRadius);
            p.draw->keyFocus = border(g3, p.size.keyFocus, p.size.cornerRadius);
        }

        // Draw the background.
        if (_buttonRole != ColorRole::None)
        {
            event.render->drawMesh(
                p.draw->bgMesh,
                event.style->getColorRole(_buttonRole));
        }
        const bool checkedTint = _checked && _checkedRole != ColorRole::None;
        if (checkedTint)
        {
            event.render->drawMesh(p.draw->bgMesh, _getCheckedTint(event));
        }

        // Draw the mouse state.
        if (_isMousePressed())
        {
            event.render->drawMesh(
                p.draw->bgMesh,
                event.style->getColorRole(ColorRole::Pressed));
        }
        else if (_isMouseInside())
        {
            event.render->drawMesh(
                p.draw->bgMesh,
                event.style->getColorRole(ColorRole::Hover));
        }

        // Draw the current state.
        if (p.current)
        {
            event.render->drawMesh(
                p.draw->keyFocus,
                event.style->getColorRole(ColorRole::KeyFocus));
        }

        // Draw the text.
        if (!_text.empty() && p.draw->glyphs.empty())
        {
            p.draw->glyphs = event.fontSystem->getGlyphs(_text, p.size.fontInfo);
        }
        event.render->drawText(
            p.draw->glyphs,
            p.size.fontMetrics,
            V2I(p.draw->g2.x() + p.size.pad, p.draw->g2.y()),
            checkedTint ?
            _getCheckedColor(event) :
            event.style->getColorRole(_textRole));
    }
}