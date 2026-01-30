// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DoubleEditSlider.h>

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
        std::shared_ptr<DoubleResetButton> resetButton;
        std::shared_ptr<HorizontalLayout> layout;

        std::function<void(double)> callback;
        std::function<void(double, bool)> pressedCallback;
        bool blockCallbacks = false;
    };

    void DoubleEditSlider::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::DoubleEditSlider", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.model = model;

        p.edit = DoubleEdit::create(context, p.model);

        p.slider = DoubleSlider::create(context, p.model);

        p.resetButton = DoubleResetButton::create(context, p.model);

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.edit->setParent(p.layout);
        p.slider->setParent(p.layout);
        p.slider->setHStretch(Stretch::Expanding);
        p.resetButton->setParent(p.layout);

        p.slider->setCallback(
            [this](double value)
            {
                FTK_P();
                if (p.callback && !p.blockCallbacks)
                {
                    p.callback(value);
                }
            });

        p.slider->setPressedCallback(
            [this](double value, bool pressed)
            {
                FTK_P();
                if (p.pressedCallback && !p.blockCallbacks)
                {
                    p.pressedCallback(value, pressed);
                }
            });
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
        out->_init(context, DoubleModel::create(context), parent);
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
        p.blockCallbacks = true;
        p.model->setValue(value);
        p.blockCallbacks = false;
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
        p.blockCallbacks = true;
        p.model->setRange(value);
        p.blockCallbacks = false;
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

    double DoubleEditSlider::getDefaultValue() const
    {
        return _p->model->getDefaultValue();
    }

    void DoubleEditSlider::setDefaultValue(double value)
    {
        _p->model->setDefaultValue(value);
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

    FontRole DoubleEditSlider::getFontRole() const
    {
        return _p->edit->getFontRole();
    }

    void DoubleEditSlider::setFontRole(FontRole value)
    {
        _p->edit->setFontRole(value);
    }

    Size2I DoubleEditSlider::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void DoubleEditSlider::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }
}