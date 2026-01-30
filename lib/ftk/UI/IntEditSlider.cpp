// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IntEditSlider.h>

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
        std::shared_ptr<IntResetButton> resetButton;
        std::shared_ptr<HorizontalLayout> layout;

        std::function<void(double)> callback;
        std::function<void(double, bool)> pressedCallback;
        bool blockCallbacks = false;
    };

    void IntEditSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::IntEditSlider", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.model = model;

        p.edit = IntEdit::create(context, p.model);

        p.slider = IntSlider::create(context, p.model);

        p.resetButton = IntResetButton::create(context, p.model);

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.edit->setParent(p.layout);
        p.slider->setParent(p.layout);
        p.slider->setHStretch(Stretch::Expanding);
        p.resetButton->setParent(p.layout);

        p.slider->setCallback(
            [this](int value)
            {
                FTK_P();
                if (p.callback && !p.blockCallbacks)
                {
                    p.callback(value);
                }
            });

        p.slider->setPressedCallback(
            [this](int value, bool pressed)
            {
                FTK_P();
                if (p.pressedCallback && !p.blockCallbacks)
                {
                    p.pressedCallback(value, pressed);
                }
            });
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
        out->_init(context, IntModel::create(context), parent);
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
        p.blockCallbacks = true;
        p.model->setValue(value);
        p.blockCallbacks = false;
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
        p.blockCallbacks = true;
        p.model->setRange(value);
        p.blockCallbacks = false;
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

    int IntEditSlider::getDefaultValue() const
    {
        return _p->model->getDefaultValue();
    }

    void IntEditSlider::setDefaultValue(int value)
    {
        _p->model->setDefaultValue(value);
    }

    const std::shared_ptr<IntModel>& IntEditSlider::getModel() const
    {
        return _p->model;
    }

    FontRole IntEditSlider::getFontRole() const
    {
        return _p->edit->getFontRole();
    }

    void IntEditSlider::setFontRole(FontRole value)
    {
        _p->edit->setFontRole(value);
    }

    Size2I IntEditSlider::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void IntEditSlider::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }
}