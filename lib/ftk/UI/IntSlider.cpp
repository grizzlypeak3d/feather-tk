// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IntSlider.h>

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/Menu.h>
#include <ftk/UI/RangePopup.h>

#include <chrono>
#include <optional>

namespace ftk
{
    namespace
    {
        const float doubleClickTime = .5F;
    }

    struct IIntSlider::Private
    {
        std::shared_ptr<IntModel> model;
        std::function<void(int)> callback;
        std::function<void(int, bool)> pressedCallback;
        std::shared_ptr<IntRangePopup> rangePopup;
        std::chrono::steady_clock::time_point pressTime;
        bool resetPress = false;
        std::shared_ptr<Observer<int> > valueObserver;
        std::shared_ptr<Observer<RangeI> > rangeObserver;
        std::shared_ptr<Observer<bool> > hasDefaultObserver;
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
        setContextMenuCallback([this] { return _createContextMenu(); });

        p.model = model;

        p.valueObserver = Observer<int>::create(
            p.model->observeValue(),
            [this](int value)
            {
                FTK_P();
                setSizeUpdate();
                setDrawUpdate();
                if (p.callback)
                {
                    p.callback(value);
                }
                if (p.pressedCallback)
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

        p.hasDefaultObserver = Observer<bool>::create(
            p.model->observeHasDefault(),
            [this](bool)
            {
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
        p.model->setValue(value);
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
        p.model->setRange(value);
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
        if (_isMousePressed() && !p.resetPress)
        {
            p.model->setValue(_posToValue(_getMousePos().x));
        }
    }

    void IIntSlider::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        if (!_isMousePressed())
        {
            // An unclaimed button; a right click opens the context menu.
            return;
        }
        takeKeyFocus();
        const auto now = std::chrono::steady_clock::now();
        const std::chrono::duration<float> diff = now - p.pressTime;
        p.pressTime = now;
        // A double click resets to the default value. The press is
        // remembered so that the drag and release do not set the value
        // back to the click position.
        p.resetPress = diff.count() < doubleClickTime && p.model->hasDefault();
        if (p.resetPress)
        {
            p.model->setDefault();
        }
        else
        {
            p.model->setValue(_posToValue(_getMousePos().x));
        }
        setDrawUpdate();
    }

    void IIntSlider::mouseReleaseEvent(MouseClickEvent& event)
    {
        const bool wasPressed = _isMousePressed();
        IMouseWidget::mouseReleaseEvent(event);
        FTK_P();
        if (!wasPressed)
        {
            return;
        }
        if (!p.resetPress)
        {
            p.model->setValue(_posToValue(_getMousePos().x));
        }
        p.resetPress = false;
        if (p.pressedCallback)
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

    std::shared_ptr<Menu> IIntSlider::_createContextMenu()
    {
        FTK_P();
        auto context = getContext();
        if (!context)
            return nullptr;

        // The menu takes the key focus while it is open and returns it
        // here when it closes.
        takeKeyFocus();

        auto out = Menu::create(context);

        // Hold the model rather than the widget so the actions stay valid
        // if the slider is destroyed while the menu is open.
        auto model = p.model;
        auto reset = Action::create(
            "Reset",
            [model] { model->setDefault(); });
        out->addAction(reset);
        out->setEnabled(reset, model->hasDefault());

        // A soft range is a starting point the user may adjust, so it
        // is offered here; a hard range is a fixed bound.
        if (model->isRangeSoft())
        {
            out->addDivider();

            std::weak_ptr<IIntSlider> weak(
                std::static_pointer_cast<IIntSlider>(shared_from_this()));
            auto setRange = Action::create(
                "Set Range...",
                [weak]
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_showRangePopup();
                    }
                });
            out->addAction(setRange);
        }

        return out;
    }

    void IIntSlider::_showRangePopup()
    {
        FTK_P();
        auto context = getContext();
        if (context && !p.rangePopup)
        {
            p.rangePopup = IntRangePopup::create(context, p.model);
            std::weak_ptr<IIntSlider> weak(
                std::static_pointer_cast<IIntSlider>(shared_from_this()));
            p.rangePopup->setCloseCallback(
                [weak]
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->rangePopup.reset();
                    }
                });
            p.rangePopup->open(getWindow(), getGeometry());
        }
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
            bool init = true;
            int size = 0;
            int border = 0;
            int keyFocus = 0;
            int handle = 0;
            Size2I sizeHint;
        };
        SizeData size;
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
        return _p->size.sizeHint;
    }
    
    void IntSlider::styleEvent(const StyleEvent& event)
    {
        IIntSlider::styleEvent(event);
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
        }
    }

    void IntSlider::sizeHintEvent(const SizeHintEvent& event)
    {
        IIntSlider::sizeHintEvent(event);
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.size = event.style->getSizeRole(SizeRole::Slider, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.handle = event.style->getSizeRole(SizeRole::Handle, event.displayScale);

            const auto fontInfo = event.style->getFont(FontType::Regular, event.displayScale);
            p.size.sizeHint = Size2I(p.size.size, event.fontSystem->getMetrics(fontInfo).lineHeight);
            p.size.sizeHint = margin(p.size.sizeHint, p.size.keyFocus);
        }
    }

    void IntSlider::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IIntSlider::drawEvent(drawRect, event);
        FTK_P();

        const bool enabled = isEnabled();

        // Draw the trough.
        const int pos = _valueToPos(getValue());
        const Box2I g2 = _getInsideGeometry();
        const int h = p.size.border * 2;
        const int y = g2.y() + g2.h() / 2 - h / 2;
        // The filled part in the checked color, the way a toggled tool button
        // is. Both halves were the border color, told apart by alpha, which on
        // the dark palette is a near black against a near black; and the dimmed
        // half then vanished into the light one, the border there being a near
        // white. With the fill carrying a color of its own the remainder can be
        // the border at its own weight, which reads on both.
        const Color4F troughColor = event.style->getColorRole(ColorRole::Checked, enabled);
        const Color4F remainderColor = event.style->getColorRole(ColorRole::Border, enabled);
        event.render->drawRect(
            Box2I(g2.x(), y, g2.w(), h),
            remainderColor);
        event.render->drawRect(
            Box2I(g2.x(), y, pos - g2.x(), h),
            troughColor);

        // Mark the default value with a tick. The handle covers the tick
        // when the value is at the default, so seeing it means the value
        // has been changed.
        const auto& model = getModel();
        if (model->hasDefault())
        {
            const int defaultValue = model->getDefault();
            const RangeI& range = model->getRange();
            if (defaultValue >= range.min() && defaultValue <= range.max())
            {
                event.render->drawRect(
                    Box2I(
                        _valueToPos(defaultValue) - p.size.border,
                        g2.y() + g2.h() / 6,
                        p.size.border * 2,
                        g2.h() * 2 / 3),
                    event.style->getColorRole(ColorRole::TextDisabled, enabled));
            }
        }

        // Draw the handle.
        const Box2I gh = _getHandleGeometry();
        event.render->drawRect(
            gh,
            event.style->getColorRole(ColorRole::Button, enabled));
        event.render->drawMesh(
            border(gh, p.size.border),
            event.style->getColorRole(ColorRole::Border, enabled));
        if (hasKeyFocus())
        {
            event.render->drawRect(
                gh,
                event.style->getColorRole(ColorRole::KeyFocus));
        }
        if (_isMousePressed())
        {
            event.render->drawRect(
                gh,
                event.style->getColorRole(ColorRole::Pressed));
        }
        else if (_isMouseInside())
        {
            event.render->drawRect(
                gh,
                event.style->getColorRole(ColorRole::Hover));
        }
    }

    Box2I IntSlider::_getSliderGeometry() const
    {
        FTK_P();
        return margin(_getInsideGeometry(), -p.size.handle / 2, 0, -p.size.handle / 2, 0);
    }

    Box2I IntSlider::_getInsideGeometry() const
    {
        FTK_P();
        // Draw at a fixed height, centered vertically, regardless of how
        // the widget is stretched.
        const Box2I& g = getGeometry();
        const int h = std::min(p.size.sizeHint.h, g.h());
        const Box2I g2(g.min.x, g.min.y + (g.h() - h) / 2, g.w(), h);
        return margin(g2, -p.size.keyFocus);
    }

    Box2I IntSlider::_getHandleGeometry() const
    {
        FTK_P();
        const Box2I& g = _getInsideGeometry();
        const int pos = _valueToPos(getValue());
        const Box2I handle(
            pos - p.size.handle / 2,
            g.min.y,
            p.size.handle,
            g.h());
        return handle;
    }
}
