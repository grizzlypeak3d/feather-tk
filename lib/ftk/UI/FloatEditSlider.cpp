// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FloatEditSlider.h>

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
        std::shared_ptr<FloatResetButton> resetButton;
        std::shared_ptr<HorizontalLayout> layout;

        std::function<void(float)> callback;
        std::function<void(float, bool)> pressedCallback;
        bool blockCallbacks = false;
    };

    void FloatEditSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::FloatEditSlider", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.model = model;

        p.edit = FloatEdit::create(context, p.model);

        p.slider = FloatSlider::create(context, p.model);

        p.resetButton = FloatResetButton::create(context, p.model);

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.edit->setParent(p.layout);
        p.slider->setParent(p.layout);
        p.slider->setHStretch(Stretch::Expanding);
        p.resetButton->setParent(p.layout);

        p.slider->setCallback(
            [this](float value)
            {
                FTK_P();
                if (p.callback && !p.blockCallbacks)
                {
                    p.callback(value);
                }
            });

        p.slider->setPressedCallback(
            [this](float value, bool pressed)
            {
                FTK_P();
                if (p.pressedCallback && !p.blockCallbacks)
                {
                    p.pressedCallback(value, pressed);
                }
            });
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
        out->_init(context, FloatModel::create(context), parent);
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
        p.blockCallbacks = true;
        p.model->setValue(value);
        p.blockCallbacks = false;
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
        p.blockCallbacks = true;
        p.model->setRange(value);
        p.blockCallbacks = false;
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

    float FloatEditSlider::getDefaultValue() const
    {
        return _p->model->getDefaultValue();
    }

    void FloatEditSlider::setDefaultValue(float value)
    {
        _p->model->setDefaultValue(value);
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

    FontRole FloatEditSlider::getFontRole() const
    {
        return _p->edit->getFontRole();
    }

    void FloatEditSlider::setFontRole(FontRole value)
    {
        _p->edit->setFontRole(value);
    }
    
    Size2I FloatEditSlider::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void FloatEditSlider::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }
}