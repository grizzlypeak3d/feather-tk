// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IMenuPopup.h>

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/IMouseWidget.h>
#include <ftk/UI/IWindow.h>
#include <ftk/UI/ScrollWidget.h>

#include <optional>

namespace ftk
{
    namespace
    {
        class MenuPopupWidget : public IMouseWidget
        {
        protected:
            void _init(
                const std::shared_ptr<Context>&,
                const std::shared_ptr<IWidget>& parent = nullptr);

            MenuPopupWidget();

        public:
            virtual ~MenuPopupWidget();

            static std::shared_ptr<MenuPopupWidget> create(
                const std::shared_ptr<Context>&,
                const std::shared_ptr<IWidget>& parent = nullptr);

            Size2I getSizeHint() const override;
            void setGeometry(const Box2I&) override;
        };

        void MenuPopupWidget::_init(
            const std::shared_ptr<Context>& context,
            const std::shared_ptr<IWidget>& parent)
        {
            IMouseWidget::_init(context, "ftk::MenuPopupWidget", parent);
            _setMouseHoverEnabled(true);
            _setMousePressEnabled(true);
        }

        MenuPopupWidget::MenuPopupWidget()
        {}

        MenuPopupWidget::~MenuPopupWidget()
        {}

        std::shared_ptr<MenuPopupWidget> MenuPopupWidget::create(
            const std::shared_ptr<Context>& context,
            const std::shared_ptr<IWidget>& parent)
        {
            auto out = std::shared_ptr<MenuPopupWidget>(new MenuPopupWidget);
            out->_init(context, parent);
            return out;
        }

        Size2I MenuPopupWidget::getSizeHint() const
        {
            const auto& children = getChildren();
            return !children.empty() ? children.front()->getSizeHint() : Size2I();
        }

        void MenuPopupWidget::setGeometry(const Box2I& value)
        {
            IMouseWidget::setGeometry(value);
            const auto& children = getChildren();
            if (!children.empty())
            {
                children.front()->setGeometry(value);
            }
        }
    }

    struct IMenuPopup::Private
    {
        MenuPopup popup = MenuPopup::Menu;
        ColorRole popupRole = ColorRole::Window;
        Box2I buttonGeometry;
        bool open = false;
        std::function<void(void)> closeCallback;
        std::shared_ptr<IWidget> widget;
        std::shared_ptr<ScrollWidget> scrollWidget;
        std::shared_ptr<MenuPopupWidget> menuPopupWidget;
        std::weak_ptr<IWidget> restoreFocus;

        struct SizeData
        {
            std::optional<float> displayScale;
            int shadow = 0;
            Box2I g;
            Box2I g2;
        };
        SizeData size;

        struct DrawData
        {
            TriMesh2F shadow;
        };
        std::optional<DrawData> draw;
    };

    void IMenuPopup::_init(
        const std::shared_ptr<Context>& context,
        const std::string& objectName,
        const std::shared_ptr<IWidget>& parent)
    {
        IPopup::_init(context, objectName, parent);
        FTK_P();

        p.scrollWidget = ScrollWidget::create(context, ScrollType::Menu);

        p.menuPopupWidget = MenuPopupWidget::create(context, shared_from_this());
        p.scrollWidget->setParent(p.menuPopupWidget);
    }

    IMenuPopup::IMenuPopup() :
        _p(new Private)
    {}

    IMenuPopup::~IMenuPopup()
    {}

