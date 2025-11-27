// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/RowLayout.h>

#include <ftk/UI/LayoutUtil.h>
#include <ftk/UI/Spacer.h>

#include <optional>

namespace ftk
{
    struct RowLayout::Private
    {
        Orientation orientation = Orientation::Horizontal;
        SizeRole marginRole = SizeRole::None;
        SizeRole spacingRole = SizeRole::Spacing;

        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
            int spacing = 0;
        };
        SizeData size;

        struct GeomData
        {
            Box2I g;
            Box2I g2;
        };
        GeomData geom;
    };

    void RowLayout::_init(
        const std::shared_ptr<Context>& context,
        Orientation orientation,
        const std::string& objectName,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, objectName, parent);
        FTK_P();
        p.orientation = orientation;
    }

    RowLayout::RowLayout() :
        _p(new Private)
    {}

    RowLayout::~RowLayout()
    {}

    std::shared_ptr<RowLayout> RowLayout::create(
        const std::shared_ptr<Context>& context,
        Orientation orientation,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<RowLayout>(new RowLayout);
        out->_init(context, orientation, "ftk::RowLayout", parent);
        return out;
    }

    SizeRole RowLayout::getMarginRole() const
    {
        return _p->marginRole;
    }

    void RowLayout::setMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.marginRole)
            return;
        p.marginRole = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    SizeRole RowLayout::getSpacingRole() const
    {
        return _p->spacingRole;
    }

    void RowLayout::setSpacingRole(SizeRole value)
    {
        FTK_P();
        if (value == p.spacingRole)
            return;
        p.spacingRole = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    void RowLayout::addSpacer(Stretch stretch)
    {
        FTK_P();
        addSpacer(p.spacingRole, stretch);
    }

    void RowLayout::addSpacer(SizeRole spacingRole, Stretch stretch)
    {
        FTK_P();
        if (auto context = getContext())
        {
            auto spacer = Spacer::create(context, p.orientation, shared_from_this());
            spacer->setSpacingRole(spacingRole);
            switch (p.orientation)
            {
            case Orientation::Horizontal:
                spacer->setHStretch(stretch);
                break;
            case Orientation::Vertical:
                spacer->setVStretch(stretch);
                break;
            default: break;
            }
        }
    }

    void RowLayout::clear()
    {
        auto children = getChildren();
        for (auto child : children)
        {
            child->setParent(nullptr);
        }
    }

    void RowLayout::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        FTK_P();
        p.geom.g = align(value, getSizeHint(), getHAlign(), getVAlign());
        p.geom.g2 = margin(p.geom.g, -p.size.margin);

        std::vector<Size2I> sizeHints;
        size_t expanding = 0;
        std::shared_ptr<IWidget> lastVisibleChild;
        const auto& children = getChildren();
        for (const auto& child : children)
        {
            if (child->isVisible(false))
            {
                sizeHints.push_back(child->getSizeHint());
                switch (p.orientation)
                {
                case Orientation::Horizontal:
                    if (Stretch::Expanding == child->getHStretch())
                    {
                        ++expanding;
                    }
                    break;
                case Orientation::Vertical:
                    if (Stretch::Expanding == child->getVStretch())
                    {
                        ++expanding;
                    }
                    break;
                default: break;
                }
                lastVisibleChild = child;
            }
        }
        const std::pair<int, int> extra(
            p.geom.g.w() - getSizeHint().w,
            p.geom.g.h() - getSizeHint().h);
        V2I pos = p.geom.g2.min;
        size_t count = 0;
        for (const auto& child : children)
        {
            if (child->isVisible(false))
            {
                Size2I size = sizeHints[count];
                switch (p.orientation)
                {
                case Orientation::Horizontal:
                    switch (getVAlign())
                    {
                    case VAlign::Fill:
                        size.h = p.geom.g2.h();
                        break;
                    case VAlign::Center:
                        pos.y = p.geom.g2.min.y + p.geom.g2.h() / 2 - size.h / 2;
                        break;
                    case VAlign::Bottom:
                        pos.y = p.geom.g2.min.y + p.geom.g2.h() - size.h;
                        break;
                    default: break;
                    }
                    if (expanding > 0 && Stretch::Expanding == child->getHStretch())
                    {
                        size.w += extra.first / expanding;
                        if (child == lastVisibleChild)
                        {
                            size.w += extra.first - (extra.first / expanding * expanding);
                        }
                    }
                    break;
                case Orientation::Vertical:
                    switch (getHAlign())
                    {
                    case HAlign::Fill:
                        size.w = p.geom.g2.w();
                        break;
                    case HAlign::Center:
                        pos.x = p.geom.g2.min.x + p.geom.g2.w() / 2 - size.w / 2;
                        break;
                    case HAlign::Right:
                        pos.x = p.geom.g2.min.x + p.geom.g2.w() - size.w;
                        break;
                    default: break;
                    }
                    if (expanding > 0 && Stretch::Expanding == child->getVStretch())
                    {
                        size.h += extra.second / expanding;
                        if (child == lastVisibleChild)
                        {
                            size.h += extra.second - (extra.second / expanding * expanding);
                        }
                    }
                    break;
                default: break;
                }
                child->setGeometry(Box2I(pos, size));
                switch (p.orientation)
                {
                case Orientation::Horizontal:
                    pos.x += size.w + p.size.spacing;
                    break;
                case Orientation::Vertical:
                    pos.y += size.h + p.size.spacing;
                    break;
                default: break;
                }
                ++count;
            }
        }
    }

    Box2I RowLayout::getChildrenClipRect() const
    {
        return _p->geom.g2;
    }

    void RowLayout::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();

        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(p.marginRole, event.displayScale);
            p.size.spacing = event.style->getSizeRole(p.spacingRole, event.displayScale);
        }

        Size2I sizeHint;
        std::vector<Size2I> sizeHints;
        size_t visible = 0;
        for (const auto& child : getChildren())
        {
            if (child->isVisible(false))
            {
                const Size2I& childSizeHint = child->getSizeHint();
                sizeHints.push_back(childSizeHint);
                switch (p.orientation)
                {
                case Orientation::Horizontal:
                    sizeHint.w += childSizeHint.w;
                    sizeHint.h = std::max(sizeHint.h, childSizeHint.h);
                    ++visible;
                    break;
                case Orientation::Vertical:
                    sizeHint.w = std::max(sizeHint.w, childSizeHint.w);
                    sizeHint.h += childSizeHint.h;
                    ++visible;
                    break;
                default: break;
                }
            }
        }
        if (visible > 0)
        {
            switch (p.orientation)
            {
            case Orientation::Horizontal:
                sizeHint.w += p.size.spacing * (visible - 1);
                break;
            case Orientation::Vertical:
                sizeHint.h += p.size.spacing * (visible - 1);
                break;
            default: break;
            }
        }
        sizeHint.w += p.size.margin * 2;
        sizeHint.h += p.size.margin * 2;
        setSizeHint(sizeHint);
    }

    void RowLayout::childAddEvent(const ChildAddEvent& event)
    {
        IWidget::childAddEvent(event);
        setSizeUpdate();
        setDrawUpdate();
    }

    void RowLayout::childRemoveEvent(const ChildRemoveEvent& event)
    {
        IWidget::childRemoveEvent(event);
        setSizeUpdate();
        setDrawUpdate();
    }

    void HorizontalLayout::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        RowLayout::_init(
            context,
            Orientation::Horizontal,
            "ftk::HorizontalLayout",
            parent);
    }

    HorizontalLayout::HorizontalLayout()
    {}

    HorizontalLayout::~HorizontalLayout()
    {}

    std::shared_ptr<HorizontalLayout> HorizontalLayout::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<HorizontalLayout>(new HorizontalLayout);
        out->_init(context, parent);
        return out;
    }

    void VerticalLayout::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        RowLayout::_init(
            context,
            Orientation::Vertical,
            "ftk::VerticalLayout",
            parent);
    }

    VerticalLayout::VerticalLayout()
    {}

    VerticalLayout::~VerticalLayout()
    {}

    std::shared_ptr<VerticalLayout> VerticalLayout::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<VerticalLayout>(new VerticalLayout);
        out->_init(context, parent);
        return out;
    }
}