// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Label.h>

#include <ftk/UI/LayoutUtil.h>

#include <ftk/Core/String.h>

#include <algorithm>
#include <optional>

namespace ftk
{
    namespace
    {
        const std::string ellipsis = "...";

        //! The longest elision of the text that fits the width: the most
        //! characters kept, from whichever end the mode says, around an
        //! ellipsis. Measured in the font, since characters are not all as
        //! wide, and cut on code point boundaries so none is split.
        std::string elideToWidth(
            const std::shared_ptr<FontSystem>& fontSystem,
            const std::string& text,
            const FontInfo& fontInfo,
            int width,
            ElideMode mode)
        {
            std::vector<size_t> bounds;
            for (size_t i = 0; i < text.size();)
            {
                bounds.push_back(i);
                const size_t next = utf8Next(text, i);
                if (next <= i)
                {
                    break;
                }
                i = next;
            }
            bounds.push_back(text.size());
            const size_t count = bounds.size() - 1;

            const auto candidate = [&](size_t n)
            {
                switch (mode)
                {
                case ElideMode::Left:
                    return ellipsis + text.substr(bounds[count - n]);
                case ElideMode::Middle:
                {
                    // The odd character, if there is one, goes to the
                    // beginning, as ftk::elide() does.
                    const size_t head = n - n / 2;
                    const size_t tail = n / 2;
                    return text.substr(0, bounds[head]) + ellipsis +
                        text.substr(bounds[count - tail]);
                }
                case ElideMode::Right:
                default:
                    return text.substr(0, bounds[n]) + ellipsis;
                }
            };

            // Widths only grow with more characters kept, so the most that
            // fit can be searched for rather than tried one at a time.
            size_t lo = 0;
            size_t hi = count > 0 ? count - 1 : 0;
            std::string out = ellipsis;
            while (lo <= hi)
            {
                const size_t mid = lo + (hi - lo) / 2;
                std::string s = candidate(mid);
                if (fontSystem->getSize(s, fontInfo).w <= width)
                {
                    out = std::move(s);
                    lo = mid + 1;
                }
                else
                {
                    if (0 == mid)
                    {
                        break;
                    }
                    hi = mid - 1;
                }
            }
            return out;
        }
    }

    struct Label::Private
    {
        std::string text;
        ColorRole textRole = ColorRole::Text;
        SizeRole hMarginRole = SizeRole::None;
        SizeRole vMarginRole = SizeRole::None;
        FontType font = FontType::Regular;
        int fontSize = FontInfo().size;
        bool clipText = false;
        bool elide = false;
        ElideMode elideMode = ElideMode::Right;

        struct SizeData
        {
            bool init = true;
            int hMargin = 0;
            int vMargin = 0;
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
            std::vector<std::shared_ptr<Glyph> > glyphs;
        };
        std::optional<DrawData> draw;
    };

