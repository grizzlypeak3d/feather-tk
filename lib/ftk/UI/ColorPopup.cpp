// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ColorPopup.h>

#include <ftk/UI/ColorWidget.h>
#include <ftk/UI/RowLayout.h>

namespace ftk
{
    struct ColorPopup::Private
    {
        std::shared_ptr<ColorWidget> widget;
        std::shared_ptr<VerticalLayout> layout;
    };

    void ColorPopup::_init(
        const std::shared_ptr<Context>& context,
        const Color4F& color,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidgetPopup::_init(context, "ftk::ColorPopup", parent);
        FTK_P();

        p.widget = ColorWidget::create(context);
        p.widget->setColor(color);

        p.layout = VerticalLayout::create(context);
        p.layout->setMarginRole(SizeRole::MarginSmall);
        p.widget->setParent(p.layout);
        setWidget(p.layout);
    }

    ColorPopup::ColorPopup() :
        _p(new Private)
    {}

    ColorPopup::~ColorPopup()
    {}

    std::shared_ptr<ColorPopup> ColorPopup::create(
        const std::shared_ptr<Context>& context,
        const Color4F& color,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ColorPopup>(new ColorPopup);
        out->_init(context, color, parent);
        return out;
    }

    void ColorPopup::setCallback(const std::function<void(const Color4F&)>& value)
    {
        _p->widget->setCallback(value);
    }

    void ColorPopup::setPressedCallback(const std::function<void(const Color4F&, bool)>& value)
    {
        _p->widget->setPressedCallback(value);
    }
}