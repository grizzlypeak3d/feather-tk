// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FloatEditShuttle.h>

#include <ftk/UI/FloatEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ShuttleWidget.h>

namespace ftk
{
    struct FloatEditShuttle::Private
    {
        std::shared_ptr<FloatModel> model;
        bool pressed = false;

        std::shared_ptr<FloatEdit> edit;
        std::shared_ptr<ShuttleWidget> shuttle;
        std::shared_ptr<FloatResetButton> resetButton;
        std::shared_ptr<HorizontalLayout> layout;

        std::function<void(float)> callback;
        std::function<void(float, bool)> pressedCallback;
        int blockCallbacks = 0;
    };

    void FloatEditShuttle::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::FloatEditShuttle", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.model = model;

        p.edit = FloatEdit::create(context, p.model);

        p.shuttle = ShuttleWidget::create(context);

        p.resetButton = FloatResetButton::create(context, p.model);

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.edit->setParent(p.layout);
        p.shuttle->setParent(p.layout);
        p.resetButton->setParent(p.layout);

        p.edit->setCallback(
            [this](float value)
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

    FloatEditShuttle::FloatEditShuttle() :
        _p(new Private)
    {}

    FloatEditShuttle::~FloatEditShuttle()
    {}

    std::shared_ptr<FloatEditShuttle> FloatEditShuttle::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FloatEditShuttle>(new FloatEditShuttle);
        out->_init(context, FloatModel::create(context), parent);
        return out;
    }

    std::shared_ptr<FloatEditShuttle> FloatEditShuttle::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FloatEditShuttle>(new FloatEditShuttle);
        out->_init(context, model, parent);
        return out;
    }

    float FloatEditShuttle::getValue() const
    {
        return _p->model->getValue();
    }

    void FloatEditShuttle::setValue(float value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setValue(value);
        --(p.blockCallbacks);
    }

    void FloatEditShuttle::setCallback(const std::function<void(float)>& value)
    {
        _p->callback = value;
    }

    void FloatEditShuttle::setPressedCallback(const std::function<void(float, bool) > & value)
    {
        _p->pressedCallback = value;
    }

    const RangeF& FloatEditShuttle::getRange() const
    {
        return _p->model->getRange();
    }

    void FloatEditShuttle::setRange(const RangeF& value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setRange(value);
        --(p.blockCallbacks);
    }

    void FloatEditShuttle::setRange(float min, float max)
    {
        setRange(RangeF(min, max));
    }

    float FloatEditShuttle::getStep() const
    {
        return _p->model->getStep();
    }

    void FloatEditShuttle::setStep(float value)
    {
        _p->model->setStep(value);
    }

    float FloatEditShuttle::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void FloatEditShuttle::setLargeStep(float value)
    {
        _p->model->setLargeStep(value);
    }

    float FloatEditShuttle::getDefaultValue() const
    {
        return _p->model->getDefaultValue();
    }

    void FloatEditShuttle::setDefaultValue(float value)
    {
        _p->model->setDefaultValue(value);
    }

    const std::shared_ptr<FloatModel>& FloatEditShuttle::getModel() const
    {
        return _p->model;
    }

    int FloatEditShuttle::getPrecision() const
    {
        return _p->edit->getPrecision();
    }

    void FloatEditShuttle::setPrecision(int value)
    {
        _p->edit->setPrecision(value);
    }

    FontRole FloatEditShuttle::getFontRole() const
    {
        return _p->edit->getFontRole();
    }

    void FloatEditShuttle::setFontRole(FontRole value)
    {
        _p->edit->setFontRole(value);
    }
    
    Size2I FloatEditShuttle::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void FloatEditShuttle::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }
}