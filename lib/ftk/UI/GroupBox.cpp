// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/GroupBox.h>

#include <ftk/UI/DrawUtil.h>

#include <optional>

namespace ftk
{
    struct GroupBox::Private
    {
        std::string text;
        FontRole fontRole = FontRole::Label;

        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
            int spacing = 0;
            int border = 0;
            FontInfo fontInfo;
            FontMetrics fontMetrics;
            Size2I textSize;
        };
        SizeData size;

        struct DrawData
        {
            Box2I g;
            Box2I g2;
            TriMesh2F border;
            std::vector<std::shared_ptr<Glyph> > glyphs;
        };
        std::optional<DrawData> draw;
    };

    void GroupBox::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::GroupBox", parent);
    }

    GroupBox::GroupBox() :
        _p(new Private)
    {}

    GroupBox::~GroupBox()
    {}

    std::shared_ptr<GroupBox> GroupBox::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<GroupBox>(new GroupBox);
        out->_init(context, parent);
        return out;
    }

    std::shared_ptr<GroupBox> GroupBox::create(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = create(context, parent);
        out->setText(text);
        return out;
    }

    const std::string& GroupBox::getText() const
    {
        return _p->text;
    }

    void GroupBox::setText(const std::string& value)
    {
        FTK_P();
        if (value == p.text)
            return;
        p.text = value;
        p.size.displayScale.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    FontRole GroupBox::getFontRole() const
    {
        return _p->fontRole;
    }

    void GroupBox::setFontRole(FontRole value)
    {
        FTK_P();
        if (value == p.fontRole)
            return;
        p.fontRole = value;
        p.size.displayScale.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    void GroupBox::clear()
    {
        auto children = getChildren();
        for (auto child : children)
        {
            child->setParent(nullptr);
        }
    }

    void GroupBox::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IWidget::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.draw.reset();
        }

        Box2I g = value;
        g.min.y += p.size.fontMetrics.lineHeight + p.size.spacing;
        g = margin(g, -(p.size.border + p.size.margin));
        for (const auto& child : getChildren())
        {
            child->setGeometry(g);
        }
    }

    void GroupBox::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();

        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(SizeRole::MarginSmall, event.displayScale);
            p.size.spacing = event.style->getSizeRole(SizeRole::SpacingSmall, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.fontInfo = event.style->getFontRole(p.fontRole, event.displayScale);
            p.size.fontMetrics = event.fontSystem->getMetrics(p.size.fontInfo);
            p.size.textSize = event.fontSystem->getSize(p.text, p.size.fontInfo);
            p.draw.reset();
        }

        Size2I sizeHint;
        for (const auto& child : getChildren())
        {
            const Size2I& childSizeHint = child->getSizeHint();
            sizeHint.w = std::max(sizeHint.w, childSizeHint.w);
            sizeHint.h = std::max(sizeHint.h, childSizeHint.h);
        }
        sizeHint = margin(sizeHint, p.size.margin + p.size.border);
        sizeHint.w = std::max(sizeHint.w, p.size.textSize.w);
        sizeHint.h += p.size.fontMetrics.lineHeight + p.size.spacing;
        setSizeHint(sizeHint);
    }

    void GroupBox::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void GroupBox::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IWidget::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->g = getGeometry();
            p.draw->g2 = Box2I(
                V2I(
                    p.draw->g.min.x,
                    p.draw->g.min.y + p.size.fontMetrics.lineHeight + p.size.spacing),
                p.draw->g.max);
            p.draw->border = border(p.draw->g2, p.size.border, p.size.margin);
        }

        if (!p.text.empty() && p.draw->glyphs.empty())
        {
            p.draw->glyphs = event.fontSystem->getGlyphs(p.text, p.size.fontInfo);
        }
        event.render->drawText(
            p.draw->glyphs,
            p.size.fontMetrics,
            p.draw->g.min,
            event.style->getColorRole(ColorRole::Text));

        event.render->drawMesh(
            p.draw->border,
            event.style->getColorRole(ColorRole::Border));
    }
}