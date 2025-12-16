// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/MDIWidget.h>

#include <ftk/UI/Divider.h>
#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <optional>
#include <sstream>

namespace ftk
{
    FTK_ENUM_IMPL(
        MDIResize,
        "North",
        "North East",
        "East",
        "South East",
        "South",
        "South West",
        "West",
        "North West");

    Box2I getResizeBox(MDIResize value, const Box2I& g, int handle)
    {
        Box2I out;
        switch (value)
        {
        case MDIResize::North:
            out = Box2I(
                g.min.x + handle,
                g.min.y - handle,
                g.w() - handle * 2,
                handle);
            break;
        case MDIResize::NorthEast:
            out = Box2I(
                g.max.x - handle,
                g.min.y - handle,
                handle * 2,
                handle * 2);
            break;
        case MDIResize::East:
            out = Box2I(
                g.max.x,
                g.min.y + handle,
                handle,
                g.h() - handle * 2);
            break;
        case MDIResize::SouthEast:
            out = Box2I(
                g.max.x - handle,
                g.max.y - handle,
                handle * 2,
                handle * 2);
            break;
        case MDIResize::South:
            out = Box2I(
                g.min.x + handle,
                g.max.y,
                g.w() - handle * 2,
                handle);
            break;
        case MDIResize::SouthWest:
            out = Box2I(
                g.min.x - handle,
                g.max.y - handle,
                handle * 2,
                handle * 2);
            break;
        case MDIResize::West:
            out = Box2I(
                g.min.x - handle,
                g.min.y + handle,
                handle,
                g.h() - handle * 2);
            break;
        case MDIResize::NorthWest:
            out = Box2I(
                g.min.x - handle,
                g.min.y - handle,
                handle * 2,
                handle * 2);
            break;
        default: break;
        }
        return out;
    }

    Box2I mdiAddMargins(const Box2I& box, const std::array<int, 4>& margins)
    {
        return margin(box, margins[0], margins[1], margins[2], margins[3]);
    }

    Box2I mdiRemoveMargins(const Box2I& box, const std::array<int, 4>& margins)
    {
        return margin(box, -margins[0], -margins[1], -margins[2], -margins[3]);
    }

    Size2I mdiRemoveMargins(const Size2I& size, const std::array<int, 4>& margins)
    {
        return Size2I(
            size.w - margins[0] - margins[2],
            size.h - margins[1] - margins[3]);
    }

    struct MDIWidget::Private
    {
        std::shared_ptr<Label> titleLabel;
        std::shared_ptr<ToolButton> closeButton;
        std::shared_ptr<IWidget> widget;
        std::shared_ptr<VerticalLayout> widgetLayout;
        std::shared_ptr<VerticalLayout> layout;
        std::function<void(bool)> pressCallback;
        std::function<void(const V2I&)> moveCallback;
        std::function<void(MDIResize, const V2I&)> resizeCallback;

        struct SizeData
        {
            std::optional<float> displayScale;
            int border = 0;
            int handle = 0;
            int shadow = 0;
            std::array<int, 4> margins;
        };
        SizeData size;

        struct DrawData
        {
            TriMesh2F border;
            TriMesh2F shadow;
        };
        std::optional<DrawData> draw;

        struct MouseData
        {
            MDIResize resize = MDIResize::None;
        };
        MouseData mouse;
    };

