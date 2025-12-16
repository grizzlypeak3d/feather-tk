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
        };
        SizeData size;

        struct DrawData
        {
            Box2I g;
            TriMesh2F border;
        };
        std::optional<DrawData> draw;

        struct MouseData
        {
            MDIResize resize = MDIResize::None;
            std::map<MDIResize, Box2I> resizeBoxes;
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

    void MDIWidget::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IMouseWidget::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.draw.reset();
        }

        Box2I g = ftk::margin(
            value,
            -(p.size.handle + p.size.border),
            -(p.size.handle + p.size.border),
            -(p.size.handle + p.size.border),
            -(p.size.handle + p.size.border));
        p.layout->setGeometry(g);

        p.mouse.resizeBoxes.clear();
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
            p.size.handle * 2);
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
            p.draw.reset();
        }

        Size2I sizeHint = p.layout->getSizeHint() + p.size.border * 2;
        sizeHint.w += p.size.handle * 2;
        sizeHint.h += p.size.handle * 2;
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

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            const Box2I& g = getGeometry();
            const Box2I g2 = ftk::margin(g, -p.size.handle);
            p.draw->g = ftk::margin(g2, -p.size.border);
            p.draw->border = border(g2, p.size.border);
        }

        if (p.mouse.resize != MDIResize::None)
        {
            const auto i = p.mouse.resizeBoxes.find(p.mouse.resize);
            if (i != p.mouse.resizeBoxes.end())
            {
                event.render->drawRect(
                    i->second,
                    event.style->getColorRole(ColorRole::Checked));
            }
        }
        event.render->drawMesh(
            p.draw->border,
            event.style->getColorRole(ColorRole::Border));
        event.render->drawRect(
            p.draw->g,
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
            MDIResize resize = MDIResize::None;
            for (const auto& box : p.mouse.resizeBoxes)
            {
                if (contains(box.second, event.pos))
                {
                    resize = box.first;
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

    Box2I MDIWidget::_addMargins(const Box2I& value) const
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
    }
}
