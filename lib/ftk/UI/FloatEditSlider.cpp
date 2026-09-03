// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FloatEditSlider.h>

#include <ftk/UI/SliderPopup.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/UI/FloatEdit.h>
#include <ftk/UI/FloatSlider.h>
#include <ftk/UI/RowLayout.h>

namespace ftk
{
    struct FloatEditSlider::Private
    {
        std::shared_ptr<FloatModel> model;

        std::shared_ptr<FloatEdit> edit;
        std::shared_ptr<FloatSlider> slider;
        std::shared_ptr<ToolButton> popupButton;
        std::shared_ptr<FloatSliderPopup> popup;
        std::shared_ptr<HorizontalLayout> layout;
        std::shared_ptr<Observer<bool> > hasDefaultObserver;

        std::function<void(float)> callback;
        std::function<void(float, bool)> pressedCallback;
    };

    void FloatEditSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::FloatEditSlider", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.model = model;

        p.edit = FloatEdit::create(context, p.model);

        p.slider = FloatSlider::create(context, p.model);
        p.slider->setHStretch(Stretch::Expanding);

        // The popup replaces the sliders' old context menu (#829): the
        // small triangle can be seen, where a right click has to be
        // guessed at.
        p.popupButton = ToolButton::create(context);
        p.popupButton->setPopupIcon(true);
        p.popupButton->setTooltip("Reset and range options.");

        p.layout = HorizontalLayout::create(context);
        _setWidget(p.layout);
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.edit->setParent(p.layout);
        p.slider->setParent(p.layout);
        p.popupButton->setParent(p.layout);

        p.slider->setCallback(
            [this](float value)
            {
                FTK_P();
                if (p.callback)
                {
                    p.callback(value);
                }
            });

        std::weak_ptr<FloatEditSlider> weak(
            std::static_pointer_cast<FloatEditSlider>(shared_from_this()));
        p.popupButton->setClickedCallback(
            [weak]
            {
                if (auto widget = weak.lock())
                {
                    widget->_showPopup();
                }
            });

        // The button earns its place: a slider with no default and a
        // hard range has an empty popup, so it shows no button.
        p.hasDefaultObserver = Observer<bool>::create(
            p.model->observeHasDefault(),
            [weak](bool value)
            {
                if (auto widget = weak.lock())
                {
                    widget->_p->popupButton->setVisible(
                        value || widget->_p->model->isRangeSoft());
                }
            });

        p.slider->setPressedCallback(
            [this](float value, bool pressed)
            {
                FTK_P();
                if (p.pressedCallback)
                {
                    p.pressedCallback(value, pressed);
                }
            });
    }

    void FloatEditSlider::_showPopup()
    {
        FTK_P();
        auto context = getContext();
        if (context && !p.popup)
        {
            p.popup = FloatSliderPopup::create(context, p.model);
            std::weak_ptr<FloatEditSlider> weak(
                std::static_pointer_cast<FloatEditSlider>(shared_from_this()));
            p.popup->setCloseCallback(
                [weak]
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->popup.reset();
                    }
                });
            p.popup->open(getWindow(), getGeometry());
        }
    }

    FloatEditSlider::FloatEditSlider() :
        _p(new Private)
    {}

    FloatEditSlider::~FloatEditSlider()
    {}

    std::shared_ptr<FloatEditSlider> FloatEditSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FloatEditSlider>(new FloatEditSlider);
        out->_init(context, FloatModel::create(), parent);
        return out;
    }

    std::shared_ptr<FloatEditSlider> FloatEditSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FloatEditSlider>(new FloatEditSlider);
        out->_init(context, model, parent);
        return out;
    }

    float FloatEditSlider::getValue() const
    {
        return _p->model->getValue();
    }

    void FloatEditSlider::setValue(float value)
    {
        FTK_P();
        p.model->setValue(value);
    }

    void FloatEditSlider::setCallback(const std::function<void(float)>& value)
    {
        _p->callback = value;
    }

    void FloatEditSlider::setPressedCallback(const std::function<void(float, bool) > & value)
    {
        _p->pressedCallback = value;
    }

    const RangeF& FloatEditSlider::getRange() const
    {
        return _p->model->getRange();
    }

    void FloatEditSlider::setRange(const RangeF& value)
    {
        FTK_P();
        p.model->setRange(value);
    }

    void FloatEditSlider::setRange(float min, float max)
    {
        setRange(RangeF(min, max));
    }

    float FloatEditSlider::getStep() const
    {
        return _p->model->getStep();
    }

    void FloatEditSlider::setStep(float value)
    {
        _p->model->setStep(value);
    }

    float FloatEditSlider::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void FloatEditSlider::setLargeStep(float value)
    {
        _p->model->setLargeStep(value);
    }

    float FloatEditSlider::getDefault() const
    {
        return _p->model->getDefault();
    }

    void FloatEditSlider::setDefault(float value)
    {
        _p->model->setDefault(value);
    }

    const std::shared_ptr<FloatModel>& FloatEditSlider::getModel() const
    {
        return _p->model;
    }

    int FloatEditSlider::getPrecision() const
    {
        return _p->edit->getPrecision();
    }

    void FloatEditSlider::setPrecision(int value)
    {
        _p->edit->setPrecision(value);
    }

    FontType FloatEditSlider::getFont() const
    {
        return _p->edit->getFont();
    }

    void FloatEditSlider::setFont(FontType value)
    {
        _p->edit->setFont(value);
    }
    
    

    
}