// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/OverlayLayout.h>

#include <optional>

namespace ftk
{
    struct OverlayLayout::Private
    {
        SizeRole marginRole = SizeRole::None;

        struct SizeData
        {
            int margin = 0;
        };
        std::optional<SizeData> size;
    };

    void OverlayLayout::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::OverlayLayout", parent);
    }

    OverlayLayout::OverlayLayout() :
        _p(new Private)
    {}

    OverlayLayout::~OverlayLayout()
    {}

    std::shared_ptr<OverlayLayout> OverlayLayout::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<OverlayLayout>(new OverlayLayout);
        out->_init(context, parent);
        return out;
    }

    SizeRole OverlayLayout::getMarginRole() const
    {
        return _p->marginRole;
    }

    void OverlayLayout::setMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.marginRole)
            return;
        p.marginRole = value;
        p.size.reset();
        setSizeUpdate();
        setDrawUpdate();
    }
    
    Size2I OverlayLayout::getSizeHint() const
    {
        FTK_P();
        Size2I out;
        for (const auto& child : getChildren())
        {
            const Size2I& childSizeHint = child->getSizeHint();
            out.w = std::max(out.w, childSizeHint.w);
            out.h = std::max(out.h, childSizeHint.h);
        }
        out.w += p.size->margin * 2;
        out.h += p.size->margin * 2;
        return out;
    }

    void OverlayLayout::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        FTK_P();
        const Box2I g = margin(getGeometry(), -p.size->margin);
        for (const auto& child : getChildren())
        {
            child->setGeometry(g);
        }
    }

    Box2I OverlayLayout::getChildrenClipRect() const
    {
        return margin(getGeometry(), -_p->size->margin);
    }

    void OverlayLayout::styleEvent(const StyleEvent& event)
    {
        FTK_P();
        if (event.hasChanges())
        {
            p.size.reset();
        }
    }

    void OverlayLayout::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (!p.size.has_value())
        {
            p.size = Private::SizeData();
            p.size->margin = event.style->getSizeRole(p.marginRole, event.displayScale);
        }
    }
}
