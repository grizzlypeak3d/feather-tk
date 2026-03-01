// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IntEditShuttle.h>

#include <ftk/UI/IntEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ShuttleWidget.h>

namespace ftk
{
    struct IntEditShuttle::Private
    {
        std::shared_ptr<IntModel> model;
        bool pressed = false;

        std::shared_ptr<IntEdit> edit;
        std::shared_ptr<ShuttleWidget> shuttle;
        std::shared_ptr<IntResetButton> resetButton;
        std::shared_ptr<HorizontalLayout> layout;

        std::function<void(int)> callback;
        std::function<void(int, bool)> pressedCallback;
        int blockCallbacks = 0;
    };

    void IntEditShuttle::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::IntEditShuttle", parent);
        FTK_P();

        setHStretch(Stretch::Expanding);

        p.model = model;

        p.edit = IntEdit::create(context, p.model);

        p.shuttle = ShuttleWidget::create(context);

        p.resetButton = IntResetButton::create(context, p.model);

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.edit->setParent(p.layout);
        p.shuttle->setParent(p.layout);
        p.resetButton->setParent(p.layout);

        p.edit->setCallback(
            [this](int value)
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
            [this](int value)
            {
                FTK_P();
                p.model->setValue(p.model->getValue() + p.model->getStep() * value);
            });
        p.shuttle->setActiveCallback(
            [this](bool pressed)
            {
                FTK_P();
                p.pressed = pressed;
                if (!pressed && p.pressedCallback && !p.blockCallbacks)
                {
                    p.pressedCallback(p.model->getValue(), p.pressed);
                }
            });
    }

    IntEditShuttle::IntEditShuttle() :
        _p(new Private)
    {}

    IntEditShuttle::~IntEditShuttle()
    {}

    std::shared_ptr<IntEditShuttle> IntEditShuttle::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<IntEditShuttle>(new IntEditShuttle);
        out->_init(context, IntModel::create(context), parent);
        return out;
    }

    std::shared_ptr<IntEditShuttle> IntEditShuttle::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<IntEditShuttle>(new IntEditShuttle);
        out->_init(context, model, parent);
        return out;
    }

    int IntEditShuttle::getValue() const
    {
        return _p->model->getValue();
    }

    void IntEditShuttle::setValue(int value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setValue(value);
        --(p.blockCallbacks);
    }

    void IntEditShuttle::setCallback(const std::function<void(int)>& value)
    {
        _p->callback = value;
    }

    void IntEditShuttle::setPressedCallback(const std::function<void(int, bool)>& value)
    {
        _p->pressedCallback = value;
    }

    const RangeI& IntEditShuttle::getRange() const
    {
        return _p->model->getRange();
    }

    void IntEditShuttle::setRange(const RangeI& value)
    {
        FTK_P();
        ++(p.blockCallbacks);
        p.model->setRange(value);
        --(p.blockCallbacks);
    }

    void IntEditShuttle::setRange(int min, int max)
    {
        setRange(RangeI(min, max));
    }

    int IntEditShuttle::getStep() const
    {
        return _p->model->getStep();
    }

    void IntEditShuttle::setStep(int value)
    {
        _p->model->setStep(value);
    }

    int IntEditShuttle::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void IntEditShuttle::setLargeStep(int value)
    {
        _p->model->setLargeStep(value);
    }

    int IntEditShuttle::getDefault() const
    {
        return _p->model->getDefault();
    }

    void IntEditShuttle::setDefault(int value)
    {
        _p->model->setDefault(value);
    }

    const std::shared_ptr<IntModel>& IntEditShuttle::getModel() const
    {
        return _p->model;
    }

    FontRole IntEditShuttle::getFontRole() const
    {
        return _p->edit->getFontRole();
    }

    void IntEditShuttle::setFontRole(FontRole value)
    {
        _p->edit->setFontRole(value);
    }

    Size2I IntEditShuttle::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void IntEditShuttle::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }
}
