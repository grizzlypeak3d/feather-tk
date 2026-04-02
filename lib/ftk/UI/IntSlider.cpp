// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IntSlider.h>

#include <ftk/UI/DrawUtil.h>

#include <optional>

namespace ftk
{
    struct IIntSlider::Private
    {
        std::shared_ptr<IntModel> model;
        std::function<void(int)> callback;
        std::function<void(int, bool)> pressedCallback;
        int blockCallbacks = 0;
        std::shared_ptr<Observer<int> > valueObserver;
        std::shared_ptr<Observer<RangeI> > rangeObserver;
    };

    void IIntSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::IIntSlider", parent);
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

    IIntSlider::IIntSlider() :
        _p(new Private)
    {}

    IIntSlider::~IIntSlider()
    {}

    int IIntSlider::getValue() const
    {
        return _p->model->getValue();
    }

    void IIntSlider::setValue(int value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setValue(value);
        --(p.blockCallbacks);
    }

    void IIntSlider::setCallback(const std::function<void(int)>& value)
    {
        _p->callback = value;
    }

    void IIntSlider::setPressedCallback(const std::function<void(int, bool)>& value)
    {
        _p->pressedCallback = value;
    }

    const RangeI& IIntSlider::getRange() const
    {
        return _p->model->getRange();
    }

    void IIntSlider::setRange(const RangeI& value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setRange(value);
        --(p.blockCallbacks);
    }

    void IIntSlider::setRange(int min, int max)
    {
        setRange(RangeI(min, max));
    }

    int IIntSlider::getStep() const
    {
        return _p->model->getStep();
    }

    void IIntSlider::setStep(int value)
    {
        _p->model->setStep(value);
    }

    int IIntSlider::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void IIntSlider::setLargeStep(int value)
    {
        _p->model->setLargeStep(value);
    }

    int IIntSlider::getDefault() const
    {
        return _p->model->getDefault();
    }

    void IIntSlider::setDefault(int value)
    {
        _p->model->setDefault(value);
    }

    const std::shared_ptr<IntModel>& IIntSlider::getModel() const
    {
        return _p->model;
    }

    void IIntSlider::mouseEnterEvent(MouseEnterEvent& event)
    {
        IMouseWidget::mouseEnterEvent(event);
        setDrawUpdate();
    }

    void IIntSlider::mouseLeaveEvent()
    {
        IMouseWidget::mouseLeaveEvent();
        setDrawUpdate();
    }

    void IIntSlider::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        FTK_P();
        if (_isMousePressed())
        {
            p.model->setValue(_posToValue(_getMousePos().x));
        }
    }

    void IIntSlider::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        takeKeyFocus();
        p.model->setValue(_posToValue(_getMousePos().x));
        setDrawUpdate();
    }

    void IIntSlider::mouseReleaseEvent(MouseClickEvent& event)
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

    void IIntSlider::scrollEvent(ScrollEvent& event)
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

    void IIntSlider::keyPressEvent(KeyEvent& event)
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

    void IIntSlider::keyReleaseEvent(KeyEvent& event)
    {
        IMouseWidget::keyReleaseEvent(event);
        event.accept = true;
    }

    int IIntSlider::_posToValue(int pos) const
    {
        FTK_P();
        const Box2I g = _getSliderGeometry();
        const RangeI& range = p.model->getRange();
        const float inc = g.w() / static_cast<float>(range.max() - range.min());
        const float v = (pos + inc / 2 - g.min.x) / static_cast<float>(g.w());
        const int out = range.min() + (range.max() - range.min()) * v;
        return out;
    }

    int IIntSlider::_valueToPos(int value) const
    {
        FTK_P();
        const RangeI& range = p.model->getRange();
        float v = 0.F;
        if (range.min() != range.max())
        {
            v = (value - range.min()) /
                static_cast<float>(range.max() - range.min());
        }
        const Box2I g = _getSliderGeometry();
        return g.min.x + g.w() * v;
    }

    struct IntSlider::Private
    {
        struct SizeData
        {
            int size = 0;
            int border = 0;
            int keyFocus = 0;
            int handle = 0;
            Size2I sizeHint;
        };
        std::optional<SizeData> size;

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

    void IntSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IIntSlider::_init(context, model, parent);
        FTK_P();
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
        out->_init(context, IntModel::create(), parent);
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

    Size2I IntSlider::getSizeHint() const
    {
        FTK_P();
        return p.size.has_value() ? p.size->sizeHint : Size2I();
    }
    
    void IntSlider::setGeometry(const Box2I& value)
    {
        if (value != getGeometry())
        {
            _p->draw.reset();
        }
        IIntSlider::setGeometry(value);
    }

    void IntSlider::styleEvent(const StyleEvent& event)
    {
        IIntSlider::styleEvent(event);
        FTK_P();
        if (event.hasChanges())
        {
            p.size.reset();
            p.draw.reset();
        }
    }

    void IntSlider::sizeHintEvent(const SizeHintEvent& event)
    {
        IIntSlider::sizeHintEvent(event);
        FTK_P();
        if (!p.size.has_value())
        {
            p.size = Private::SizeData();
            p.size->size = event.style->getSizeRole(SizeRole::Slider, event.displayScale);
            p.size->border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size->keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size->handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);

            const auto fontInfo = event.style->getFontRole(FontRole::Label, event.displayScale);
            p.size->sizeHint = Size2I(p.size->size, event.fontSystem->getMetrics(fontInfo).lineHeight);
            p.size->sizeHint = margin(p.size->sizeHint, p.size->keyFocus);

            p.draw.reset();
        }
    }

    void IntSlider::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IIntSlider::clipEvent(clipRect, clipped);
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
        IIntSlider::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->g = getGeometry();
            p.draw->g2 = _getInsideGeometry();
            p.draw->g3 = _getSliderGeometry();
            p.draw->border = border(p.draw->g, p.size->border);
            p.draw->keyFocus = border(p.draw->g, p.size->keyFocus);
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
            pos - p.size->handle / 2,
            p.draw->g3.min.y,
            p.size->handle,
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
            border(handle, p.size->border),
            event.style->getColorRole(ColorRole::Border));
    }

    Box2I IntSlider::_getSliderGeometry() const
    {
        FTK_P();
        Box2I out;
        if (p.size.has_value())
        {
            out = margin(_getInsideGeometry(), -p.size->handle / 2, 0, -p.size->handle / 2, 0);
        }
        return out;
    }

    Box2I IntSlider::_getInsideGeometry() const
    {
        FTK_P();
        Box2I out;
        if (p.size.has_value())
        {
            out = margin(getGeometry(), -p.size->keyFocus);
        }
        return out;
    }
}