    void IMenuPopup::open(
        const std::shared_ptr<IWindow>& window,
        const Box2I& buttonGeometry)
    {
        FTK_P();
        p.buttonGeometry = buttonGeometry;
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

    bool IMenuPopup::isOpen() const
    {
        return _p->open;
    }

    void IMenuPopup::setCloseCallback(const std::function<void(void)>& value)
    {
        _p->closeCallback = value;
    }

    void IMenuPopup::setPopup(MenuPopup value)
    {
        FTK_P();
        p.popup = value;
    }

    void IMenuPopup::setPopupRole(ColorRole value)
    {
        FTK_P();
        if (value == p.popupRole)
            return;
        p.popupRole = value;
        setDrawUpdate();
    }

    void IMenuPopup::setWidget(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        p.widget = value;
        p.scrollWidget->setWidget(p.widget);
    }

    void IMenuPopup::close()
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

    void IMenuPopup::setGeometry(const Box2I& value)
    {
        IPopup::setGeometry(value);
        FTK_P();

        Size2I sizeHint = p.menuPopupWidget->getSizeHint();
        std::list<Box2I> boxes;
        switch (p.popup)
        {
        case MenuPopup::Menu:
            boxes.push_back(Box2I(
                p.buttonGeometry.min.x,
                p.buttonGeometry.max.y + 1,
                std::max(sizeHint.w, p.buttonGeometry.w()),
                sizeHint.h));
            boxes.push_back(Box2I(
                p.buttonGeometry.max.x + 1 - sizeHint.w,
                p.buttonGeometry.max.y + 1,
                std::max(sizeHint.w, p.buttonGeometry.w()),
                sizeHint.h));
            boxes.push_back(Box2I(
                p.buttonGeometry.min.x,
                p.buttonGeometry.min.y - sizeHint.h,
                std::max(sizeHint.w, p.buttonGeometry.w()),
                sizeHint.h));
            boxes.push_back(Box2I(
                p.buttonGeometry.max.x + 1 - sizeHint.w,
                p.buttonGeometry.min.y - sizeHint.h,
                std::max(sizeHint.w, p.buttonGeometry.w()),
                sizeHint.h));
            break;
        case MenuPopup::SubMenu:
            boxes.push_back(Box2I(
                p.buttonGeometry.max.x,
                p.buttonGeometry.min.y,
                sizeHint.w,
                sizeHint.h));
            boxes.push_back(Box2I(
                p.buttonGeometry.min.x - sizeHint.w,
                p.buttonGeometry.min.y,
                sizeHint.w,
                sizeHint.h));
            break;
        default: break;
        }
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
        const bool changed = g != p.menuPopupWidget->getGeometry();
        p.menuPopupWidget->setGeometry(g);

        if (changed)
        {
            p.size.g = g;
            p.size.g2 = Box2I(
                g.min.x - p.size.shadow,
                g.min.y,
                g.w() + p.size.shadow * 2,
                g.h() + p.size.shadow);
            p.draw.reset();
        }
    }

    void IMenuPopup::sizeHintEvent(const SizeHintEvent& event)
    {
        IPopup::sizeHintEvent(event);
        FTK_P();

        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.shadow = event.style->getSizeRole(SizeRole::Shadow, event.displayScale);
            p.draw.reset();
        }
    }

    void IMenuPopup::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void IMenuPopup::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IPopup::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->shadow = shadow(p.size.g2, p.size.shadow);
        }

        event.render->drawColorMesh(p.draw->shadow);
        event.render->drawRect(
            p.size.g,
            event.style->getColorRole(p.popupRole));
    }

    void IMenuPopup::mousePressEvent(MouseClickEvent& event)
    {
        IPopup::mousePressEvent(event);
        FTK_P();
        if (contains(p.buttonGeometry, event.pos))
        {
            event.accept = true;
            close();
        }
    }

    void IMenuPopup::mouseReleaseEvent(MouseClickEvent& event)
    {
        IPopup::mouseReleaseEvent(event);
        event.accept = true;
    }

    void IMenuPopup::keyPressEvent(KeyEvent& event)
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

    void IMenuPopup::keyReleaseEvent(KeyEvent& event)
    {
        IPopup::keyReleaseEvent(event);
        event.accept = true;
    }

    const std::shared_ptr<ScrollWidget>& IMenuPopup::_getScrollWidget() const
    {
        return _p->scrollWidget;
    }
}