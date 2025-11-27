// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IncButtons.h>

#include <ftk/UI/DoubleModel.h>
#include <ftk/UI/FloatModel.h>
#include <ftk/UI/IntModel.h>

namespace ftk
{
    struct IncButton::Private
    {
        struct SizeData
        {
            int margin = 0;
        };
        SizeData size;
    };

    void IncButton::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IButton::_init(context, "ftk::IncButton", parent);
        setButtonRole(ColorRole::None);
        setRepeatClick(true);
    }

    IncButton::IncButton() :
        _p(new Private)
    {}

    IncButton::~IncButton()
    {}

    std::shared_ptr<IncButton> IncButton::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<IncButton>(new IncButton);
        out->_init(context, parent);
        return out;
    }

    void IncButton::sizeHintEvent(const SizeHintEvent& event)
    {
        IButton::sizeHintEvent(event);
        FTK_P();

        //p.size.margin = event.style->getSizeRole(SizeRole::MarginInside, _displayScale);

        Size2I sizeHint;
        if (_iconImage)
        {
            sizeHint.w = _iconImage->getWidth();
            sizeHint.h = _iconImage->getHeight();
        }
        sizeHint.w += p.size.margin * 2;
        sizeHint.h += p.size.margin * 2;
        setSizeHint(sizeHint);
    }

    void IncButton::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IButton::drawEvent(drawRect, event);
        FTK_P();

        const Box2I& g = getGeometry();

        const ColorRole colorRole = _checked ?
            ColorRole::Checked :
            _buttonRole;
        if (colorRole != ColorRole::None)
        {
            event.render->drawRect(
                g,
                event.style->getColorRole(colorRole));
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

        if (_iconImage)
        {
            const Size2I iconSize = _iconImage->getSize();
            event.render->drawImage(
                _iconImage,
                Box2I(
                    g.x() + p.size.margin,
                    g.y() + g.h() / 2 - iconSize.h / 2,
                    iconSize.w,
                    iconSize.h));
        }
    }

    void IncButtons::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::IncButtons", parent);
        _incButton = IncButton::create(context, shared_from_this());
        _incButton->setIcon("Increment");
        _incButton->setVAlign(VAlign::Top);
        _decButton = IncButton::create(context, shared_from_this());
        _decButton->setIcon("Decrement");
        _decButton->setVAlign(VAlign::Bottom);
    }

    IncButtons::IncButtons()
    {}

    IncButtons::~IncButtons()
    {}

    std::shared_ptr<IncButtons> IncButtons::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<IncButtons>(new IncButtons);
        out->_init(context, parent);
        return out;
    }

    void IncButtons::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _incButton->setGeometry(Box2I(
            value.min.x,
            value.min.y,
            value.w(),
            value.h() / 2));
        _decButton->setGeometry(Box2I(
            value.min.x,
            value.max.y - value.h() / 2,
            value.w(),
            value.h() / 2));
    }

    void IncButtons::sizeHintEvent(const SizeHintEvent& event)
    {
        const Size2I incSizeHint = _incButton->getSizeHint();
        const Size2I decSizeHint = _decButton->getSizeHint();
        setSizeHint(Size2I(
            std::max(incSizeHint.w, decSizeHint.w),
            incSizeHint.h + decSizeHint.h));
    }

    void IncButtons::setIncCallback(const std::function<void(void)>& value)
    {
        _incButton->setClickedCallback(value);
    }

    void IncButtons::setDecCallback(const std::function<void(void)>& value)
    {
        _decButton->setClickedCallback(value);
    }
}