    void MDIWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::MDIWidget", parent);
        FTK_P();

        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);

        p.titleLabel = Label::create(context);
        p.titleLabel->setText(title);
        p.titleLabel->setMarginRole(SizeRole::MarginSmall);
        p.titleLabel->setHStretch(Stretch::Expanding);

        p.closeButton = ToolButton::create(context);
        p.closeButton->setIcon("Close");
        p.closeButton->setTooltip("Close the window");

        p.layout = VerticalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::None);
        auto hLayout = HorizontalLayout::create(context, p.layout);
        hLayout->setSpacingRole(SizeRole::None);
        hLayout->setBackgroundRole(ColorRole::Button);
        p.titleLabel->setParent(hLayout);
        p.closeButton->setParent(hLayout);
        Divider::create(context, Orientation::Vertical, p.layout);
        p.widgetLayout = VerticalLayout::create(context, p.layout);
        p.widgetLayout->setMarginRole(SizeRole::MarginInside);
        p.widgetLayout->setVStretch(Stretch::Expanding);

        p.closeButton->setClickedCallback(
            [this]
            {
                setParent(nullptr);
            });
    }

    MDIWidget::MDIWidget() :
        _p(new Private)
    {}

    MDIWidget::~MDIWidget()
    {}

    std::shared_ptr<MDIWidget> MDIWidget::create(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<MDIWidget>(new MDIWidget);
        out->_init(context, title, parent);
        return out;
    }

    const std::string& MDIWidget::getTitle() const
    {
        return _p->titleLabel->getText();
    }

    void MDIWidget::setTitle(const std::string& value)
    {
        _p->titleLabel->setText(value);
    }

    const std::shared_ptr<IWidget>& MDIWidget::getWidget() const
    {
        return _p->widget;
    }

    void MDIWidget::setWidget(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        if (p.widget)
        {
            p.widget->setParent(nullptr);
        }
        p.widget = value;
        if (p.widget)
        {
            p.widget->setParent(p.widgetLayout);
        }
        setSizeUpdate();
        setDrawUpdate();
    }

    void MDIWidget::setPressCallback(const std::function<void(bool)>& value)
    {
        _p->pressCallback = value;
    }

    void MDIWidget::setMoveCallback(const std::function<void(const V2I&)>& value)
    {
        _p->moveCallback = value;
    }

    void MDIWidget::setResizeCallback(const std::function<void(MDIResize, const V2I&)>& value)
    {
        _p->resizeCallback = value;
    }

    std::array<int, 4> MDIWidget::getMargins() const
    {
        return _p->size.margins;
    }

    void MDIWidget::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IMouseWidget::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.draw.reset();
        }

        const Box2I g = ftk::margin(
            value,
            -(p.size.margins[0] + p.size.border),
            -(p.size.margins[1] + p.size.border),
            -(p.size.margins[2] + p.size.border),
            -(p.size.margins[3] + p.size.border));
        p.layout->setGeometry(g);

        /*p.mouse.resizeBoxes.clear();
        p.mouse.resizeBoxes[MDIResize::North] = Box2I(
            g.min.x + p.size.handle,
            g.min.y - p.size.handle,
            g.w() - p.size.handle * 2,
            p.size.handle);
        p.mouse.resizeBoxes[MDIResize::NorthEast] = Box2I(
            g.max.x - p.size.handle,
            g.min.y - p.size.handle,
            p.size.handle * 2,
            p.size.handle * 2);
        p.mouse.resizeBoxes[MDIResize::East] = Box2I(
            g.max.x,
            g.min.y + p.size.handle,
            p.size.handle,
            g.h() - p.size.handle * 2);
        p.mouse.resizeBoxes[MDIResize::SouthEast] = Box2I(
            g.max.x - p.size.handle,
            g.max.y - p.size.handle,
            p.size.handle * 2,
            p.size.handle * 2);
        p.mouse.resizeBoxes[MDIResize::South] = Box2I(
            g.min.x + p.size.handle,
            g.max.y,
            g.w() - p.size.handle * 2,
            p.size.handle);
        p.mouse.resizeBoxes[MDIResize::SouthWest] = Box2I(
            g.min.x - p.size.handle,
            g.max.y - p.size.handle,
            p.size.handle * 2,
            p.size.handle * 2);
        p.mouse.resizeBoxes[MDIResize::West] = Box2I(
            g.min.x - p.size.handle,
            g.min.y + p.size.handle,
            p.size.handle,
            g.h() - p.size.handle * 2);
        p.mouse.resizeBoxes[MDIResize::NorthWest] = Box2I(
            g.min.x - p.size.handle,
            g.min.y - p.size.handle,
            p.size.handle * 2,
            p.size.handle * 2);*/
    }

    void MDIWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        FTK_P();

        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);
            p.size.shadow = event.style->getSizeRole(SizeRole::Shadow, event.displayScale);
            p.size.margins[0] = std::max(p.size.handle, p.size.shadow);
            p.size.margins[1] = p.size.handle;
            p.size.margins[2] = std::max(p.size.handle, p.size.shadow);
            p.size.margins[3] = std::max(p.size.handle, p.size.shadow);
            p.draw.reset();
        }

        Size2I sizeHint = p.layout->getSizeHint() + p.size.border * 2;
        sizeHint.w += p.size.margins[0] + p.size.margins[2];
        sizeHint.h += p.size.margins[1] + p.size.margins[3];
        setSizeHint(sizeHint);
    }

    void MDIWidget::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IMouseWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void MDIWidget::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IMouseWidget::drawEvent(drawRect, event);
        FTK_P();

        const Box2I& g = getGeometry();
        const Box2I g2 = ftk::margin(
            g,
            -p.size.margins[0],
            -p.size.margins[1],
            -p.size.margins[2],
            -p.size.margins[3]);
        const Box2I g3 = ftk::margin(g2, -p.size.border);
        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->border = border(g3, p.size.border);
            p.draw->shadow = shadow(
                ftk::margin(g2, p.size.shadow, 0, p.size.shadow, p.size.shadow),
                p.size.shadow);
        }

        event.render->drawColorMesh(p.draw->shadow);

        if (p.mouse.resize != MDIResize::None)
        {
            event.render->drawRect(
                getResizeBox(p.mouse.resize, g2, p.size.handle),
                event.style->getColorRole(ColorRole::Checked));
        }

        event.render->drawMesh(
            p.draw->border,
            event.style->getColorRole(ColorRole::Border));

        event.render->drawRect(
            g3,
            event.style->getColorRole(ColorRole::Window));
    }

    void MDIWidget::mouseLeaveEvent()
    {
        IMouseWidget::mouseLeaveEvent();
        FTK_P();
        if (p.mouse.resize != MDIResize::None)
        {
            p.mouse.resize = MDIResize::None;
            setDrawUpdate();
        }
    }

    void MDIWidget::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        FTK_P();
        if (!_isMousePressed())
        {
            const Box2I& g = getGeometry();
            const Box2I g2 = ftk::margin(
                g,
                -p.size.margins[0],
                -p.size.margins[1],
                -p.size.margins[2],
                -p.size.margins[3]);
            MDIResize resize = MDIResize::None;
            for (auto i : getMDIResizeEnums())
            {
                if (contains(getResizeBox(i, g2, p.size.handle), event.pos))
                {
                    resize = i;
                    break;
                }
            }
            if (resize != p.mouse.resize)
            {
                p.mouse.resize = resize;
                setDrawUpdate();
            }
        }
        else
        {
            const V2I& mousePressPos = _getMousePressPos();
            if (p.mouse.resize != MDIResize::None)
            {
                if (p.resizeCallback)
                {
                    p.resizeCallback(p.mouse.resize, event.pos - mousePressPos);
                }
            }
            else if (p.moveCallback)
            {
                p.moveCallback(event.pos - mousePressPos);
            }
        }
    }

    void MDIWidget::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        if (p.pressCallback)
        {
            p.pressCallback(true);
        }
    }

    void MDIWidget::mouseReleaseEvent(MouseClickEvent& event)
    {
        IMouseWidget::mouseReleaseEvent(event);
        FTK_P();
        if (p.pressCallback)
        {
            p.pressCallback(false);
        }
    }

    /*Box2I MDIWidget::_addMargins(const Box2I& value) const
    {
        FTK_P();
        return ftk::margin(value, p.size.handle);
    }

    Box2I MDIWidget::_removeMargins(const Box2I& value) const
    {
        FTK_P();
        return ftk::margin(value, -p.size.handle);
    }

    Size2I MDIWidget::_removeMargins(const Size2I& value) const
    {
        FTK_P();
        return Size2I(value.w - p.size.handle * 2, value.h - p.size.handle * 2);
    }*/
}
