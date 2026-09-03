// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IDialog.h>

#include <ftk/UI/Divider.h>
#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/IWindow.h>
#include <ftk/UI/Label.h>

#include <algorithm>
#include <optional>

namespace ftk
{
    struct IDialog::Private
    {
        bool open = false;
        std::function<void(void)> closeCallback;
        std::weak_ptr<IWidget> restoreFocus;

        std::string title;
        std::shared_ptr<Label> titleLabel;
        std::shared_ptr<Divider> titleDivider;

        //! The title bar and the content together, which is what the border
        //! and the shadow are drawn around.
        Box2I geom;

        struct SizeData
        {
            bool init = true;
            int margin = 0;
            int border = 0;
            int shadow = 0;
        };
        SizeData size;

        struct DrawData
        {
            Box2I g;
            Box2I g2;
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
        FTK_P();
        setBackgroundRole(ColorRole::Overlay);
        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);

        p.titleLabel = Label::create(context, shared_from_this());
        p.titleLabel->setFont(FontType::Bold);
        p.titleLabel->setMarginRole(SizeRole::MarginSmall);
        p.titleLabel->setBackgroundRole(ColorRole::Header);
        // The bar spans the dialog, which is the label's own background.
        p.titleLabel->setHAlign(HAlign::Fill);
        p.titleLabel->setVisible(false);

        p.titleDivider = Divider::create(context, Orientation::Vertical, shared_from_this());
        p.titleDivider->setVisible(false);
    }

    const std::string& IDialog::getTitle() const
    {
        return _p->title;
    }

    void IDialog::setTitle(const std::string& value)
    {
        FTK_P();
        if (value == p.title)
            return;
        p.title = value;
        p.titleLabel->setText(value);
        p.titleLabel->setVisible(!value.empty());
        p.titleDivider->setVisible(!value.empty());
        setSizeUpdate();
        setDrawUpdate();
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
        // Each open starts as the mouse left it: the shortcut that opened
        // the dialog is a command, not a navigation, and a ring inherited
        // from the previous visit reads as an inconsistency.
        window->hideKeyFocus();
        setParent(window);
        _takeKeyFocus();
    }

    void IDialog::_takeKeyFocus()
    {
        // Only what the dialog names for itself. Falling back to the first
        // widget that would take the focus lands it somewhere arbitrary --
        // a scrolling block of text, or whichever button happens to be
        // leftmost -- which reads as a default answer without being one.
        if (auto widget = getKeyFocus())
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

        // Only restore the key focus if this dialog still contains it, so we
        // don't steal the focus back from something opened on top of us.
        std::shared_ptr<IWidget> restoreFocus;
        if (containsKeyFocus())
        {
            restoreFocus = p.restoreFocus.lock();
        }
        p.restoreFocus.reset();

        setParent(nullptr);
        if (p.closeCallback)
        {
            p.closeCallback();
        }
        if (restoreFocus)
        {
            restoreFocus->takeKeyFocus();
        }
    }

    void IDialog::setGeometry(const Box2I& value)
    {
        IPopup::setGeometry(value);
        FTK_P();

        // The content is whichever child the subclass parented here; the
        // title bar is the dialog's own.
        std::shared_ptr<IWidget> content;
        for (const auto& child : getChildren())
        {
            if (child != p.titleLabel && child != p.titleDivider)
            {
                content = child;
                break;
            }
        }

        const Box2I g = margin(value, -p.size.margin);
        const bool title = p.titleLabel->isVisible(false);
        const int titleH = title ?
            (p.titleLabel->getSizeHint().h + p.titleDivider->getSizeHint().h) :
            0;

        Size2I size;
        if (content)
        {
            const Size2I sizeHint = content->getSizeHint();
            switch (content->getHStretch())
            {
            case Stretch::Expanding:
                size.w = g.w();
                break;
            case Stretch::Fixed:
            default:
                size.w = sizeHint.w;
                break;
            }
            switch (content->getVStretch())
            {
            case Stretch::Expanding:
                size.h = g.h() - titleH;
                break;
            case Stretch::Fixed:
            default:
                size.h = sizeHint.h;
                break;
            }
        }
        // A long title widens the dialog rather than being clipped.
        if (title)
        {
            size.w = std::max(size.w, p.titleLabel->getSizeHint().w);
        }

        const Box2I geom(
            g.x() + g.w() / 2 - size.w / 2,
            g.y() + g.h() / 2 - (size.h + titleH) / 2,
            size.w,
            size.h + titleH);

        int y = geom.min.y;
        if (title)
        {
            const int h = p.titleLabel->getSizeHint().h;
            p.titleLabel->setGeometry(Box2I(geom.min.x, y, geom.w(), h));
            y += h;
            const int dividerH = p.titleDivider->getSizeHint().h;
            p.titleDivider->setGeometry(Box2I(geom.min.x, y, geom.w(), dividerH));
            y += dividerH;
        }
        if (content)
        {
            content->setGeometry(Box2I(geom.min.x, y, geom.w(), size.h));
        }

        if (geom != p.geom)
        {
            p.geom = geom;
            p.draw.reset();
        }
    }

    void IDialog::styleEvent(const StyleEvent& event)
    {
        IPopup::styleEvent(event);
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
            p.draw.reset();
        }
    }

    void IDialog::sizeHintEvent(const SizeHintEvent& event)
    {
        IPopup::sizeHintEvent(event);
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
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
        else if (p.open)
        {
            // Opening runs before the first layout, and a widget that is
            // still clipped then loses the focus again the moment it is laid
            // out. Take it back once there is something laid out to give it
            // to -- but only if nothing holds it, so this never steals from
            // whatever the dialog put it on.
            if (auto window = getWindow())
            {
                if (!window->getKeyFocus())
                {
                    _takeKeyFocus();
                }
            }
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
            {
                p.draw = Private::DrawData();
                p.draw->g = p.geom;
                p.draw->g2 = Box2I(
                    p.draw->g.min.x - p.size.shadow,
                    p.draw->g.min.y,
                    p.draw->g.w() + p.size.shadow * 2,
                    p.draw->g.h() + p.size.shadow);
                p.draw->shadow = shadow(p.draw->g2, p.size.shadow);
                p.draw->border = border(margin(p.draw->g, p.size.border), p.size.border);
            }
        }

        if (p.draw->g.isValid())
        {
            event.render->drawColorMesh(p.draw->shadow);
            event.render->drawMesh(
                p.draw->border,
                event.style->getColorRole(ColorRole::Border));
            event.render->drawRect(
                p.draw->g,
                event.style->getColorRole(ColorRole::Window));
        }
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
