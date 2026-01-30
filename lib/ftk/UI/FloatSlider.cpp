// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FloatSlider.h>

#include <ftk/UI/DrawUtil.h>

#include <optional>

namespace ftk
{
    struct FloatSlider::Private
    {
        std::shared_ptr<FloatModel> model;
        std::function<void(float)> callback;
        std::function<void(float, bool)> pressedCallback;
        bool blockCallbacks = false;
        std::shared_ptr<Observer<float> > valueObserver;
        std::shared_ptr<Observer<RangeF> > rangeObserver;

        struct SizeData
        {
            std::optional<float> displayScale;
            int size = 0;
            int margin = 0;
            int border = 0;
            int keyFocus = 0;
            int handle = 0;
            Size2I sizeHint;
        };
        SizeData size;

        struct DrawData
        {
            Box2I g;
            Box2I margin;
            TriMesh2F border;
            TriMesh2F keyFocus;
        };
        std::optional<DrawData> draw;
    };

    void FloatSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::FloatSlider", parent);
        FTK_P();

        setAcceptsKeyFocus(true);
        setHStretch(Stretch::Expanding);
        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);

        p.model = model;

        p.valueObserver = Observer<float>::create(
            p.model->observeValue(),
            [this](float value)
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

        p.rangeObserver = Observer<RangeF>::create(
            p.model->observeRange(),
            [this](const RangeF&)
            {
                setSizeUpdate();
                setDrawUpdate();
            });
    }

    FloatSlider::FloatSlider() :
        _p(new Private)
    {}

    FloatSlider::~FloatSlider()
    {}

    std::shared_ptr<FloatSlider> FloatSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FloatSlider>(new FloatSlider);
        out->_init(context, FloatModel::create(context), parent);
        return out;
    }

    std::shared_ptr<FloatSlider> FloatSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FloatSlider>(new FloatSlider);
        out->_init(context, model, parent);
        return out;
    }

    float FloatSlider::getValue() const
    {
        return _p->model->getValue();
    }

    void FloatSlider::setValue(float value)
    {
        FTK_P();
        p.blockCallbacks = true;
        p.model->setValue(value);
        p.blockCallbacks = false;
    }

    void FloatSlider::setCallback(const std::function<void(float)>& value)
    {
        _p->callback = value;
    }

    void FloatSlider::setPressedCallback(const std::function<void(float, bool)>& value)
    {
        _p->pressedCallback = value;
    }

    const RangeF& FloatSlider::getRange() const
    {
        return _p->model->getRange();
    }

    void FloatSlider::setRange(const RangeF& value)
    {
        FTK_P();
        p.blockCallbacks = true;
        p.model->setRange(value);
        p.blockCallbacks = false;
    }

    void FloatSlider::setRange(float min, float max)
    {
        setRange(RangeF(min, max));
    }

    float FloatSlider::getStep() const
    {
        return _p->model->getStep();
    }

    void FloatSlider::setStep(float value)
    {
        _p->model->setStep(value);
    }

    float FloatSlider::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void FloatSlider::setLargeStep(float value)
    {
        _p->model->setLargeStep(value);
    }

    float FloatSlider::getDefaultValue() const
    {
        return _p->model->getDefaultValue();
    }

    void FloatSlider::setDefaultValue(float value)
    {
        _p->model->setDefaultValue(value);
    }

    const std::shared_ptr<FloatModel>& FloatSlider::getModel() const
    {
        return _p->model;
    }
    
    Size2I FloatSlider::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void FloatSlider::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IMouseWidget::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.draw.reset();
        }
    }

    void FloatSlider::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.size = event.style->getSizeRole(SizeRole::Slider, event.displayScale);
            p.size.margin = event.style->getSizeRole(SizeRole::MarginInside, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);

            const auto fontInfo = event.style->getFontRole(FontRole::Label, event.displayScale);
            p.size.sizeHint = Size2I(p.size.size, event.fontSystem->getMetrics(fontInfo).lineHeight);
            p.size.sizeHint = margin(p.size.sizeHint, p.size.margin + p.size.keyFocus);

            p.draw.reset();
        }
    }

    void FloatSlider::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IMouseWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void FloatSlider::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IMouseWidget::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->g = getGeometry();
            p.draw->margin = margin(p.draw->g, -(p.size.margin + p.size.keyFocus));
            p.draw->border = border(p.draw->g, p.size.border);
            p.draw->keyFocus = border(p.draw->g, p.size.keyFocus);
        }

        // Draw the background.
        event.render->drawRect(
            p.draw->g,
            event.style->getColorRole(ColorRole::Base));

        // Draw the focus and border.
        const bool keyFocus = hasKeyFocus();
        event.render->drawMesh(
            keyFocus ? p.draw->keyFocus : p.draw->border,
            event.style->getColorRole(keyFocus ? ColorRole::KeyFocus : ColorRole::Border));

        // Draw the handle.
        const Box2I g2 = _getSliderGeometry();
        const int pos = _valueToPos(p.model->getValue());
        const Box2I handle(
            pos - p.size.handle / 2,
            g2.y(),
            p.size.handle,
            g2.h());
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

    void FloatSlider::mouseEnterEvent(MouseEnterEvent& event)
    {
        IMouseWidget::mouseEnterEvent(event);
        setDrawUpdate();
    }

    void FloatSlider::mouseLeaveEvent()
    {
        IMouseWidget::mouseLeaveEvent();
        setDrawUpdate();
    }

    void FloatSlider::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        FTK_P();
        if (_isMousePressed())
        {
            p.model->setValue(_posToValue(_getMousePos().x));
        }
    }

    void FloatSlider::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        takeKeyFocus();
        p.model->setValue(_posToValue(_getMousePos().x));
        setDrawUpdate();
    }

    void FloatSlider::mouseReleaseEvent(MouseClickEvent& event)
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

    void FloatSlider::scrollEvent(ScrollEvent& event)
    {
        FTK_P();
        event.accept = true;
        const float step = event.modifiers & static_cast<int>(KeyModifier::Shift) ?
            p.model->getLargeStep() :
            p.model->getStep();
        p.model->setValue(p.model->getValue() + step * event.value.y);
    }

    void FloatSlider::keyPressEvent(KeyEvent& event)
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

    void FloatSlider::keyReleaseEvent(KeyEvent& event)
    {
        IMouseWidget::keyReleaseEvent(event);
        event.accept = true;
    }

    Box2I FloatSlider::_getSliderGeometry() const
    {
        FTK_P();
        Box2I out;
        if (p.draw.has_value())
        {
            out = margin(
                p.draw->margin,
                -(p.size.handle / 2),
                0,
                -(p.size.handle / 2),
                0);
        }
        return out;
    }

    float FloatSlider::_posToValue(int pos) const
    {
        FTK_P();
        const Box2I g = _getSliderGeometry();
        const RangeF& range = p.model->getRange();
        const float v = (pos - g.x()) / static_cast<float>(g.w());
        const float out = range.min() + (range.max() - range.min()) * v;
        return out;
    }

    int FloatSlider::_valueToPos(float value) const
    {
        FTK_P();
        const Box2I g = _getSliderGeometry();
        const RangeF& range = p.model->getRange();
        float v = 0.F;
        if (range.min() != range.max())
        {
            v = (value - range.min()) /
                static_cast<float>(range.max() - range.min());
        }
        return g.x() + g.w() * v;
    }
}