// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Tooltip.h>

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/IWindow.h>
#include <ftk/UI/Label.h>

#include <optional>

namespace ftk
{
    struct Tooltip::Private
    {
        V2I pos;

        std::shared_ptr<Label> label;

        struct SizeData
        {
            std::optional<float> displayScale;
            int border = 0;
            int handle = 0;
            int shadow = 0;
        };
        SizeData size;

        struct DrawData
        {
            Box2I g;
            TriMesh2F shadow;
            TriMesh2F border;
        };
        std::optional<DrawData> draw;
    };

    void Tooltip::_init(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const V2I& pos,
        const std::shared_ptr<IWidget>& window)
    {
        IPopup::_init(context, "ftk::Tooltip", nullptr);
        FTK_P();

        p.pos = pos;

        p.label = Label::create(context, shared_from_this());
        p.label->setText(text);
        p.label->setTextRole(ColorRole::TooltipText);
        p.label->setMarginRole(SizeRole::MarginSmall);

        setParent(window);
    }

    Tooltip::Tooltip() :
        _p(new Private)
    {}

    Tooltip::~Tooltip()
    {}

    std::shared_ptr<Tooltip> Tooltip::create(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const V2I& pos,
        const std::shared_ptr<IWidget>& window)
    {
        auto out = std::shared_ptr<Tooltip>(new Tooltip);
        out->_init(context, text, pos, window);
        return out;
    }

    void Tooltip::close()
    {
        setParent(nullptr);
    }

    void Tooltip::setGeometry(const Box2I& value)
    {
        IPopup::setGeometry(value);
        FTK_P();

        Size2I sizeHint = p.label->getSizeHint();
        std::list<Box2I> boxes;
        boxes.push_back(Box2I(
            p.pos.x + p.size.handle,
            p.pos.y + p.size.handle,
            sizeHint.w,
            sizeHint.h));
        boxes.push_back(Box2I(
            p.pos.x - p.size.handle - sizeHint.w,
            p.pos.y + p.size.handle,
            sizeHint.w,
            sizeHint.h));
        boxes.push_back(Box2I(
            p.pos.x + p.size.handle,
            p.pos.y - p.size.handle - sizeHint.h,
            sizeHint.w,
            sizeHint.h));
        boxes.push_back(Box2I(
            p.pos.x - p.size.handle - sizeHint.w,
            p.pos.y - p.size.handle - sizeHint.h,
            sizeHint.w,
            sizeHint.h));
        struct Intersect
        {
            Box2I box;
            float area = 0.F;
        };
        std::vector<Intersect> intersect;
        for (const auto& box : boxes)
        {
            intersect.push_back({ box, area(ftk::intersect(box, value)) });
        }
        std::sort(
            intersect.begin(),
            intersect.end(),
            [value](const Intersect& a, const Intersect& b)
            {
                return a.area > b.area;
            });
        Box2I g = boxes.front();
        if (g.max.x > value.max.x)
        {
            const int diff = g.max.x - value.max.x;
            g.min.x -= diff;
            g.max.x -= diff;
        }
        if (g.min.x < value.min.x)
        {
            g.max.x += value.min.x - g.min.x;
            g.min.x = value.min.x;
        }
        if (g.max.y > value.max.y)
        {
            const int diff = g.max.y - value.max.y;
            g.min.y -= diff;
            g.max.y -= diff;
        }
        if (g.min.y < value.min.y)
        {
            g.max.y += value.min.y - g.min.y;
            g.min.y = value.min.y;
        }
        const bool changed = g != p.label->getGeometry();
        p.label->setGeometry(g);

        if (!p.draw.has_value() || changed)
        {
            p.draw = Private::DrawData();
            p.draw->g = g;
            const Box2I g2(
                g.min.x - p.size.shadow,
                g.min.y,
                g.w() + p.size.shadow * 2,
                g.h() + p.size.shadow);
            p.draw->shadow = shadow(g2, p.size.shadow);
            p.draw->border = border(margin(p.draw->g, p.size.border), p.size.border);
        }
    }

    void Tooltip::sizeHintEvent(const SizeHintEvent& event)
    {
        IPopup::sizeHintEvent(event);
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);
            p.size.shadow = event.style->getSizeRole(SizeRole::Shadow, event.displayScale);
            p.draw.reset();
        }
    }

    void Tooltip::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void Tooltip::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IPopup::drawEvent(drawRect, event);
        FTK_P();

        if (p.draw.has_value())
        {
            event.render->drawColorMesh(p.draw->shadow);
            event.render->drawMesh(
                p.draw->border,
                event.style->getColorRole(ColorRole::Border));
            event.render->drawRect(
                p.draw->g,
                event.style->getColorRole(ColorRole::TooltipWindow));
        }
    }
}
