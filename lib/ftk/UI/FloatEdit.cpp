// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/FloatEdit.h>

#include <ftk/UI/IncButtons.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/Core/Format.h>

namespace ftk
{
    struct FloatEdit::Private
    {
        std::shared_ptr<FloatModel> model;
        int digits = 3;
        int precision = 2;
        std::shared_ptr<LineEdit> lineEdit;
        std::shared_ptr<IncButtons> incButtons;
        std::shared_ptr<HorizontalLayout> layout;
        std::function<void(float)> callback;
        bool blockCallbacks = false;
        std::shared_ptr<Observer<float> > valueObserver;
        std::shared_ptr<Observer<RangeF> > rangeObserver;
    };

    void FloatEdit::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::FloatEdit", parent);
        FTK_P();

        p.model = model;

        p.lineEdit = LineEdit::create(context, shared_from_this());
        p.lineEdit->setFontRole(FontRole::Mono);

        p.incButtons = IncButtons::create(context);

        p.layout = HorizontalLayout::create(context, shared_from_this());
        p.layout->setSpacingRole(SizeRole::SpacingTool);
        p.lineEdit->setParent(p.layout);
        p.incButtons->setParent(p.layout);

        p.lineEdit->setTextCallback(
            [this](const std::string& value)
            {
                _p->model->setValue(std::atof(value.c_str()));
                _textUpdate();
            });
        p.lineEdit->setFocusCallback(
            [this](bool value)
            {
                if (!value)
                {
                    _textUpdate();
                }
            });

        p.incButtons->setIncCallback(
            [this]
            {
                _p->model->incrementStep();
            });
        p.incButtons->setDecCallback(
            [this]
            {
                _p->model->decrementStep();
            });

        p.valueObserver = Observer<float>::create(
            p.model->observeValue(),
            [this](float value)
            {
                FTK_P();
                _textUpdate();
                if (p.callback && !p.blockCallbacks)
                {
                    p.callback(value);
                }
            });

        p.rangeObserver = Observer<RangeF>::create(
            p.model->observeRange(),
            [this](const RangeF& value)
            {
                _p->digits = digits(value.max());
                setSizeUpdate();
                _textUpdate();
            });

