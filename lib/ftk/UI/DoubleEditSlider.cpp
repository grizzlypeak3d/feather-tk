// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DoubleEditSlider.h>

#include <ftk/UI/SliderPopup.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/UI/DoubleEdit.h>
#include <ftk/UI/DoubleSlider.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ToolButton.h>

namespace ftk
{
    struct DoubleEditSlider::Private
    {
        std::shared_ptr<DoubleModel> model;

        std::shared_ptr<DoubleEdit> edit;
        std::shared_ptr<DoubleSlider> slider;
        std::shared_ptr<ToolButton> popupButton;
        std::shared_ptr<DoubleSliderPopup> popup;
        std::shared_ptr<HorizontalLayout> layout;
        std::shared_ptr<Observer<bool> > hasDefaultObserver;

        std::function<void(double)> callback;
        std::function<void(double, bool)> pressedCallback;
    };

    void DoubleEditSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::DoubleEditSlider", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.model = model;

        p.edit = DoubleEdit::create(context, p.model);

        p.slider = DoubleSlider::create(context, p.model);
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
            [this](double value)
            {
                FTK_P();
                if (p.callback)
                {
                    p.callback(value);
                }
            });

        std::weak_ptr<DoubleEditSlider> weak(
            std::static_pointer_cast<DoubleEditSlider>(shared_from_this()));
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
            [this](double value, bool pressed)
            {
                FTK_P();
                if (p.pressedCallback)
                {
                    p.pressedCallback(value, pressed);
                }
            });
    }

    void DoubleEditSlider::_showPopup()
    {
        FTK_P();
        // The button toggles, the way a combo box does: the press falls
        // through the open popup to the button, and the click closes it.
        if (p.popup)
        {
            p.popup->close();
            p.popup.reset();
            return;
        }
        auto context = getContext();
        if (context)
        {
            p.popup = DoubleSliderPopup::create(context, p.model);
            std::weak_ptr<DoubleEditSlider> weak(
                std::static_pointer_cast<DoubleEditSlider>(shared_from_this()));
            p.popup->setCloseCallback(
                [weak]
                {
                    if (auto widget = weak.lock())
                    {
                        widget->_p->popup.reset();
                    }
                });
            // Under the button that opened it, not the whole slider.
            p.popup->open(getWindow(), p.popupButton->getGeometry());
        }
    }

    DoubleEditSlider::DoubleEditSlider() :
        _p(new Private)
    {}

    DoubleEditSlider::~DoubleEditSlider()
    {}

    std::shared_ptr<DoubleEditSlider> DoubleEditSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DoubleEditSlider>(new DoubleEditSlider);
        out->_init(context, DoubleModel::create(), parent);
        return out;
    }

    std::shared_ptr<DoubleEditSlider> DoubleEditSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DoubleEditSlider>(new DoubleEditSlider);
        out->_init(context, model, parent);
        return out;
    }

    double DoubleEditSlider::getValue() const
    {
        return _p->model->getValue();
    }

    void DoubleEditSlider::setValue(double value)
    {
        FTK_P();
        p.model->setValue(value);
    }

    void DoubleEditSlider::setCallback(const std::function<void(double)>& value)
    {
        _p->callback = value;
    }

    void DoubleEditSlider::setPressedCallback(const std::function<void(double, bool)>& value)
    {
        _p->pressedCallback = value;
    }

    const RangeD& DoubleEditSlider::getRange() const
    {
        return _p->model->getRange();
    }

    void DoubleEditSlider::setRange(const RangeD& value)
    {
        FTK_P();
        p.model->setRange(value);
    }

    void DoubleEditSlider::setRange(double min, double max)
    {
        setRange(RangeD(min, max));
    }

    double DoubleEditSlider::getStep() const
    {
        return _p->model->getStep();
    }

    void DoubleEditSlider::setStep(double value)
    {
        _p->model->setStep(value);
    }

    double DoubleEditSlider::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void DoubleEditSlider::setLargeStep(double value)
    {
        _p->model->setLargeStep(value);
    }

    double DoubleEditSlider::getDefault() const
    {
        return _p->model->getDefault();
    }

    void DoubleEditSlider::setDefault(double value)
    {
        _p->model->setDefault(value);
    }

    const std::shared_ptr<DoubleModel>& DoubleEditSlider::getModel() const
    {
        return _p->model;
    }

    int DoubleEditSlider::getPrecision() const
    {
        return _p->edit->getPrecision();
    }

    void DoubleEditSlider::setPrecision(int value)
    {
        _p->edit->setPrecision(value);
    }

    FontType DoubleEditSlider::getFont() const
    {
        return _p->edit->getFont();
    }

    void DoubleEditSlider::setFont(FontType value)
    {
        _p->edit->setFont(value);
    }

    

    
}