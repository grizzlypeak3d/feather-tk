// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "DragDrop.h"

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/RowLayout.h>

#include <ftk/Core/Format.h>

using namespace ftk;

namespace widgets
{
    DragDropData::DragDropData(const std::shared_ptr<DragWidget>& widget) :
        _widget(widget)
    {}

    DragDropData::~DragDropData()
    {}

    const std::shared_ptr<DragWidget>& DragDropData::getWidget() const
    {
        return _widget;
    }

    void DragWidget::_init(
        const std::shared_ptr<Context>& context,
        int number,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::examples::DragWidget", parent);

        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);

        _number = number;

        _label = Label::create(context, Format("{0}").arg(number, 4, '0'), shared_from_this());
        _label->setMarginRole(SizeRole::MarginSmall);
        _label->setHAlign(HAlign::Center);
    }

    DragWidget::~DragWidget()
    {}

    std::shared_ptr<DragWidget> DragWidget::create(
        const std::shared_ptr<Context>& context,
        int number,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DragWidget>(new DragWidget);
        out->_init(context, number, parent);
        return out;
    }

    void DragWidget::setGeometry(const Box2I& value)
    {
        IMouseWidget::setGeometry(value);
        _label->setGeometry(getGeometry());
    }

    void DragWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        _dragLength = event.style->getSizeRole(SizeRole::DragLength, event.displayScale);
        setSizeHint(_label->getSizeHint());
    }

    void DragWidget::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IMouseWidget::drawEvent(drawRect, event);
        if (_isMouseInside())
        {
            const Box2I& g = getGeometry();
            event.render->drawRect(
                g,
                event.style->getColorRole(ColorRole::Hover));
        }
    }

    void DragWidget::mouseEnterEvent(MouseEnterEvent& event)
    {
        IMouseWidget::mouseEnterEvent(event);
        setDrawUpdate();
    }

    void DragWidget::mouseLeaveEvent()
    {
        IMouseWidget::mouseLeaveEvent();
        setDrawUpdate();
    }

    void DragWidget::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        if (_isMousePressed())
        {
            const float length = ftk::length(event.pos - _getMousePressPos());
            if (length > _dragLength)
            {
                event.dragDropData = std::make_shared<DragDropData>(
                    std::dynamic_pointer_cast<DragWidget>(shared_from_this()));
                const Box2I& g = getGeometry();
                const int w = g.w();
                const int h = g.h();
                event.dragDropCursor = Image::create(w, h, ImageType::RGBA_U8);
                uint8_t* p = event.dragDropCursor->getData();
                for (int y = 0; y < h; ++y)
                {
                    for (int x = 0; x < w; ++x)
                    {
                        p[0] = 255;
                        p[1] = 255;
                        p[2] = 255;
                        p[3] = 63;
                        p += 4;
                    }
                }
                event.dragDropCursorHotspot = _getMousePos() - g.min;
            }
        }
    }

    void ContainerWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::examples::ContainerWidget", parent);

        _layout = VerticalLayout::create(context, shared_from_this());
        _layout->setSpacingRole(SizeRole::None);
    }

    ContainerWidget::~ContainerWidget()
    {}

    std::shared_ptr<ContainerWidget> ContainerWidget::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ContainerWidget>(new ContainerWidget);
        out->_init(context, parent);
        return out;
    }

    void ContainerWidget::addWidget(const std::shared_ptr<DragWidget>& value)
    {
        value->setParent(_layout);
    }

    void ContainerWidget::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _layout->setGeometry(getGeometry());
    }

    void ContainerWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        _handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);
        setSizeHint(_layout->getSizeHint());
    }

    void ContainerWidget::drawOverlayEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IWidget::drawOverlayEvent(drawRect, event);
        if (_dropTarget != -1)
        {
            const Box2I g = _getDropGeom(_dropTarget);
            event.render->drawRect(
                g,
                event.style->getColorRole(ColorRole::Checked));
        }
    }

    void ContainerWidget::dragEnterEvent(DragDropEvent& event)
    {
        event.accept = true;
        _dropTarget = _getDropIndex(event.pos);
        setDrawUpdate();
    }

    void ContainerWidget::dragLeaveEvent(DragDropEvent& event)
    {
        event.accept = true;
        _dropTarget = -1;
        setDrawUpdate();
    }

    void ContainerWidget::dragMoveEvent(DragDropEvent& event)
    {
        event.accept = true;
        const int dropTarget = _getDropIndex(event.pos);
        if (dropTarget != _dropTarget)
        {
            _dropTarget = dropTarget;
            setDrawUpdate();
        }
    }

    void ContainerWidget::dropEvent(DragDropEvent& event)
    {
        if (auto data = std::dynamic_pointer_cast<DragDropData>(event.data))
        {
            event.accept = true;
            auto widget = data->getWidget();
            if (widget->getParent() != _layout)
            {
                widget->setParent(_layout);
            }
            else
            {
                const int index = _layout->getChildIndex(widget);
                if (index < _dropTarget)
                {
                    --_dropTarget;
                }
            }
            _layout->moveToIndex(widget, _dropTarget);
            _dropTarget = -1;
            setDrawUpdate();
        }
    }

    int ContainerWidget::_getDropIndex(const V2I& pos) const
    {
        int out = 0;
        for (const auto& child : _layout->getChildren())
        {
            if (pos.y < center(child->getGeometry()).y)
            {
                break;
            }
            ++out;
        }
        return out;
    }

    Box2I ContainerWidget::_getDropGeom(int index) const
    {
        Box2I out;
        const auto& children = _layout->getChildren();
        int i = 0;
        for (const auto& child : children)
        {
            if (i == index)
            {
                const Box2I& g = child->getGeometry();
                out = Box2I(g.min.x, g.min.y - _handle / 2, g.w(), _handle);
                break;
            }
            ++i;
        }
        if (i == children.size())
        {
            const Box2I& g = children.back()->getGeometry();
            out = Box2I(g.min.x, g.min.y + g.h() - _handle / 2, g.w(), _handle);
        }
        return out;
    }

    void DragDrop::_init(const std::shared_ptr<Context>& context)
    {
        ftk::IWidget::_init(context, "DragDrop", nullptr);

        // Create a layout.
        _layout = HorizontalLayout::create(context, shared_from_this());
        _layout->setSpacingRole(SizeRole::None);
        _layout->setStretch(Stretch::Expanding);
        auto scrollWidget0 = ScrollWidget::create(context, ScrollType::Vertical, _layout);
        scrollWidget0->setBorder(false);
        scrollWidget0->setStretch(Stretch::Expanding);
        auto scrollWidget1 = ScrollWidget::create(context, ScrollType::Vertical, _layout);
        scrollWidget1->setBorder(false);
        scrollWidget1->setStretch(Stretch::Expanding);

        // Create the drag and drop widgets.
        auto dragDropContainer0 = ContainerWidget::create(context);
        scrollWidget0->setWidget(dragDropContainer0);
        auto dragDropContainer1 = ContainerWidget::create(context);
        scrollWidget1->setWidget(dragDropContainer1);
        for (int i = 0; i < 100; ++i)
        {
            dragDropContainer0->addWidget(DragWidget::create(context, i));
            dragDropContainer1->addWidget(DragWidget::create(context, i));
        }
    }

    DragDrop::~DragDrop()
    {}

    std::shared_ptr<DragDrop> DragDrop::create(const std::shared_ptr<Context>& context)
    {
        auto out = std::shared_ptr<DragDrop>(new DragDrop);
        out->_init(context);
        return out;
    }

    void DragDrop::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _layout->setGeometry(value);
    }

    void DragDrop::sizeHintEvent(const SizeHintEvent& event)
    {
        setSizeHint(_layout->getSizeHint());
    }
}