    void Label::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::Label", parent);
        setHAlign(HAlign::Left);
        setVAlign(VAlign::Center);
    }

    Label::Label() :
        _p(new Private)
    {}

    Label::~Label()
    {}

    std::shared_ptr<Label> Label::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Label>(new Label);
        out->_init(context, parent);
        return out;
    }

    std::shared_ptr<Label> Label::create(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = create(context, parent);
        out->setText(text);
        return out;
    }

    const std::string& Label::getText() const
    {
        return _p->text;
    }

    void Label::setText(const std::string& value)
    {
        FTK_P();
        if (value == p.text)
            return;
        p.text = value;
        p.size.init = true;
        setSizeUpdate();
        setDrawUpdate();
    }

    ColorRole Label::getTextRole() const
    {
        return _p->textRole;
    }

    void Label::setTextRole(ColorRole value)
    {
        FTK_P();
        if (value == p.textRole)
            return;
        p.textRole = value;
        setDrawUpdate();
    }

    SizeRole Label::getMarginRole() const
    {
        return _p->hMarginRole;
    }

    SizeRole Label::getHMarginRole() const
    {
        return _p->hMarginRole;
    }

    SizeRole Label::getVMarginRole() const
    {
        return _p->vMarginRole;
    }

    void Label::setMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.hMarginRole && value == p.vMarginRole)
            return;
        p.hMarginRole = value;
        p.vMarginRole = value;
        p.size.init = true;
        setSizeUpdate();
        setDrawUpdate();
    }

    void Label::setMarginRole(SizeRole horizontal, SizeRole vertical)
    {
        FTK_P();
        if (horizontal == p.hMarginRole && vertical == p.vMarginRole)
            return;
        p.hMarginRole = horizontal;
        p.vMarginRole = vertical;
        p.size.init = true;
        setSizeUpdate();
        setDrawUpdate();
    }

    void Label::setHMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.hMarginRole)
            return;
        p.hMarginRole = value;
        p.size.init = true;
        setSizeUpdate();
        setDrawUpdate();
    }

    void Label::setVMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.vMarginRole)
            return;
        p.vMarginRole = value;
        p.size.init = true;
        setSizeUpdate();
        setDrawUpdate();
    }

    FontType Label::getFont() const
    {
        return _p->font;
    }

    void Label::setFont(FontType value)
    {
        FTK_P();
        if (value == p.font)
            return;
        p.font = value;
        p.size.init = true;
        setSizeUpdate();
        setDrawUpdate();
    }

    int Label::getFontSize() const
    {
        return _p->fontSize;
    }

    void Label::setFontSize(int value)
    {
        FTK_P();
        if (value == p.fontSize)
            return;
        p.fontSize = value;
        p.size.init = true;
        setSizeUpdate();
        setDrawUpdate();
    }

    bool Label::getClipText() const
    {
        return _p->clipText;
    }

    void Label::setClipText(bool value)
    {
        FTK_P();
        if (value == p.clipText)
            return;
        p.clipText = value;
        setDrawUpdate();
    }

    bool Label::getElide() const
    {
        return _p->elide;
    }

    ElideMode Label::getElideMode() const
    {
        return _p->elideMode;
    }

    void Label::setElide(bool value, ElideMode mode)
    {
        FTK_P();
        if (value == p.elide && mode == p.elideMode)
            return;
        // A layout sizes a left aligned widget to its size hint, which here
        // is only the ellipsis; filling is what lets the label have the room
        // it is given. The text is drawn from the left either way. Turning
        // eliding off puts back the label's own alignment, unless something
        // has changed it since.
        if (value != p.elide)
        {
            if (value)
            {
                setHAlign(HAlign::Fill);
            }
            else if (HAlign::Fill == getHAlign())
            {
                setHAlign(HAlign::Left);
            }
        }
        p.elide = value;
        p.elideMode = mode;
        p.size.init = true;
        setSizeUpdate();
        setDrawUpdate();
    }

    Size2I Label::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void Label::setGeometry(const Box2I& value)
    {
        if (value != getGeometry())
        {
            _p->draw.reset();
        }
        IWidget::setGeometry(value);
    }

    void Label::styleEvent(const StyleEvent& event)
    {
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
            p.draw.reset();
        }
    }

    void Label::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.hMargin = event.style->getSizeRole(p.hMarginRole, event.displayScale);
            p.size.vMargin = event.style->getSizeRole(p.vMarginRole, event.displayScale);
            p.size.fontInfo = event.style->getFont(p.font, p.fontSize, event.displayScale);
            p.size.fontMetrics = event.fontSystem->getMetrics(p.size.fontInfo);
            p.size.textSize = event.fontSystem->getSize(p.text, p.size.fontInfo);

            Size2I size = p.size.textSize;
            if (p.elide)
            {
                // No wider than what is left when nothing fits, so the
                // text never decides how wide the label's parent is.
                const Size2I ellipsisSize =
                    event.fontSystem->getSize(ellipsis, p.size.fontInfo);
                size.w = std::min(size.w, ellipsisSize.w);
                size.h = std::max(size.h, ellipsisSize.h);
            }
            p.size.sizeHint = margin(size, p.size.hMargin, p.size.vMargin);

            p.draw.reset();
        }
    }

    void Label::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void Label::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        IWidget::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->g = getGeometry();
            p.draw->g2 = margin(p.draw->g, -p.size.hMargin, -p.size.vMargin, -p.size.hMargin, -p.size.vMargin);
            if (!p.text.empty())
            {
                const std::string text =
                    p.elide && p.size.textSize.w > p.draw->g2.w() ?
                    elideToWidth(
                        event.fontSystem,
                        p.text,
                        p.size.fontInfo,
                        p.draw->g2.w(),
                        p.elideMode) :
                    p.text;
                p.draw->glyphs = event.fontSystem->getGlyphs(text, p.size.fontInfo);
            }
        }

        // Clipped when eliding too: with less room than the ellipsis
        // itself, even that is wider than the label.
        const bool clip = p.clipText || p.elide;
        const bool clipRectEnabledPrev = event.render->getClipRectEnabled();
        const Box2I clipRectPrev = event.render->getClipRect();
        if (clip)
        {
            event.render->setClipRectEnabled(true);
            event.render->setClipRect(getGeometry());
        }

        event.render->drawText(
            p.draw->glyphs,
            p.size.fontMetrics,
            p.draw->g2.min,
            event.style->getColorRole(p.textRole, isEnabled()));

        if (clip)
        {
            event.render->setClipRectEnabled(clipRectEnabledPrev);
            event.render->setClipRect(clipRectPrev);
        }
    }
}
