// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DoubleSlider.h>

#include <ftk/UI/DrawUtil.h>

#include <optional>

namespace ftk
{
    struct DoubleSlider::Private
    {
        std::shared_ptr<DoubleModel> model;
        std::function<void(double)> callback;
        std::function<void(double, bool)> pressedCallback;
        bool blockCallbacks = false;
        std::shared_ptr<Observer<double> > valueObserver;
        std::shared_ptr<Observer<RangeD> > rangeObserver;

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

    void DoubleSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::DoubleSlider", parent);
        FTK_P();

        setAcceptsKeyFocus(true);
        setHStretch(Stretch::Expanding);
        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);

        p.model = model;

        p.valueObserver = Observer<double>::create(
            p.model->observeValue(),
            [this](double value)
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

        p.rangeObserver = Observer<RangeD>::create(
            p.model->observeRange(),
            [this](const RangeD&)
            {
                setSizeUpdate();
                setDrawUpdate();
            });
    }

    DoubleSlider::DoubleSlider() :
        _p(new Private)
    {}

    DoubleSlider::~DoubleSlider()
    {}

    std::shared_ptr<DoubleSlider> DoubleSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DoubleSlider>(new DoubleSlider);
        out->_init(context, DoubleModel::create(context), parent);
        return out;
    }

    std::shared_ptr<DoubleSlider> DoubleSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DoubleSlider>(new DoubleSlider);
        out->_init(context, model, parent);
        return out;
    }

    double DoubleSlider::getValue() const
    {
        return _p->model->getValue();
    }

    void DoubleSlider::setValue(double value)
    {
        FTK_P();
        p.blockCallbacks = true;
        p.model->setValue(value);
        p.blockCallbacks = false;
    }

    void DoubleSlider::setCallback(const std::function<void(double)>& value)
    {
        _p->callback = value;
    }

    void DoubleSlider::setPressedCallback(const std::function<void(double, bool)>& value)
    {
        _p->pressedCallback = value;
    }

    const RangeD& DoubleSlider::getRange() const
    {
        return _p->model->getRange();
    }

    void DoubleSlider::setRange(const RangeD& value)
    {
        FTK_P();
        p.blockCallbacks = true;
        p.model->setRange(value);
        p.blockCallbacks = false;
    }

    void DoubleSlider::setRange(double min, double max)
    {
        setRange(RangeD(min, max));
    }

    double DoubleSlider::getStep() const
    {
        return _p->model->getStep();
    }

    void DoubleSlider::setStep(double value)
    {
        _p->model->setStep(value);
    }

    double DoubleSlider::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void DoubleSlider::setLargeStep(double value)
    {
        _p->model->setLargeStep(value);
    }

    double DoubleSlider::getDefaultValue() const
    {
        return _p->model->getDefaultValue();
    }

    void DoubleSlider::setDefaultValue(double value)
    {
        _p->model->setDefaultValue(value);
    }

    const std::shared_ptr<DoubleModel>& DoubleSlider::getModel() const
    {
        return _p->model;
    }

    Size2I DoubleSlider::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void DoubleSlider::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IMouseWidget::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.draw.reset();
        }
    }

    void DoubleSlider::sizeHintEvent(const SizeHintEvent& event)
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
            p.size.margin = event.style->getSizeRole(SizeRole::MarginInside, event.displayScale);
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);

            const auto fontInfo = event.style->getFontRole(FontRole::Label, event.displayScale);
            p.size.sizeHint = Size2I(p.size.size, event.fontSystem->getMetrics(fontInfo).lineHeight);
            p.size.sizeHint = margin(p.size.sizeHint, p.size.margin + p.size.keyFocus);

            p.draw.reset();
        }
    }

    void DoubleSlider::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IMouseWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void DoubleSlider::drawEvent(
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
        int pos = 0;
        if (p.model)
        {
            pos = _valueToPos(p.model->getValue());
        }
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

    void DoubleSlider::mouseEnterEvent(MouseEnterEvent& event)
    {
        IMouseWidget::mouseEnterEvent(event);
        setDrawUpdate();
    }

    void DoubleSlider::mouseLeaveEvent()
    {
        IMouseWidget::mouseLeaveEvent();
        setDrawUpdate();
    }

    void DoubleSlider::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        FTK_P();
        if (_isMousePressed() && p.model)
        {
            p.model->setValue(_posToValue(_getMousePos().x));
        }
    }

    void DoubleSlider::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        takeKeyFocus();
        if (p.model)
        {
            p.model->setValue(_posToValue(_getMousePos().x));
        }
        setDrawUpdate();
    }

    void DoubleSlider::mouseReleaseEvent(MouseClickEvent& event)
    {
        IMouseWidget::mouseReleaseEvent(event);
        FTK_P();
        if (p.pressedCallback)
        {
            p.pressedCallback(_posToValue(_getMousePos().x), false);
        }
        setDrawUpdate();
    }

    void DoubleSlider::scrollEvent(ScrollEvent& event)
    {
        FTK_P();
        event.accept = true;
        const double step = event.modifiers & static_cast<int>(KeyModifier::Shift) ?
            p.model->getLargeStep() :
            p.model->getStep();
        p.model->setValue(p.model->getValue() + step * event.value.y);
    }

    void DoubleSlider::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        if (isEnabled() && p.model && 0 == event.modifiers)
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

    void DoubleSlider::keyReleaseEvent(KeyEvent& event)
    {
        IMouseWidget::keyReleaseEvent(event);
        event.accept = true;
    }

    Box2I DoubleSlider::_getSliderGeometry() const
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

    double DoubleSlider::_posToValue(int pos) const
    {
        FTK_P();
        const Box2I g = _getSliderGeometry();
        const double v = (pos - g.x()) / static_cast<double>(g.w());
        double out = 0.0;
        if (p.model)
        {
            const RangeD& range = p.model->getRange();
            out = range.min() + (range.max() - range.min()) * v;
        }
        return out;
    }

    int DoubleSlider::_valueToPos(double value) const
    {
        FTK_P();
        const Box2I g = _getSliderGeometry();
        double v = 0.0;
        if (p.model)
        {
            const RangeD& range = p.model->getRange();
            if (range.min() != range.max())
            {
                v = (value - range.min()) /
                    static_cast<double>(range.max() - range.min());
            }
        }
        return g.x() + g.w() * v;
    }
}