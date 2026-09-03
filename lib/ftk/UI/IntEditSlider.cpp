// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IntEditSlider.h>

#include <ftk/UI/SliderPopup.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/UI/IntEdit.h>
#include <ftk/UI/IntSlider.h>
#include <ftk/UI/RowLayout.h>

namespace ftk
{
    struct IntEditSlider::Private
    {
        std::shared_ptr<IntModel> model;

        std::shared_ptr<IntEdit> edit;
        std::shared_ptr<IntSlider> slider;
        std::shared_ptr<ToolButton> popupButton;
        std::shared_ptr<IntSliderPopup> popup;
        std::shared_ptr<HorizontalLayout> layout;
        std::shared_ptr<Observer<bool> > hasDefaultObserver;

        std::function<void(int)> callback;
        std::function<void(int, bool)> pressedCallback;
    };

    void IntEditSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::IntEditSlider", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.model = model;

        p.edit = IntEdit::create(context, p.model);

        p.slider = IntSlider::create(context, p.model);
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
            [this](int value)
            {
                FTK_P();
                if (p.callback)
                {
                    p.callback(value);
                }
            });

        std::weak_ptr<IntEditSlider> weak(
            std::static_pointer_cast<IntEditSlider>(shared_from_this()));
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
            [this](int value, bool pressed)
            {
                FTK_P();
                if (p.pressedCallback)
                {
                    p.pressedCallback(value, pressed);
                }
            });
    }

    void IntEditSlider::_showPopup()
    {
        FTK_P();
        auto context = getContext();
        if (context && !p.popup)
        {
            p.popup = IntSliderPopup::create(context, p.model);
            std::weak_ptr<IntEditSlider> weak(
                std::static_pointer_cast<IntEditSlider>(shared_from_this()));
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

    IntEditSlider::IntEditSlider() :
        _p(new Private)
    {}

    IntEditSlider::~IntEditSlider()
    {}

    std::shared_ptr<IntEditSlider> IntEditSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<IntEditSlider>(new IntEditSlider);
        out->_init(context, IntModel::create(), parent);
        return out;
    }

    std::shared_ptr<IntEditSlider> IntEditSlider::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<IntEditSlider>(new IntEditSlider);
        out->_init(context, model, parent);
        return out;
    }

    int IntEditSlider::getValue() const
    {
        return _p->model->getValue();
    }

    void IntEditSlider::setValue(int value)
    {
        FTK_P();
        p.model->setValue(value);
    }

    void IntEditSlider::setCallback(const std::function<void(int)>& value)
    {
        _p->callback = value;
    }

    void IntEditSlider::setPressedCallback(const std::function<void(int, bool)>& value)
    {
        _p->pressedCallback = value;
    }

    const RangeI& IntEditSlider::getRange() const
    {
        return _p->model->getRange();
    }

    void IntEditSlider::setRange(const RangeI& value)
    {
        FTK_P();
        p.model->setRange(value);
    }

    void IntEditSlider::setRange(int min, int max)
    {
        setRange(RangeI(min, max));
    }

    int IntEditSlider::getStep() const
    {
        return _p->model->getStep();
    }

    void IntEditSlider::setStep(int value)
    {
        _p->model->setStep(value);
    }

    int IntEditSlider::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void IntEditSlider::setLargeStep(int value)
    {
        _p->model->setLargeStep(value);
    }

    int IntEditSlider::getDefault() const
    {
        return _p->model->getDefault();
    }

    void IntEditSlider::setDefault(int value)
    {
        _p->model->setDefault(value);
    }

    const std::shared_ptr<IntModel>& IntEditSlider::getModel() const
    {
        return _p->model;
    }

    FontType IntEditSlider::getFont() const
    {
        return _p->edit->getFont();
    }

    void IntEditSlider::setFont(FontType value)
    {
        _p->edit->setFont(value);
    }

    

    
}