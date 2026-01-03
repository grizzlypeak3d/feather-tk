// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/Icon.h>

#include <ftk/UI/LayoutUtil.h>

#include <ftk/Core/String.h>

#include <optional>

namespace ftk
{
    struct Icon::Private
    {
        std::string icon;
        float iconScale = 1.F;
        std::shared_ptr<Image> iconImage;
        SizeRole marginRole = SizeRole::None;

        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
        };
        SizeData size;
    };

    void Icon::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::Icon", parent);
        setHAlign(HAlign::Left);
        setVAlign(VAlign::Center);
    }

    Icon::Icon() :
        _p(new Private)
    {}

    Icon::~Icon()
    {}

    std::shared_ptr<Icon> Icon::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Icon>(new Icon);
        out->_init(context, parent);
        return out;
    }

    std::shared_ptr<Icon> Icon::create(
        const std::shared_ptr<Context>& context,
        const std::string& icon,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = create(context, parent);
        out->setIcon(icon);
        return out;
    }

    const std::string& Icon::getIcon() const
    {
        return _p->icon;
    }

    void Icon::setIcon(const std::string& value)
    {
        FTK_P();
        if (value == p.icon)
            return;
        p.icon = value;
        p.iconImage.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    SizeRole Icon::getMarginRole() const
    {
        return _p->marginRole;
    }

    Size2I Icon::getSizeHint() const
    {
        FTK_P();
        Size2I out;
        if (p.iconImage)
        {
            out.w = p.iconImage->getWidth();
            out.h = p.iconImage->getHeight();
        }
        return out;
    }

    void Icon::setMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.marginRole)
            return;
        p.marginRole = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    void Icon::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(p.marginRole, event.displayScale);
        }
        if (event.displayScale != p.iconScale)
        {
            p.iconScale = event.displayScale;
            p.iconImage.reset();
        }
        if (!p.icon.empty() && !p.iconImage)
        {
            p.iconImage = event.iconSystem->get(p.icon, event.displayScale);
        }
    }

    void Icon::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IWidget::drawEvent(drawRect, event);
        FTK_P();
        if (p.iconImage)
        {
            const Box2I g = margin(getGeometry(), -p.size.margin);
            const Size2I& iconSize = p.iconImage->getSize();
            event.render->drawImage(
                p.iconImage,
                Box2I(
                    g.x() + g.w() / 2 - iconSize.w / 2,
                    g.y() + g.h() / 2 - iconSize.h / 2,
                    iconSize.w,
                    iconSize.h),
                event.style->getColorRole(isEnabled() ?
                    ColorRole::Text :
                    ColorRole::TextDisabled));
        }
    }
}
