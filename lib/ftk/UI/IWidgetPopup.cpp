// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IWidgetPopup.h>

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/IWindow.h>

#include <optional>

namespace ftk
{
    struct IWidgetPopup::Private
    {
        ColorRole popupRole = ColorRole::Window;
        Box2I buttonGeometry;
        std::optional<Box2I> widgetGeometry;
        bool open = false;
        std::function<void(void)> closeCallback;
        std::shared_ptr<IWidget> widget;
        std::weak_ptr<IWidget> restoreFocus;

        struct SizeData
        {
            bool init = true;
            int border = 0;
            int shadow = 0;
        };
        SizeData size;

        struct DrawData
        {
            Box2I g;
            Box2I g2;
            Box2I g3;
            TriMesh2F shadow;
            TriMesh2F border;
        };
        std::optional<DrawData> draw;
    };

    void IWidgetPopup::_init(
        const std::shared_ptr<Context>& context,
        const std::string& objectName,
        const std::shared_ptr<IWidget>& parent)
    {
        IPopup::_init(context, objectName, parent);
    }

    IWidgetPopup::IWidgetPopup() :
        _p(new Private)
    {}

    IWidgetPopup::~IWidgetPopup()
    {}

    void IWidgetPopup::open(
        const std::shared_ptr<IWindow>& window,
        const Box2I& buttonGeometry,
        const std::optional<Box2I>& widgetGeometry)
    {
        FTK_P();
        p.buttonGeometry = buttonGeometry;
        p.widgetGeometry = widgetGeometry;
        p.open = true;
        p.restoreFocus = window->getKeyFocus();
        setParent(window);
        if (auto widget = window->getNextKeyFocus(shared_from_this()))
        {
            widget->takeKeyFocus();
        }
        else
        {
            window->setKeyFocus(nullptr);
        }
    }

    bool IWidgetPopup::isOpen() const
    {
        return _p->open;
    }

    void IWidgetPopup::close()
    {
        FTK_P();
        p.open = false;
        setParent(nullptr);
        auto widget = p.restoreFocus.lock();
        p.restoreFocus.reset();
        if (p.closeCallback)
        {
            p.closeCallback();
        }
        if (widget)
        {
            widget->takeKeyFocus();
        }
    }

    void IWidgetPopup::setCloseCallback(const std::function<void(void)>& value)
    {
        _p->closeCallback = value;
    }

    ColorRole IWidgetPopup::getPopupRole() const
    {
        return _p->popupRole;
    }

    void IWidgetPopup::setPopupRole(ColorRole value)
    {
        FTK_P();
        if (value == p.popupRole)
            return;
        p.popupRole = value;
        setDrawUpdate();
    }

    const std::shared_ptr<IWidget>& IWidgetPopup::getWidget() const
    {
        return _p->widget;
    }

    void IWidgetPopup::setWidget(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        if (p.widget)
        {
            p.widget->setParent(nullptr);
        }
        p.widget = value;
        if (p.widget)
        {
            p.widget->setParent(shared_from_this());
        }
    }

