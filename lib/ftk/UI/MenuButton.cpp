// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/MenuPrivate.h>

#include <ftk/UI/DrawUtil.h>

#include <ftk/Core/String.h>

#include <optional>

namespace ftk
{
    struct MenuButton::Private
    {
        bool current = false;
        std::vector<KeyShortcut> shortcuts;
        std::string shortcutText;

        float iconScale = 1.F;
        std::string subMenuIcon;
        std::shared_ptr<Image> subMenuImage;

        std::function<void(bool)> enabledCallback;

        std::shared_ptr<Observer<std::string> > textObserver;
        std::shared_ptr<Observer<std::string> > iconObserver;
        std::shared_ptr<Observer<std::string> > checkedIconObserver;
        std::shared_ptr<ListObserver<KeyShortcut> > shortcutsObserver;
        std::shared_ptr<Observer<bool> > checkableObserver;
        std::shared_ptr<Observer<bool> > checkedObserver;
        std::shared_ptr<Observer<bool> > enabledObserver;

        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
            int keyFocus = 0;
            int pad = 0;
            FontInfo fontInfo;
            FontMetrics fontMetrics;
            Size2I textSize;
            Size2I shortcutSize;
            Size2I sizeHint;
        };
        SizeData size;

        struct DrawData
        {
            Box2I g;
            Box2I g2;
            TriMesh2F keyFocus;
            std::vector<std::shared_ptr<Glyph> > textGlyphs;
            std::vector<std::shared_ptr<Glyph> > shortcutGlyphs;
        };
        std::optional<DrawData> draw;
    };

    void MenuButton::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<Action>& action,
        const std::shared_ptr<IWidget>& parent)
    {
        IButton::_init(context, "ftk::MenuButton", parent);
        FTK_P();

        setButtonRole(ColorRole::None);

        if (action)
        {
            p.textObserver = Observer<std::string>::create(
                action->observeText(),
                [this](const std::string& value)
                {
                    setText(value);
                });
            p.iconObserver = Observer<std::string>::create(
                action->observeIcon(),
                [this](const std::string& value)
                {
                    setIcon(!value.empty() ? value : "Empty");
                });
            p.checkedIconObserver = Observer<std::string>::create(
                action->observeCheckedIcon(),
                [this](const std::string& value)
                {
                    setCheckedIcon(value);
                });
            p.shortcutsObserver = ListObserver<KeyShortcut>::create(
                action->observeShortcuts(),
                [this](const std::vector<KeyShortcut>& value)
                {
                    setShortcuts(value);
                });
            p.checkableObserver = Observer<bool>::create(
                action->observeCheckable(),
                [this](bool value)
                {
                    setCheckable(value);
                    if ("Empty" == getIcon() && getCheckedIcon().empty())
                    {
                        setCheckedIcon("MenuChecked");
                    }
                });
            p.checkedObserver = Observer<bool>::create(
                action->observeChecked(),
                [this](bool value)
                {
                    setChecked(value);
                    if ("Empty" == getIcon() && getCheckedIcon().empty())
                    {
                        setCheckedIcon("MenuChecked");
                    }
                });
            p.enabledObserver = Observer<bool>::create(
                action->observeEnabled(),
                [this](bool value)
                {
                    setEnabled(value);
                });
        }
    }

    MenuButton::MenuButton() :
        _p(new Private)
    {}

    MenuButton::~MenuButton()
    {}

    std::shared_ptr<MenuButton> MenuButton::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<Action>& action,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<MenuButton>(new MenuButton);
        out->_init(context, action, parent);
        return out;
    }

    void MenuButton::setCurrent(bool value)
    {
        FTK_P();
        if (value == p.current)
            return;
        p.current = value;
        setDrawUpdate();
    }

    void MenuButton::setShortcuts(const std::vector<KeyShortcut>& value)
    {
        FTK_P();
        if (value == p.shortcuts)
            return;
        p.shortcuts = value;
        std::vector<std::string> tmp;
        for (const auto& shortcut : value)
        {
            if (shortcut.key != Key::Unknown)
            {
                tmp.push_back(getShortcutLabel(shortcut.key, shortcut.modifiers));
            }
        }
        p.shortcutText = join(tmp, ", ");
        p.size.displayScale.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    void MenuButton::setSubMenuIcon(const std::string& name)
    {
        FTK_P();
        if (name == p.subMenuIcon)
            return;
        p.subMenuIcon = name;
        p.subMenuImage.reset();
    }

    void MenuButton::setEnabledCallback(const std::function<void(bool)>& value)
    {
        _p->enabledCallback = value;
    }

    void MenuButton::setText(const std::string& value)
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

    void MenuButton::setEnabled(bool value)
    {
        const bool changed = value != isEnabled(false);
        IWidget::setEnabled(value);
        FTK_P();
        if (changed)
        {
            if (p.enabledCallback)
            {
                p.enabledCallback(value);
            }
        }
    }
    
    Size2I MenuButton::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void MenuButton::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IButton::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.draw.reset();
        }
    }

    void MenuButton::sizeHintEvent(const SizeHintEvent& event)
    {
        IButton::sizeHintEvent(event);
        FTK_P();
        bool init = false;
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            init = true;
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(SizeRole::MarginInside, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.pad = event.style->getSizeRole(SizeRole::LabelPad, event.displayScale);
            p.size.fontInfo = event.style->getFontRole(_fontRole, event.displayScale);
            p.size.fontMetrics = event.fontSystem->getMetrics(p.size.fontInfo);
            p.size.textSize = event.fontSystem->getSize(_text, p.size.fontInfo);
            p.size.shortcutSize = event.fontSystem->getSize(p.shortcutText, p.size.fontInfo);
            p.draw.reset();
        }
        if (event.displayScale != p.iconScale)
        {
            init = true;
            p.iconScale = event.displayScale;
            p.subMenuImage.reset();
        }
        if (!p.subMenuIcon.empty() && !p.subMenuImage)
        {
            init = true;
            p.subMenuImage = event.iconSystem->get(p.subMenuIcon, p.iconScale);
        }

        if (init)
        {
            p.size.sizeHint = Size2I();
            if (_iconImage)
            {
                p.size.sizeHint.w = _iconImage->getWidth();
                p.size.sizeHint.h = _iconImage->getHeight();
            }
            if (!_text.empty())
            {
                p.size.sizeHint.w += p.size.textSize.w + p.size.pad * 2;
                p.size.sizeHint.h = std::max(p.size.sizeHint.h, p.size.fontMetrics.lineHeight);
            }
            if (!p.shortcutText.empty())
            {
                p.size.sizeHint.w += p.size.shortcutSize.w + p.size.pad * 4;
                p.size.sizeHint.h = std::max(p.size.sizeHint.h, p.size.shortcutSize.h);
            }
            if (p.subMenuImage)
            {
                p.size.sizeHint.w += p.subMenuImage->getWidth();
                p.size.sizeHint.h = std::max(p.size.sizeHint.h, p.subMenuImage->getHeight());
            }
            p.size.sizeHint = margin(p.size.sizeHint, p.size.margin + p.size.keyFocus);
        }
    }

    void MenuButton::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void MenuButton::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IButton::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->g = getGeometry();
            p.draw->g2 = margin(p.draw->g, -(p.size.margin + p.size.keyFocus));
            p.draw->keyFocus = border(p.draw->g, p.size.keyFocus);
        }

        // Draw the background.
        if (_buttonRole != ColorRole::None)
        {
            event.render->drawRect(
                p.draw->g,
                event.style->getColorRole(_buttonRole));
        }

        // Draw the current state.
        if (p.current)
        {
            event.render->drawMesh(
                p.draw->keyFocus,
                event.style->getColorRole(ColorRole::KeyFocus));
        }

        // Draw the mouse state.
        if (_isMousePressed())
        {
            event.render->drawRect(
                p.draw->g,
                event.style->getColorRole(ColorRole::Pressed));
        }
        else if (_isMouseInside())
        {
            event.render->drawRect(
                p.draw->g,
                event.style->getColorRole(ColorRole::Hover));
        }

        // Draw the icon.
        int x = p.draw->g2.x();
        if (auto image = _checked && _checkedIconImage ? _checkedIconImage : _iconImage)
        {
            const Size2I& iconSize = _iconImage->getSize();
            const Box2I iconRect(
                x,
                p.draw->g2.y() + p.draw->g2.h() / 2 - iconSize.h / 2,
                iconSize.w,
                iconSize.h);
            if (_checked)
            {
                event.render->drawRect(
                    iconRect,
                    event.style->getColorRole(ColorRole::Checked));
            }
            event.render->drawImage(
                image,
                iconRect,
                event.style->getColorRole(isEnabled() ?
                    ColorRole::Text :
                    ColorRole::TextDisabled));
            x += iconSize.w;
        }

        // Draw the text.
        if (!_text.empty())
        {
            if (p.draw->textGlyphs.empty())
            {
                p.draw->textGlyphs = event.fontSystem->getGlyphs(_text, p.size.fontInfo);
            }
            event.render->drawText(
                p.draw->textGlyphs,
                p.size.fontMetrics,
                V2I(x + p.size.pad,
                    p.draw->g2.y() + p.draw->g2.h() / 2 - p.size.textSize.h / 2),
                event.style->getColorRole(isEnabled() ?
                    ColorRole::Text :
                    ColorRole::TextDisabled));
        }

        // Draw the shortcut.
        if (!p.shortcutText.empty())
        {
            if (p.draw->shortcutGlyphs.empty())
            {
                p.draw->shortcutGlyphs = event.fontSystem->getGlyphs(p.shortcutText, p.size.fontInfo);
            }
            const V2I pos(
                p.draw->g2.max.x - p.size.shortcutSize.w - p.size.pad,
                p.draw->g2.y() + p.draw->g2.h() / 2 - p.size.shortcutSize.h / 2);
            event.render->drawText(
                p.draw->shortcutGlyphs,
                p.size.fontMetrics,
                pos,
                event.style->getColorRole(isEnabled() ?
                    ColorRole::Text :
                    ColorRole::TextDisabled));
        }

        // Draw the sub menu icon.
        if (p.subMenuImage)
        {
            const Size2I& iconSize = p.subMenuImage->getSize();
            event.render->drawImage(
                p.subMenuImage,
                Box2I(
                    p.draw->g2.max.x - iconSize.w,
                    p.draw->g2.y() + p.draw->g2.h() / 2 - iconSize.h / 2,
                    iconSize.w,
                    iconSize.h),
                event.style->getColorRole(isEnabled() ?
                    ColorRole::Text :
                    ColorRole::TextDisabled));
        }
    }
}
