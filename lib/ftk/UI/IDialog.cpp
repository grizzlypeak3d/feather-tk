// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IDialog.h>

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/IWindow.h>

#include <optional>

namespace ftk
{
    struct IDialog::Private
    {
        bool open = false;
        std::function<void(void)> closeCallback;
        std::weak_ptr<IWidget> restoreFocus;

        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
            int border = 0;
            int shadow = 0;
            Box2I g;
            Box2I g2;
        };
        SizeData size;

        struct DrawData
        {
            TriMesh2F shadow;
            TriMesh2F border;
        };
        std::optional<DrawData> draw;
    };

    void IDialog::_init(
        const std::shared_ptr<Context>& context,
        const std::string& objectName,
        const std::shared_ptr<IWidget>& parent)
    {
        IPopup::_init(context, objectName, parent);
        setBackgroundRole(ColorRole::Overlay);
        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);
    }

    IDialog::IDialog() :
        _p(new Private)
    {}

    IDialog::~IDialog()
    {}

    void IDialog::open(const std::shared_ptr<IWindow>& window)
    {
        FTK_P();
        p.open = true;
        p.restoreFocus = window->getKeyFocus();
        window->setKeyFocus(nullptr);
        setParent(window);
        if (auto widget = getKeyFocus())
        {
            widget->takeKeyFocus();
        }
        else if ((widget = window->getNextKeyFocus(shared_from_this())))
        {
            widget->takeKeyFocus();
        }
    }

    bool IDialog::isOpen() const
    {
        return _p->open;
    }

    void IDialog::setCloseCallback(const std::function<void(void)>& value)
    {
        _p->closeCallback = value;
    }

    std::shared_ptr<IWidget> IDialog::getKeyFocus() const
    {
        return nullptr;
    }

    void IDialog::close()
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

    void IDialog::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IPopup::setGeometry(value);
        FTK_P();

        const auto& children = getChildren();
        if (!children.empty())
        {
            const Box2I g = margin(value, -p.size.margin);
            const Size2I sizeHint = children.front()->getSizeHint();
            Size2I size;
            switch (children.front()->getHStretch())
            {
            case Stretch::Expanding:
                size.w = g.w();
                break;
            case Stretch::Fixed:
            default:
                size.w = sizeHint.w;
                break;
            }
            switch (children.front()->getVStretch())
            {
            case Stretch::Expanding:
                size.h = g.h();
                break;
            case Stretch::Fixed:
            default:
                size.h = sizeHint.h;
                break;
            }

            const Box2I g2(
                g.x() + g.w() / 2 - size.w / 2,
                g.y() + g.h() / 2 - size.h / 2,
                size.w,
                size.h);
            if (g2 != children.front()->getGeometry())
            {
                p.draw.reset();
            }
            children.front()->setGeometry(g2);
        }

        if (changed)
        {
            if (!children.empty())
            {
                p.size.g = children.front()->getGeometry();
                p.size.g2 = Box2I(
                    p.size.g.min.x - p.size.shadow,
                    p.size.g.min.y,
                    p.size.g.w() + p.size.shadow * 2,
                    p.size.g.h() + p.size.shadow);
            }
            else
            {
                p.size.g = Box2I();
                p.size.g2 = Box2I();
            }
            p.draw.reset();
        }
    }

    void IDialog::sizeHintEvent(const SizeHintEvent& event)
    {
        IPopup::sizeHintEvent(event);
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(SizeRole::MarginDialog, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.shadow = event.style->getSizeRole(SizeRole::Shadow, event.displayScale);
            p.draw.reset();
        }
    }

    void IDialog::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void IDialog::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IPopup::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            const auto& children = getChildren();
            if (!children.empty())
            {
                p.draw = Private::DrawData();
                p.draw->shadow = shadow(p.size.g2, p.size.shadow);
                p.draw->border = border(margin(p.size.g, p.size.border), p.size.border);
            }
            else
            {
                p.draw->shadow = TriMesh2F();
                p.draw->border = TriMesh2F();
            }
        }

        if (p.size.g.isValid())
        {
            event.render->drawColorMesh(p.draw->shadow);
            event.render->drawMesh(
                p.draw->border,
                event.style->getColorRole(ColorRole::Border));
            event.render->drawRect(
                p.size.g,
                event.style->getColorRole(ColorRole::Window));
        }
    }

    void IDialog::scrollEvent(ScrollEvent& event)
    {
        event.accept = true;
    }

    void IDialog::keyPressEvent(KeyEvent& event)
    {
        event.accept = event.key != Key::Tab;
        if (Key::Escape == event.key && 0 == event.modifiers)
        {
            close();
        }
    }

    void IDialog::keyReleaseEvent(KeyEvent& event)
    {
        event.accept = true;
    }
}
