// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ToolButton.h>

#include <ftk/UI/Action.h>
#include <ftk/UI/DrawUtil.h>

#include <optional>

namespace ftk
{
    struct ToolButton::Private
    {
        std::shared_ptr<Action> action;
        std::shared_ptr<Observer<std::string> > iconObserver;
        std::shared_ptr<Observer<std::string> > checkedIconObserver;
        std::shared_ptr<Observer<bool> > checkableObserver;
        std::shared_ptr<Observer<bool> > checkedObserver;
        std::shared_ptr<Observer<bool> > enabledObserver;
        std::shared_ptr<Observer<std::string> > tooltipObserver;

        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
            int keyFocus = 0;
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
            TriMesh2F mesh;
            TriMesh2F keyFocus;
            std::vector<std::shared_ptr<Glyph> > glyphs;
        };
        std::optional<DrawData> draw;
    };

    void ToolButton::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<Action>& action,
        const std::shared_ptr<IWidget>& parent)
    {
        IButton::_init(context, "ftk::ToolButton", parent);
        FTK_P();

        setAcceptsKeyFocus(true);

        _buttonRole = ColorRole::None;

        p.action = action;

        if (action)
        {
            setClickedCallback(
                [this]
                {
                    _p->action->doCallback();
                });

            setCheckedCallback(
                [this](bool value)
                {
                    _p->action->doCheckedCallback(value);
                });

            p.iconObserver = Observer<std::string>::create(
                action->observeIcon(),
                [this](const std::string& value)
                {
                    setIcon(value);
                });
            p.checkedIconObserver = Observer<std::string>::create(
                action->observeCheckedIcon(),
                [this](const std::string& value)
                {
                    setCheckedIcon(value);
                });
            p.checkableObserver = Observer<bool>::create(
                action->observeCheckable(),
                [this](bool value)
                {
                    setCheckable(value);
                });
            p.checkedObserver = Observer<bool>::create(
                action->observeChecked(),
                [this](bool value)
                {
                    setChecked(value);
                });
            p.enabledObserver = Observer<bool>::create(
                action->observeEnabled(),
                [this](bool value)
                {
                    setEnabled(value);
                });
            p.tooltipObserver = Observer<std::string>::create(
                action->observeTooltip(),
                [this](const std::string& value)
                {
                    setTooltip(value);
                });
        }
    }

    ToolButton::ToolButton() :
        _p(new Private)
    {}

    ToolButton::~ToolButton()
    {}

    std::shared_ptr<ToolButton> ToolButton::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ToolButton>(new ToolButton);
        out->_init(context, nullptr, parent);
        return out;
    }

    std::shared_ptr<ToolButton> ToolButton::create(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ToolButton>(new ToolButton);
        out->_init(context, nullptr, parent);
        out->setText(text);
        return out;
    }

    std::shared_ptr<ToolButton> ToolButton::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<Action>& action,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ToolButton>(new ToolButton);
        out->_init(context, action, parent);
        return out;
    }

    void ToolButton::setText(const std::string& value)
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

    void ToolButton::setFontRole(FontRole value)
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

    void ToolButton::setAcceptsKeyFocus(bool value)
    {
        const bool changed = value != acceptsKeyFocus();
        IButton::setAcceptsKeyFocus(value);
        FTK_P();
        if (changed)
        {
            p.draw.reset();
            setSizeUpdate();
            setDrawUpdate();
        }
    }
    
    Size2I ToolButton::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void ToolButton::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IButton::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.draw.reset();
        }
    }

    void ToolButton::sizeHintEvent(const SizeHintEvent& event)
    {
        IButton::sizeHintEvent(event);
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(SizeRole::MarginInside, event.displayScale);
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
                if (_icon.empty())
                {
                    const int max = std::max(p.size.sizeHint.w, p.size.sizeHint.h);
                    p.size.sizeHint.w = max;
                    p.size.sizeHint.h = p.size.sizeHint.h;
                }
            }
            if (_iconImage)
            {
                p.size.sizeHint.w += _iconImage->getWidth();
                p.size.sizeHint.h = std::max(p.size.sizeHint.h, _iconImage->getHeight());
            }
            p.size.sizeHint = margin(p.size.sizeHint, p.size.margin);
            if (acceptsKeyFocus())
            {
                p.size.sizeHint = margin(p.size.sizeHint, p.size.keyFocus);
            }

            p.draw.reset();
        }
    }

    void ToolButton::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IButton::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void ToolButton::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IButton::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->g = getGeometry();
            p.draw->g2 = margin(p.draw->g, -p.size.margin);
            if (acceptsKeyFocus())
            {
                p.draw->g2 = margin(p.draw->g2, -p.size.keyFocus);
            }
            p.draw->mesh = rect(p.draw->g);
            p.draw->keyFocus = border(p.draw->g, p.size.keyFocus);
        }

        // Draw the background.
        const ColorRole colorRole = _checked ? _checkedRole : _buttonRole;
        if (colorRole != ColorRole::None)
        {
            event.render->drawMesh(
                p.draw->mesh,
                event.style->getColorRole(colorRole));
        }

        // Draw the focus.
        if (hasKeyFocus())
        {
            event.render->drawMesh(
                p.draw->keyFocus,
                event.style->getColorRole(ColorRole::KeyFocus));
        }

        // Draw the mouse states.
        if (_isMousePressed())
        {
            event.render->drawMesh(
                p.draw->mesh,
                event.style->getColorRole(ColorRole::Pressed));
        }
        else if (_isMouseInside())
        {
            event.render->drawMesh(
                p.draw->mesh,
                event.style->getColorRole(ColorRole::Hover));
        }

        // Draw the icon.
        int x = p.draw->g2.x();
        auto iconImage = _iconImage;
        if (_checked && _checkedIconImage)
        {
            iconImage = _checkedIconImage;
        }
        if (iconImage)
        {
            const Size2I& iconSize = iconImage->getSize();
            if (_text.empty())
            {
                x = p.draw->g2.x() + p.draw->g2.w() / 2 - iconSize.w / 2;
            }
            event.render->drawImage(
                iconImage,
                Box2I(
                    x,
                    p.draw->g2.y() + p.draw->g2.h() / 2 - iconSize.h / 2,
                    iconSize.w,
                    iconSize.h),
                event.style->getColorRole(isEnabled() ?
                    ColorRole::Text :
                    ColorRole::TextDisabled));
            x += iconSize.w;
        }
            
        // Draw the text.
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
                    p.draw->g2.y() + p.draw->g2.h() / 2 - p.size.textSize.h / 2),
                event.style->getColorRole(isEnabled() ?
                    ColorRole::Text :
                    ColorRole::TextDisabled));
        }
    }

    void ToolButton::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        if (0 == event.modifiers)
        {
            switch (event.key)
            {
            case Key::Return:
                event.accept = true;
                if (_pressedCallback)
                {
                    _pressedCallback();
                }
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

    void ToolButton::keyReleaseEvent(KeyEvent& event)
    {
        IButton::keyReleaseEvent(event);
        event.accept = true;
    }
}
