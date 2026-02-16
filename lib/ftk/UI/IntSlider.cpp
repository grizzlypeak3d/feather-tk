// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IntSlider.h>

#include <ftk/UI/DrawUtil.h>

#include <optional>

namespace ftk
{
    struct IntSlider::Private
    {
        std::shared_ptr<IntModel> model;
        std::function<void(int)> callback;
        std::function<void(int, bool)> pressedCallback;
        bool blockCallbacks = false;
        std::shared_ptr<Observer<int> > valueObserver;
        std::shared_ptr<Observer<RangeI> > rangeObserver;

        struct SizeData
        {
            std::optional<float> displayScale;
            int size = 0;
            int border = 0;
            int keyFocus = 0;
            int handle = 0;
            Size2I sizeHint;
            Box2I g;
            Box2I g2;
            Box2I g3;
        };
        SizeData size;

        struct DrawData
        {
            TriMesh2F border;
            TriMesh2F keyFocus;
        };
        std::optional<DrawData> draw;
    };

    void IntSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::IntSlider", parent);
        FTK_P();

        setAcceptsKeyFocus(true);
        setHStretch(Stretch::Expanding);
        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);

        p.model = model;

        p.valueObserver = Observer<int>::create(
            p.model->observeValue(),
            [this](int value)
            {
                FTK_P();
                setSizeUpdate();
                setDrawUpdate();
                if (p.callback && !p.blockCallbacks)
                {
                    p.callback(value);
                }
                if (p.pressedCallback && !p.blockCallbacks)
                {
                    p.pressedCallback(value, _isMousePressed());
                }
            });

        p.rangeObserver = Observer<RangeI>::create(
            p.model->observeRange(),
            [this](const RangeI&)
            {
                setSizeUpdate();
                setDrawUpdate();
            });
    }

    IntSlider::IntSlider() :
        _p(new Private)
    {}

    IntSlider::~IntSlider()
    {}

    std::shared_ptr<IntSlider> IntSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<IntSlider>(new IntSlider);
        out->_init(context, IntModel::create(context), parent);
        return out;
    }

    std::shared_ptr<IntSlider> IntSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<IntSlider>(new IntSlider);
        out->_init(context, model, parent);
        return out;
    }

    int IntSlider::getValue() const
    {
        return _p->model->getValue();
    }

    void IntSlider::setValue(int value)
    {
        FTK_P();
        p.blockCallbacks = true;
        p.model->setValue(value);
        p.blockCallbacks = false;
    }

    void IntSlider::setCallback(const std::function<void(int)>& value)
    {
        _p->callback = value;
    }

    void IntSlider::setPressedCallback(const std::function<void(int, bool)>& value)
    {
        _p->pressedCallback = value;
    }

    const RangeI& IntSlider::getRange() const
    {
        return _p->model->getRange();
    }

    void IntSlider::setRange(const RangeI& value)
    {
        FTK_P();
        p.blockCallbacks = true;
        p.model->setRange(value);
        p.blockCallbacks = false;
    }

    void IntSlider::setRange(int min, int max)
    {
        setRange(RangeI(min, max));
    }

    int IntSlider::getStep() const
    {
        return _p->model->getStep();
    }

    void IntSlider::setStep(int value)
    {
        _p->model->setStep(value);
    }

    int IntSlider::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void IntSlider::setLargeStep(int value)
    {
        _p->model->setLargeStep(value);
    }

    int IntSlider::getDefaultValue() const
    {
        return _p->model->getDefaultValue();
    }

    void IntSlider::setDefaultValue(int value)
    {
        _p->model->setDefaultValue(value);
    }

    const std::shared_ptr<IntModel>& IntSlider::getModel() const
    {
        return _p->model;
    }

    Size2I IntSlider::getSizeHint() const
    {
        return _p->size.sizeHint;
    }
    
    void IntSlider::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IMouseWidget::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.size.g = value;
            p.size.g2 = margin(p.size.g, -p.size.keyFocus);
            p.size.g3 = margin(p.size.g2, -p.size.handle / 2, 0, -p.size.handle / 2, 0);
            p.draw.reset();
        }
    }

    void IntSlider::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.size = event.style->getSizeRole(SizeRole::Slider, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);

            const auto fontInfo = event.style->getFontRole(FontRole::Label, event.displayScale);
            p.size.sizeHint = Size2I(p.size.size, event.fontSystem->getMetrics(fontInfo).lineHeight);
            p.size.sizeHint = margin(p.size.sizeHint, p.size.keyFocus);

            p.draw.reset();
        }
    }

    void IntSlider::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IMouseWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void IntSlider::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IMouseWidget::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->border = border(p.size.g, p.size.border);
            p.draw->keyFocus = border(p.size.g, p.size.keyFocus);
        }

        // Draw the background.
        event.render->drawRect(
            p.size.g,
            event.style->getColorRole(ColorRole::Base));

        // Draw the focus and border.
        const bool keyFocus = hasKeyFocus();
        event.render->drawMesh(
            keyFocus ? p.draw->keyFocus : p.draw->border,
            event.style->getColorRole(keyFocus ? ColorRole::KeyFocus : ColorRole::Border));

        // Draw the handle.
        const int pos = _valueToPos(p.model->getValue());
        const Box2I handle(
            pos - p.size.handle / 2,
            p.size.g3.min.y,
            p.size.handle,
            p.size.g3.h());
        event.render->drawRect(
            handle,
            event.style->getColorRole(ColorRole::Button));
        if (_isMousePressed())
        {
            event.render->drawRect(
                handle,
                event.style->getColorRole(ColorRole::Pressed));
        }
        else if (_isMouseInside())
        {
            event.render->drawRect(
                handle,
                event.style->getColorRole(ColorRole::Hover));
        }
        event.render->drawMesh(
            border(handle, p.size.border),
            event.style->getColorRole(ColorRole::Border));
    }

    void IntSlider::mouseEnterEvent(MouseEnterEvent& event)
    {
        IMouseWidget::mouseEnterEvent(event);
        setDrawUpdate();
    }

    void IntSlider::mouseLeaveEvent()
    {
        IMouseWidget::mouseLeaveEvent();
        setDrawUpdate();
    }

    void IntSlider::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        FTK_P();
        if (_isMousePressed())
        {
            p.model->setValue(_posToValue(_getMousePos().x));
        }
    }

    void IntSlider::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        takeKeyFocus();
        p.model->setValue(_posToValue(_getMousePos().x));
        setDrawUpdate();
    }

    void IntSlider::mouseReleaseEvent(MouseClickEvent& event)
    {
        IMouseWidget::mouseReleaseEvent(event);
        FTK_P();
        p.model->setValue(_posToValue(_getMousePos().x));
        if (p.pressedCallback)
        {
            p.pressedCallback(p.model->getValue(), false);
        }
        setDrawUpdate();
    }

    void IntSlider::scrollEvent(ScrollEvent& event)
    {
        FTK_P();
        if (hasKeyFocus())
        {
            event.accept = true;
            const int step = event.modifiers & static_cast<int>(KeyModifier::Shift) ?
                p.model->getLargeStep() :
                p.model->getStep();
            p.model->setValue(p.model->getValue() + step * event.value.y);
        }
    }

    void IntSlider::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        if (isEnabled() && 0 == event.modifiers)
        {
            switch (event.key)
            {
            case Key::Left:
            case Key::Down:
                event.accept = true;
                p.model->decrementStep();
                break;
            case Key::Right:
            case Key::Up:
                event.accept = true;
                p.model->incrementStep();
                break;
            case Key::PageUp:
                event.accept = true;
                p.model->incrementLargeStep();
                break;
            case Key::PageDown:
                event.accept = true;
                p.model->decrementLargeStep();
                break;
            case Key::Home:
                event.accept = true;
                p.model->setValue(p.model->getRange().min());
                break;
            case Key::End:
                event.accept = true;
                p.model->setValue(p.model->getRange().max());
                break;
            case Key::Escape:
                if (hasKeyFocus())
                {
                    event.accept = true;
                    releaseKeyFocus();
                }
                break;
            default: break;
            }
        }
        if (!event.accept)
        {
            IMouseWidget::keyPressEvent(event);
        }
    }

    void IntSlider::keyReleaseEvent(KeyEvent& event)
    {
        IMouseWidget::keyReleaseEvent(event);
        event.accept = true;
    }

    int IntSlider::_posToValue(int pos) const
    {
        FTK_P();
        const RangeI& range = p.model->getRange();
        const float inc = p.size.g3.w() / static_cast<float>(range.max() - range.min());
        const float v = (pos + inc / 2 - p.size.g3.min.x) / static_cast<float>(p.size.g3.w());
        const int out = range.min() + (range.max() - range.min()) * v;
        return out;
    }

    int IntSlider::_valueToPos(int value) const
    {
        FTK_P();
        const RangeI& range = p.model->getRange();
        float v = 0.F;
        if (range.min() != range.max())
        {
            v = (value - range.min()) /
                static_cast<float>(range.max() - range.min());
        }
        return p.size.g3.min.x + p.size.g3.w() * v;
    }
}