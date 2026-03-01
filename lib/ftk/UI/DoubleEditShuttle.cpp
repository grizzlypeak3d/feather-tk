// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DoubleEditShuttle.h>

#include <ftk/UI/DoubleEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ShuttleWidget.h>
#include <ftk/UI/ToolButton.h>

namespace ftk
{
    struct DoubleEditShuttle::Private
    {
        std::shared_ptr<DoubleModel> model;
        bool pressed = false;

        std::shared_ptr<DoubleEdit> edit;
        std::shared_ptr<ShuttleWidget> shuttle;
        std::shared_ptr<DoubleResetButton> resetButton;
        std::shared_ptr<HorizontalLayout> layout;

        std::function<void(double)> callback;
        std::function<void(double, bool)> pressedCallback;
        int blockCallbacks = 0;
    };

    void DoubleEditShuttle::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::DoubleEditShuttle", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.model = model;

        p.edit = DoubleEdit::create(context, p.model);

        p.shuttle = ShuttleWidget::create(context);

        p.resetButton = DoubleResetButton::create(context, p.model);

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.edit->setParent(p.layout);
        p.shuttle->setParent(p.layout);
        p.resetButton->setParent(p.layout);

        p.edit->setCallback(
            [this](double value)
            {
                FTK_P();
                if (p.callback && !p.blockCallbacks)
                {
                    p.callback(value);
                }
                if (p.pressedCallback && !p.blockCallbacks)
                {
                    p.pressedCallback(value, p.pressed);
                }
            });

        p.shuttle->setDeltaCallback(
            [this](int value, bool pressed)
            {
                FTK_P();
                p.pressed = pressed;
                if (pressed)
                {
                    p.model->setValue(p.model->getValue() + p.model->getStep() * value);
                }
                else
                {
                    if (p.pressedCallback && !p.blockCallbacks)
                    {
                        p.pressedCallback(p.model->getValue(), p.pressed);
                    }
                }
            });
    }

    DoubleEditShuttle::DoubleEditShuttle() :
        _p(new Private)
    {}

    DoubleEditShuttle::~DoubleEditShuttle()
    {}

    std::shared_ptr<DoubleEditShuttle> DoubleEditShuttle::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DoubleEditShuttle>(new DoubleEditShuttle);
        out->_init(context, DoubleModel::create(context), parent);
        return out;
    }

    std::shared_ptr<DoubleEditShuttle> DoubleEditShuttle::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DoubleEditShuttle>(new DoubleEditShuttle);
        out->_init(context, model, parent);
        return out;
    }

    double DoubleEditShuttle::getValue() const
    {
        return _p->model->getValue();
    }

    void DoubleEditShuttle::setValue(double value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setValue(value);
        --(p.blockCallbacks);
    }

    void DoubleEditShuttle::setCallback(const std::function<void(double)>& value)
    {
        _p->callback = value;
    }

    void DoubleEditShuttle::setPressedCallback(const std::function<void(double, bool)>& value)
    {
        _p->pressedCallback = value;
    }

    const RangeD& DoubleEditShuttle::getRange() const
    {
        return _p->model->getRange();
    }

    void DoubleEditShuttle::setRange(const RangeD& value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setRange(value);
        --(p.blockCallbacks);
    }

    void DoubleEditShuttle::setRange(double min, double max)
    {
        setRange(RangeD(min, max));
    }

    double DoubleEditShuttle::getStep() const
    {
        return _p->model->getStep();
    }

    void DoubleEditShuttle::setStep(double value)
    {
        _p->model->setStep(value);
    }

    double DoubleEditShuttle::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void DoubleEditShuttle::setLargeStep(double value)
    {
        _p->model->setLargeStep(value);
    }

    double DoubleEditShuttle::getDefaultValue() const
    {
        return _p->model->getDefaultValue();
    }

    void DoubleEditShuttle::setDefaultValue(double value)
    {
        _p->model->setDefaultValue(value);
    }

    const std::shared_ptr<DoubleModel>& DoubleEditShuttle::getModel() const
    {
        return _p->model;
    }

    int DoubleEditShuttle::getPrecision() const
    {
        return _p->edit->getPrecision();
    }

    void DoubleEditShuttle::setPrecision(int value)
    {
        _p->edit->setPrecision(value);
    }

    FontRole DoubleEditShuttle::getFontRole() const
    {
        return _p->edit->getFontRole();
    }

    void DoubleEditShuttle::setFontRole(FontRole value)
    {
        _p->edit->setFontRole(value);
    }

    Size2I DoubleEditShuttle::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void DoubleEditShuttle::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }
}