// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DoubleSlider.h>

#include <ftk/UI/DrawUtil.h>

#include <optional>

namespace ftk
{
    struct IDoubleSlider::Private
    {
        std::shared_ptr<DoubleModel> model;
        std::function<void(double)> callback;
        std::function<void(double, bool)> pressedCallback;
        int blockCallbacks = 0;
        std::shared_ptr<Observer<double> > valueObserver;
        std::shared_ptr<Observer<RangeD> > rangeObserver;
    };

    void IDoubleSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::IDoubleSlider", parent);
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

    IDoubleSlider::IDoubleSlider() :
        _p(new Private)
    {}

    IDoubleSlider::~IDoubleSlider()
    {}

    double IDoubleSlider::getValue() const
    {
        return _p->model->getValue();
    }

    void IDoubleSlider::setValue(double value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setValue(value);
        --(p.blockCallbacks);
    }

    void IDoubleSlider::setCallback(const std::function<void(double)>& value)
    {
        _p->callback = value;
    }

    void IDoubleSlider::setPressedCallback(const std::function<void(double, bool)>& value)
    {
        _p->pressedCallback = value;
    }

    const RangeD& IDoubleSlider::getRange() const
    {
        return _p->model->getRange();
    }

    void IDoubleSlider::setRange(const RangeD& value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setRange(value);
        --(p.blockCallbacks);
    }

    void IDoubleSlider::setRange(double min, double max)
    {
        setRange(RangeD(min, max));
    }

    double IDoubleSlider::getStep() const
    {
        return _p->model->getStep();
    }

    void IDoubleSlider::setStep(double value)
    {
        _p->model->setStep(value);
    }

    double IDoubleSlider::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void IDoubleSlider::setLargeStep(double value)
    {
        _p->model->setLargeStep(value);
    }

    double IDoubleSlider::getDefault() const
    {
        return _p->model->getDefault();
    }

    void IDoubleSlider::setDefault(double value)
    {
        _p->model->setDefault(value);
    }

    const std::shared_ptr<DoubleModel>& IDoubleSlider::getModel() const
    {
        return _p->model;
    }

    void IDoubleSlider::mouseEnterEvent(MouseEnterEvent& event)
    {
        IMouseWidget::mouseEnterEvent(event);
        setDrawUpdate();
    }

    void IDoubleSlider::mouseLeaveEvent()
    {
        IMouseWidget::mouseLeaveEvent();
        setDrawUpdate();
    }

    void IDoubleSlider::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        FTK_P();
        if (_isMousePressed())
        {
            p.model->setValue(_posToValue(_getMousePos().x));
        }
    }

    void IDoubleSlider::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        takeKeyFocus();
        p.model->setValue(_posToValue(_getMousePos().x));
        setDrawUpdate();
    }

    void IDoubleSlider::mouseReleaseEvent(MouseClickEvent& event)
    {
        IMouseWidget::mouseReleaseEvent(event);
        FTK_P();
        p.model->setValue(_posToValue(_getMousePos().x));
        if (p.pressedCallback && !p.blockCallbacks)
        {
            p.pressedCallback(p.model->getValue(), false);
        }
        setDrawUpdate();
    }

    void IDoubleSlider::scrollEvent(ScrollEvent& event)
    {
        FTK_P();
        if (hasKeyFocus())
        {
            event.accept = true;
            const double step = event.modifiers & static_cast<int>(KeyModifier::Shift) ?
                p.model->getLargeStep() :
                p.model->getStep();
            p.model->setValue(p.model->getValue() + step * event.value.y);
        }
    }

    void IDoubleSlider::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        if (isEnabled() && 0 == event.modifiers)
        {
            switch (event.key)
            {
            case Key::Left:
            case Key::Down:
                event.accept = true;
                p.model->stepDec();
                break;
            case Key::Right:
            case Key::Up:
                event.accept = true;
                p.model->step();
                break;
            case Key::PageUp:
                event.accept = true;
                p.model->largeStep();
                break;
            case Key::PageDown:
                event.accept = true;
                p.model->largeStepDec();
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

    void IDoubleSlider::keyReleaseEvent(KeyEvent& event)
    {
        IMouseWidget::keyReleaseEvent(event);
        event.accept = true;
    }

    double IDoubleSlider::_posToValue(int pos) const
    {
        FTK_P();
        const Box2I g = _getSliderGeometry();
        const RangeD& range = p.model->getRange();
        const double v = (pos - g.min.x) / static_cast<double>(g.w());
        const double out = range.min() + (range.max() - range.min()) * v;
        return out;
    }

    int IDoubleSlider::_valueToPos(double value) const
    {
        FTK_P();
        const RangeD& range = p.model->getRange();
        double v = 0.0;
        if (range.min() != range.max())
        {
            v = (value - range.min()) /
                static_cast<double>(range.max() - range.min());
        }
        const Box2I g = _getSliderGeometry();
        return g.min.x + g.w() * v;
    }

    struct DoubleSlider::Private
    {
        struct SizeData
        {
            bool init = true;
            int size = 0;
            int border = 0;
            int keyFocus = 0;
            int handle = 0;
            Size2I sizeHint;
        };
        SizeData size;

        struct DrawData
        {
            Box2I g;
            Box2I g2;
            Box2I g3;
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
        IDoubleSlider::_init(context, model, parent);
        FTK_P();
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
        out->_init(context, DoubleModel::create(), parent);
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

    Size2I DoubleSlider::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void DoubleSlider::setGeometry(const Box2I& value)
    {
        if (value != getGeometry())
        {
            _p->draw.reset();
        }
        IDoubleSlider::setGeometry(value);
    }

    void DoubleSlider::styleEvent(const StyleEvent& event)
    {
        IDoubleSlider::styleEvent(event);
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
            p.draw.reset();
        }
    }

    void DoubleSlider::sizeHintEvent(const SizeHintEvent& event)
    {
        IDoubleSlider::sizeHintEvent(event);
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
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

    void DoubleSlider::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IDoubleSlider::clipEvent(clipRect, clipped);
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
        IDoubleSlider::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->g = getGeometry();
            p.draw->g2 = _getInsideGeometry();
            p.draw->g3 = _getSliderGeometry();
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
        const int pos = _valueToPos(getValue());
        const Box2I handle(
            pos - p.size.handle / 2,
            p.draw->g3.min.y,
            p.size.handle,
            p.draw->g3.h());
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

    Box2I DoubleSlider::_getSliderGeometry() const
    {
        FTK_P();
        return margin(_getInsideGeometry(), -p.size.handle / 2, 0, -p.size.handle / 2, 0);
    }

    Box2I DoubleSlider::_getInsideGeometry() const
    {
        FTK_P();
        return margin(getGeometry(), -p.size.keyFocus);
    }
}