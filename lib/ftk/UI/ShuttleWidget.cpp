// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the DJV project.

#include <ftk/UI/ShuttleWidget.h>

#include <ftk/Core/Format.h>

#include <optional>

namespace ftk
{
    struct ShuttleWidget::Private
    {
        std::vector<std::shared_ptr<Image> > iconImages;
        int iconIndex = 0;
        std::function<void(bool)> activeCallback;
        std::function<void(int)> callback;

        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
            float iconScale = 1.F;
            Size2I sizeHint;
        };
        SizeData size;
    };

    void ShuttleWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "tl::ui::ShuttleWidget", parent);
        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);
    }

    ShuttleWidget::ShuttleWidget() :
        _p(new Private)
    {
    }

    ShuttleWidget::~ShuttleWidget()
    {
    }

    std::shared_ptr<ShuttleWidget> ShuttleWidget::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ShuttleWidget>(new ShuttleWidget);
        out->_init(context, parent);
        return out;
    }

    void ShuttleWidget::setActiveCallback(const std::function<void(bool)>& value)
    {
        _p->activeCallback = value;
    }

    void ShuttleWidget::setCallback(const std::function<void(int)>& value)
    {
        _p->callback = value;
    }

    Size2I ShuttleWidget::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void ShuttleWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(SizeRole::MarginInside, event.displayScale);
            p.size.iconScale = event.displayScale;
            p.iconImages.clear();
            for (int i = 0; i < 8; ++i)
            {
                p.iconImages.push_back(event.iconSystem->get(
                    Format("Shuttle{0}").
                    arg(i),
                    event.displayScale));
            }
            p.size.sizeHint = Size2I();
            if (!p.iconImages.empty())
            {
                p.size.sizeHint = p.iconImages.front()->getSize();
            }
            p.size.sizeHint = margin(p.size.sizeHint, p.size.margin);
        }
    }

    void ShuttleWidget::drawEvent(const Box2I& drawRect, const DrawEvent& event)
    {
        IMouseWidget::drawEvent(drawRect, event);
        FTK_P();
        const Box2I& g = getGeometry();
        if (p.iconIndex >= 0 && p.iconIndex < p.iconImages.size())
        {
            const Size2I& iconSize = p.iconImages.front()->getSize();
            const Box2I g2(
                g.min.x + g.w() / 2 - iconSize.w / 2,
                g.min.y + g.h() / 2 - iconSize.h / 2,
                iconSize.w,
                iconSize.h);
            event.render->drawImage(
                p.iconImages[p.iconIndex],
                g2,
                event.style->getColorRole(isEnabled() ?
                    ColorRole::Text :
                    ColorRole::TextDisabled));
        }
        if (_isMousePressed())
        {
            event.render->drawRect(
                g,
                event.style->getColorRole(ColorRole::Pressed));
        }
        else if (_isMouseInside())
        {
            event.render->drawRect(
                g,
                event.style->getColorRole(ColorRole::Hover));
        }
    }

    void ShuttleWidget::mouseEnterEvent(MouseEnterEvent& event)
    {
        IMouseWidget::mouseEnterEvent(event);
        setDrawUpdate();
    }

    void ShuttleWidget::mouseLeaveEvent()
    {
        IMouseWidget::mouseLeaveEvent();
        setDrawUpdate();
    }

    void ShuttleWidget::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        FTK_P();
        const Box2I& g = getGeometry();
        if (_isMousePressed() && g.isValid())
        {
            const int d = event.pos.x - _getMousePressPos().x;
            const int i = d / (g.h() / 4);
            if (i != p.iconIndex)
            {
                p.iconIndex = i % p.iconImages.size();
                setDrawUpdate();
                if (p.callback)
                {
                    p.callback(i);
                }
            }
        }
    }

    void ShuttleWidget::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        setDrawUpdate();
        if (p.activeCallback)
        {
            p.activeCallback(true);
        }
    }

    void ShuttleWidget::mouseReleaseEvent(MouseClickEvent& event)
    {
        IMouseWidget::mouseReleaseEvent(event);
        FTK_P();
        p.iconIndex = 0;
        setDrawUpdate();
        if (p.activeCallback)
        {
            p.activeCallback(false);
        }
    }
}