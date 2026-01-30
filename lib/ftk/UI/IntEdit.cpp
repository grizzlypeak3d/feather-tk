// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/IntEdit.h>

#include <ftk/UI/IncButtons.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ToolButton.h>

#include <ftk/Core/Format.h>

namespace ftk
{
    struct IntEdit::Private
    {
        std::shared_ptr<IntModel> model;
        int digits = 3;
        std::shared_ptr<LineEdit> lineEdit;
        std::shared_ptr<IncButtons> incButtons;
        std::shared_ptr<HorizontalLayout> layout;
        std::function<void(int)> callback;
        bool blockCallbacks = false;
        std::shared_ptr<Observer<int> > valueObserver;
        std::shared_ptr<Observer<RangeI> > rangeObserver;
    };

    void IntEdit::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::IntEdit", parent);
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
                _p->model->setValue(std::atoi(value.c_str()));
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

        p.valueObserver = Observer<int>::create(
            p.model->observeValue(),
            [this](int value)
            {
                FTK_P();
                _textUpdate();
                if (p.callback && !p.blockCallbacks)
                {
                    p.callback(value);
                }
            });

        p.rangeObserver = Observer<RangeI>::create(
            p.model->observeRange(),
            [this](const RangeI& value)
            {
                _p->digits = digits(value.max());
                setSizeUpdate();
                _textUpdate();
            });

        _textUpdate();
    }

    IntEdit::IntEdit() :
        _p(new Private)
    {}

    IntEdit::~IntEdit()
    {}

    std::shared_ptr<IntEdit> IntEdit::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<IntEdit>(new IntEdit);
        out->_init(context, IntModel::create(context), parent);
        return out;
    }

    std::shared_ptr<IntEdit> IntEdit::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<IntEdit>(new IntEdit);
        out->_init(context, model, parent);
        return out;
    }

    int IntEdit::getValue() const
    {
        return _p->model->getValue();
    }

    void IntEdit::setValue(int value)
    {
        FTK_P();
        p.blockCallbacks = true;
        p.model->setValue(value);
        p.blockCallbacks = false;
    }

    void IntEdit::setCallback(const std::function<void(int)>& value)
    {
        _p->callback = value;
    }

    const RangeI& IntEdit::getRange() const
    {
        return _p->model->getRange();
    }

    void IntEdit::setRange(const RangeI& value)
    {
        FTK_P();
        p.blockCallbacks = true;
        p.model->setRange(value);
        p.blockCallbacks = false;
    }

    void IntEdit::setRange(int min, int max)
    {
        setRange(RangeI(min, max));
    }

    int IntEdit::getStep() const
    {
        return _p->model->getStep();
    }

    void IntEdit::setStep(int value)
    {
        _p->model->setStep(value);
    }

    int IntEdit::getLargeStep() const
    {
        return _p->model->getLargeStep();
    }

    void IntEdit::setLargeStep(int value)
    {
        _p->model->setLargeStep(value);
    }

    int IntEdit::getDefaultValue() const
    {
        return _p->model->getDefaultValue();
    }

    void IntEdit::setDefaultValue(int value)
    {
        _p->model->setDefaultValue(value);
    }

    const std::shared_ptr<IntModel>& IntEdit::getModel() const
    {
        return _p->model;
    }

    FontRole IntEdit::getFontRole() const
    {
        return _p->lineEdit->getFontRole();
    }

    void IntEdit::setFontRole(FontRole value)
    {
        _p->lineEdit->setFontRole(value);
    }

    Size2I IntEdit::getSizeHint() const
    {
        return _p->layout->getSizeHint();
    }

    void IntEdit::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->layout->setGeometry(value);
    }

    void IntEdit::scrollEvent(ScrollEvent& event)
    {
        FTK_P();
        event.accept = true;
        const int step = event.modifiers & static_cast<int>(KeyModifier::Shift) ?
            p.model->getLargeStep() :
            p.model->getStep();
        p.model->setValue(p.model->getValue() + step * event.value.y);
    }

    void IntEdit::keyPressEvent(KeyEvent& event)
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

    void IntEdit::keyReleaseEvent(KeyEvent& event)
    {
        event.accept = true;
    }

    void IntEdit::_textUpdate()
    {
        FTK_P();
        std::string text;
        std::string format;
        if (p.model)
        {
            text = Format("{0}").arg(p.model->getValue());
            int width = std::max(p.digits, 2);
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

    struct IntResetButton::Private
    {
        std::shared_ptr<IntModel> model;

        std::shared_ptr<ToolButton> resetButton;

        std::shared_ptr<Observer<int> > valueObserver;
        std::shared_ptr<Observer<bool> > hasDefaultObserver;
        std::shared_ptr<Observer<int> > defaultObserver;
    };

    void IntResetButton::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::IntResetButton", parent);
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

        p.valueObserver = Observer<int>::create(
            p.model->observeValue(),
            [this](int)
            {
                _widgetUpdate();
            });

        p.hasDefaultObserver = Observer<bool>::create(
            p.model->observeHasDefaultValue(),
            [this](bool)
            {
                _widgetUpdate();
            });

        p.defaultObserver = Observer<int>::create(
            p.model->observeDefaultValue(),
            [this](int)
            {
                _widgetUpdate();
            });
    }

    IntResetButton::IntResetButton() :
        _p(new Private)
    {}

    IntResetButton::~IntResetButton()
    {}

    std::shared_ptr<IntResetButton> IntResetButton::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IntModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<IntResetButton>(new IntResetButton);
        out->_init(context, model, parent);
        return out;
    }

    Size2I IntResetButton::getSizeHint() const
    {
        return _p->resetButton->getSizeHint();
    }

    void IntResetButton::setGeometry(const Box2I& value)
    {
        IWidget::setGeometry(value);
        _p->resetButton->setGeometry(value);
    }

    void IntResetButton::_widgetUpdate()
    {
        FTK_P();
        setVisible(p.model->hasDefaultValue());
        setEnabled(p.model->getValue() != p.model->getDefaultValue());
    }
}