        _textUpdate();
    }

    FloatEdit::FloatEdit() :
        _p(new Private)
    {}

    FloatEdit::~FloatEdit()
    {}

    std::shared_ptr<FloatEdit> FloatEdit::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FloatEdit>(new FloatEdit);
        out->_init(context, FloatModel::create(context), parent);
        return out;
    }

    std::shared_ptr<FloatEdit> FloatEdit::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FloatEdit>(new FloatEdit);
        out->_init(context, model, parent);
        return out;
    }

    float FloatEdit::getValue() const
    {
        return _p->model->getValue();
    }

    void FloatEdit::setValue(float value)
    {
        FTK_P();
        p.blockCallbacks = true;
        p.model->setValue(value);
        p.blockCallbacks = false;
    }

    void FloatEdit::setCallback(const std::function<void(float)>& value)
    {
        _p->callback = value;
    }

    const RangeF& FloatEdit::getRange() const
    {
        return _p->model->getRange();
    }

    void FloatEdit::setRange(const RangeF& value)
    {
        FTK_P();
        p.blockCallbacks = true;
        p.model->setRange(value);
        p.blockCallbacks = false;
    }

    void FloatEdit::setRange(float min, float max)
    {
        setRange(RangeF(min, max));
    }

    float FloatEdit::getStep() const
    {
        return _p->model->getStep();
    }

    void FloatEdit::setStep(float value)
    {
        _p->model->setStep(value);
    }

    float FloatEdit::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void FloatEdit::setLargeStep(float value)
    {
        _p->model->setLargeStep(value);
    }

    float FloatEdit::getDefaultValue() const
    {
        return _p->model->getDefaultValue();
    }

    void FloatEdit::setDefaultValue(float value)
    {
        _p->model->setDefaultValue(value);
    }

    const std::shared_ptr<FloatModel>& FloatEdit::getModel() const
    {
        return _p->model;
    }

    int FloatEdit::getPrecision() const
    {
        return _p->precision;
    }

    void FloatEdit::setPrecision(int value)
    {
        FTK_P();
        if (value == p.precision)
            return;
        p.precision = value;
        _textUpdate();
    }

    FontRole FloatEdit::getFontRole() const
    {
        return _p->lineEdit->getFontRole();
    }

    void FloatEdit::setFontRole(FontRole value)
    {
        _p->lineEdit->setFontRole(value);
    }
    
    Size2I FloatEdit::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void FloatEdit::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    void FloatEdit::scrollEvent(ScrollEvent& event)
    {
        FTK_P();
        if (p.lineEdit->hasKeyFocus())
        {
            event.accept = true;
            const float step = event.modifiers & static_cast<int>(KeyModifier::Shift) ?
                p.model->getLargeStep() :
                p.model->getStep();
            p.model->setValue(p.model->getValue() + step * event.value.y);
        }
    }

    void FloatEdit::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        if (isEnabled() &&
            p.lineEdit->hasKeyFocus() &&
            p.model &&
            0 == event.modifiers)
        {
            switch (event.key)
            {
            case Key::Down:
                event.accept = true;
                p.model->decrementStep();
                break;
            case Key::Up:
                event.accept = true;
                p.model->incrementStep();
                break;
            case Key::PageUp:
                event.accept = true;
                p.model->incrementLargeStep();
                break;
            case Key::PageDown:
                event.accept = true;
                p.model->decrementLargeStep();
                break;
            default: break;
            }
        }
    }

    void FloatEdit::keyReleaseEvent(KeyEvent& event)
    {
        event.accept = true;
    }

    void FloatEdit::_textUpdate()
    {
        FTK_P();
        std::string text;
        std::string format;
        if (p.model)
        {
            text = Format("{0}").arg(p.model->getValue(), p.precision);
            int width = p.digits + 1 + p.precision;
            if (p.model->getRange().min() < 0 ||
                p.model->getRange().max() < 0)
            {
                ++width;
            }
            format = Format("{0}").arg(0, width);
        }
        p.lineEdit->setText(text);
        p.lineEdit->setFormat(format);
    }

    struct FloatResetButton::Private
    {
        std::shared_ptr<FloatModel> model;

        std::shared_ptr<ToolButton> resetButton;

        std::shared_ptr<Observer<float> > valueObserver;
        std::shared_ptr<Observer<bool> > hasDefaultObserver;
        std::shared_ptr<Observer<float> > defaultObserver;
    };

    void FloatResetButton::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::FloatResetButton", parent);
        FTK_P();

        p.model = model;

        p.resetButton = ToolButton::create(context, shared_from_this());
        p.resetButton->setIcon("Reset");
        p.resetButton->setTooltip("Reset to the default value");

        p.resetButton->setClickedCallback(
            [this]
            {
                _p->model->setDefaultValue();
            });

        p.valueObserver = Observer<float>::create(
            p.model->observeValue(),
            [this](float)
            {
                _widgetUpdate();
            });

        p.hasDefaultObserver = Observer<bool>::create(
            p.model->observeHasDefaultValue(),
            [this](bool)
            {
                _widgetUpdate();
            });

        p.defaultObserver = Observer<float>::create(
            p.model->observeDefaultValue(),
            [this](float)
            {
                _widgetUpdate();
            });
    }

    FloatResetButton::FloatResetButton() :
        _p(new Private)
    {}

    FloatResetButton::~FloatResetButton()
    {}

    std::shared_ptr<FloatResetButton> FloatResetButton::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<FloatModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<FloatResetButton>(new FloatResetButton);
        out->_init(context, model, parent);
        return out;
    }

    Size2I FloatResetButton::getSizeHint() const
    {
        return _p->resetButton->getSizeHint();
    }

    void FloatResetButton::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->resetButton->setGeometry(value);
    }

    void FloatResetButton::_widgetUpdate()
    {
        FTK_P();
        setVisible(p.model->hasDefaultValue());
        setEnabled(p.model->getValue() != p.model->getDefaultValue());
    }
}