    void IWidgetPopup::setGeometry(const Box2I& value)
    {
        IPopup::setGeometry(value);
        FTK_P();
        bool changed = false;
        if (p.widget)
        {
            const Box2I popupGeometry =
                p.widgetGeometry.has_value() ?
                p.widgetGeometry.value() :
                p.buttonGeometry;
            Size2I sizeHint = p.widget->getSizeHint();
            std::list<Box2I> boxes;
            boxes.push_back(Box2I(
                popupGeometry.min.x,
                popupGeometry.max.y + 1,
                sizeHint.w,
                sizeHint.h));
            boxes.push_back(Box2I(
                popupGeometry.max.x + 1 - sizeHint.w,
                popupGeometry.max.y + 1,
                sizeHint.w,
                sizeHint.h));
            boxes.push_back(Box2I(
                popupGeometry.min.x,
                popupGeometry.min.y - sizeHint.h,
                sizeHint.w,
                sizeHint.h));
            boxes.push_back(Box2I(
                popupGeometry.max.x + 1 - sizeHint.w,
                popupGeometry.min.y - sizeHint.h,
                sizeHint.w,
                sizeHint.h));
            struct Intersect
            {
                Box2I original;
                Box2I intersected;
            };
            std::vector<Intersect> intersect;
            for (const auto& box : boxes)
            {
                intersect.push_back({ box, ftk::intersect(box, value) });
            }
            std::stable_sort(
                intersect.begin(),
                intersect.end(),
                [](const Intersect& a, const Intersect& b)
                {
                    return
                        area(a.intersected.size()) >
                        area(b.intersected.size());
                });
            const Box2I g = intersect.front().intersected;
            changed = g != p.widget->getGeometry();
            p.widget->setGeometry(g);
        }
        if (changed)
        {
            p.draw.reset();
        }
    }

    void IWidgetPopup::styleEvent(const StyleEvent& event)
    {
        IPopup::styleEvent(event);
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
            p.draw.reset();
        }
    }

    void IWidgetPopup::sizeHintEvent(const SizeHintEvent& event)
    {
        IPopup::sizeHintEvent(event);
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.shadow = event.style->getSizeRole(SizeRole::Shadow, event.displayScale);
            p.draw.reset();
        }
    }

    void IWidgetPopup::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void IWidgetPopup::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IPopup::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            if (p.widget)
            {
                p.draw->g = p.widget->getGeometry();
                p.draw->g2 = margin(p.draw->g, p.size.border);
                p.draw->g3 = Box2I(
                    p.draw->g.min.x - p.size.shadow,
                    p.draw->g.min.y,
                    p.draw->g.w() + p.size.shadow * 2,
                    p.draw->g.h() + p.size.shadow);
                p.draw->shadow = shadow(p.draw->g3, p.size.shadow);
                p.draw->border = border(p.draw->g2, p.size.border);
            }
        }

        if (p.widget)
        {
            event.render->drawColorMesh(p.draw->shadow);
            event.render->drawMesh(
                p.draw->border,
                event.style->getColorRole(ColorRole::Border));
            event.render->drawRect(
                p.draw->g,
                event.style->getColorRole(p.popupRole));
        }
    }

    void IWidgetPopup::mouseMoveEvent(MouseMoveEvent& event)
    {
        IPopup::mouseMoveEvent(event);
        FTK_P();
        if (p.widget && contains(p.widget->getGeometry(), event.pos))
        {
            event.accept = true;
        }
    }

    void IWidgetPopup::mousePressEvent(MouseClickEvent& event)
    {
        IPopup::mousePressEvent(event);
        FTK_P();
        if (p.widget)
        {
            if (contains(p.widget->getGeometry(), event.pos))
            {
                event.accept = true;
            }
            else if (!contains(p.buttonGeometry, event.pos))
            {
                close();
            }
        }
    }

    void IWidgetPopup::mouseReleaseEvent(MouseClickEvent& event)
    {
        IPopup::mouseReleaseEvent(event);
        FTK_P();
        event.accept = true;
    }

    void IWidgetPopup::scrollEvent(ScrollEvent& event)
    {
        IPopup::scrollEvent(event);
        FTK_P();
        if (p.widget)
        {
            if (contains(p.widget->getGeometry(), event.pos))
            {
                event.accept = true;
            }
            else
            {
                close();
            }
        }
    }

    void IWidgetPopup::keyPressEvent(KeyEvent& event)
    {
        if (0 == event.modifiers)
        {
            if (Key::Escape == event.key)
            {
                event.accept = true;
                close();
            }
        }
        if (!event.accept)
        {
            IPopup::keyPressEvent(event);
        }
    }

    void IWidgetPopup::keyReleaseEvent(KeyEvent& event)
    {
        IPopup::keyReleaseEvent(event);
        event.accept = true;
    }
}