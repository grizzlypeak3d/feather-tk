// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/DoubleEdit.h>

#include <ftk/UI/IncButtons.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/Core/Format.h>

namespace ftk
{
    struct DoubleEdit::Private
    {
        std::shared_ptr<DoubleModel> model;
        int digits = 3;
        int precision = 2;

        std::shared_ptr<LineEdit> lineEdit;
        std::shared_ptr<IncButtons> incButtons;
        std::shared_ptr<HorizontalLayout> layout;

        std::function<void(double)> callback;
        std::shared_ptr<Observer<double> > valueObserver;
        std::shared_ptr<Observer<RangeD> > rangeObserver;
    };

    void DoubleEdit::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::DoubleEdit", parent);
        FTK_P();

        p.model = model;

        p.lineEdit = LineEdit::create(context);
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

        p.valueObserver = Observer<double>::create(
            p.model->observeValue(),
            [this](double value)
            {
                _textUpdate();
                if (_p->callback)
                {
                    _p->callback(value);
                }
            });

        p.rangeObserver = Observer<RangeD>::create(
            p.model->observeRange(),
            [this](const RangeD& value)
            {
                _p->digits = digits(value.max());
                setSizeUpdate();
                _textUpdate();
            });

        _textUpdate();
    }

    DoubleEdit::DoubleEdit() :
        _p(new Private)
    {}

    DoubleEdit::~DoubleEdit()
    {}

    std::shared_ptr<DoubleEdit> DoubleEdit::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DoubleEdit>(new DoubleEdit);
        out->_init(context, DoubleModel::create(context), parent);
        return out;
    }

    std::shared_ptr<DoubleEdit> DoubleEdit::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DoubleEdit>(new DoubleEdit);
        out->_init(context, model, parent);
        return out;
    }

    double DoubleEdit::getValue() const
    {
        return _p->model->getValue();
    }

    void DoubleEdit::setValue(double value)
    {
        _p->model->setValue(value);
    }

    void DoubleEdit::setCallback(const std::function<void(double)>& value)
    {
        _p->callback = value;
    }

    const RangeD& DoubleEdit::getRange() const
    {
        return _p->model->getRange();
    }

    void DoubleEdit::setRange(const RangeD& value)
    {
        _p->model->setRange(value);
    }

    void DoubleEdit::setRange(double min, double max)
    {
        _p->model->setRange(RangeD(min, max));
    }

    double DoubleEdit::getStep() const
    {
        return _p->model->getStep();
    }

    void DoubleEdit::setStep(double value)
    {
        _p->model->setStep(value);
    }

    double DoubleEdit::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void DoubleEdit::setLargeStep(double value)
    {
        _p->model->setLargeStep(value);
    }

    double DoubleEdit::getDefaultValue() const
    {
        return _p->model->getDefaultValue();
    }

    void DoubleEdit::setDefaultValue(double value)
    {
        _p->model->setDefaultValue(value);
    }

    const std::shared_ptr<DoubleModel>& DoubleEdit::getModel() const
    {
        return _p->model;
    }

    int DoubleEdit::getPrecision() const
    {
        return _p->precision;
    }

    void DoubleEdit::setPrecision(int value)
    {
        FTK_P();
        if (value == p.precision)
            return;
        p.precision = value;
        _textUpdate();
    }

    FontRole DoubleEdit::getFontRole() const
    {
        return _p->lineEdit->getFontRole();
    }

    void DoubleEdit::setFontRole(FontRole value)
    {
        _p->lineEdit->setFontRole(value);
    }

    ColorRole DoubleEdit::getBorderRole() const
    {
        return _p->lineEdit->getBorderRole();
    }

    void DoubleEdit::setBorderRole(ColorRole value)
    {
        _p->lineEdit->setBorderRole(value);
    }

    Size2I DoubleEdit::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void DoubleEdit::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    void DoubleEdit::scrollEvent(ScrollEvent& event)
    {
        FTK_P();
        event.accept = true;
        const double step = event.modifiers & static_cast<int>(KeyModifier::Shift) ?
            p.model->getLargeStep() :
            p.model->getStep();
        p.model->setValue(p.model->getValue() + step * event.value.y);
    }

    void DoubleEdit::keyPressEvent(KeyEvent& event)
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

    void DoubleEdit::keyReleaseEvent(KeyEvent& event)
    {
        event.accept = true;
    }

    void DoubleEdit::_textUpdate()
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

    struct DoubleResetButton::Private
    {
        std::shared_ptr<DoubleModel> model;

        std::shared_ptr<ToolButton> resetButton;

        std::shared_ptr<Observer<double> > valueObserver;
        std::shared_ptr<Observer<bool> > hasDefaultObserver;
        std::shared_ptr<Observer<double> > defaultObserver;
    };

    void DoubleResetButton::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::DoubleResetButton", parent);
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

        p.valueObserver = Observer<double>::create(
            p.model->observeValue(),
            [this](double)
            {
                _widgetUpdate();
            });

        p.hasDefaultObserver = Observer<bool>::create(
            p.model->observeHasDefaultValue(),
            [this](bool)
            {
                _widgetUpdate();
            });

        p.defaultObserver = Observer<double>::create(
            p.model->observeDefaultValue(),
            [this](double)
            {
                _widgetUpdate();
            });
    }

    DoubleResetButton::DoubleResetButton() :
        _p(new Private)
    {}

    DoubleResetButton::~DoubleResetButton()
    {}

    std::shared_ptr<DoubleResetButton> DoubleResetButton::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<DoubleModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<DoubleResetButton>(new DoubleResetButton);
        out->_init(context, model, parent);
        return out;
    }
    
    Size2I DoubleResetButton::getSizeHint() const
    {
        return _p->resetButton->getSizeHint();
    }

    void DoubleResetButton::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->resetButton->setGeometry(value);
    }

    void DoubleResetButton::_widgetUpdate()
    {
        FTK_P();
        setVisible(p.model->hasDefaultValue());
        setEnabled(p.model->getValue() != p.model->getDefaultValue());
    }
}