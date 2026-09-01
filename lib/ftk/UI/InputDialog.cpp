// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/InputDialog.h>

#include <ftk/UI/Divider.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/LineEdit.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/RowLayout.h>

namespace ftk
{
    class InputDialogWidget : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::string& value,
            const std::string& confirm,
            const std::string& cancel,
            const std::shared_ptr<IWidget>& parent);

        InputDialogWidget();

    public:
        virtual ~InputDialogWidget();

        static std::shared_ptr<InputDialogWidget> create(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::string& value,
            const std::string& confirm,
            const std::string& cancel,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setCallback(const std::function<void(const std::string&)>&);
        void setCancelCallback(const std::function<void(void)>&);

        const std::shared_ptr<LineEdit>& getLineEdit() const;

        Size2I getSizeHint() const override;
        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _accept();

        std::shared_ptr<Label> _label;
        std::shared_ptr<LineEdit> _lineEdit;
        std::shared_ptr<PushButton> _okButton;
        std::shared_ptr<PushButton> _cancelButton;
        std::shared_ptr<VerticalLayout> _layout;
        std::function<void(const std::string&)> _callback;
        std::function<void(void)> _cancelCallback;
        //! Accepting closes the dialog, and closing releases the key focus,
        //! which is a second door back into this widget while the first call
        //! is still on the stack. The latch keeps the answer to one.
        bool _accepted = false;
        int _sizeHint = 0;
    };

    void InputDialogWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const std::string& value,
        const std::string& confirm,
        const std::string& cancel,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::InputDialogWidget", parent);

        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);

        _label = Label::create(context, text);
        _label->setAlign(HAlign::Left, VAlign::Center);

        _lineEdit = LineEdit::create(context);
        _lineEdit->setText(value);
        _lineEdit->setSelectAllOnFocus(true);
        // Return is the commitment; a commit fired again when the closing
        // dialog takes the focus away would answer twice.
        _lineEdit->setCallbackOnFocusLost(false);

        _okButton = PushButton::create(context, confirm);
        _cancelButton = PushButton::create(context, cancel);

        _layout = VerticalLayout::create(context, shared_from_this());
        _layout->setSpacingRole(SizeRole::None);
        auto vLayout = VerticalLayout::create(context, _layout);
        vLayout->setMarginRole(SizeRole::Margin);
        vLayout->setSpacingRole(SizeRole::SpacingSmall);
        _label->setParent(vLayout);
        _lineEdit->setParent(vLayout);
        Divider::create(context, Orientation::Vertical, _layout);
        auto hLayout = HorizontalLayout::create(context, _layout);
        hLayout->setMarginRole(SizeRole::MarginSmall);
        hLayout->setSpacingRole(SizeRole::SpacingSmall);
        hLayout->addSpacer(SizeRole::None, Stretch::Expanding);
        _okButton->setParent(hLayout);
        _cancelButton->setParent(hLayout);

        _lineEdit->setCallback(
            [this](const std::string&)
            {
                _accept();
            });

        _okButton->setClickedCallback(
            [this]
            {
                _accept();
            });

        _cancelButton->setClickedCallback(
            [this]
            {
                if (_cancelCallback)
                {
                    _cancelCallback();
                }
            });
    }

    InputDialogWidget::InputDialogWidget()
    {}

    InputDialogWidget::~InputDialogWidget()
    {}

    std::shared_ptr<InputDialogWidget> InputDialogWidget::create(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const std::string& value,
        const std::string& confirm,
        const std::string& cancel,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<InputDialogWidget>(new InputDialogWidget);
        out->_init(context, text, value, confirm, cancel, parent);
        return out;
    }

    void InputDialogWidget::setCallback(const std::function<void(const std::string&)>& value)
    {
        _callback = value;
    }

    void InputDialogWidget::setCancelCallback(const std::function<void(void)>& value)
    {
        _cancelCallback = value;
    }

    const std::shared_ptr<LineEdit>& InputDialogWidget::getLineEdit() const
    {
        return _lineEdit;
    }

    void InputDialogWidget::_accept()
    {
        if (_accepted)
        {
            return;
        }
        _accepted = true;
        if (_callback)
        {
            _callback(_lineEdit->getText());
        }
    }

    Size2I InputDialogWidget::getSizeHint() const
    {
        Size2I out = _layout->getSizeHint();
        out.w = std::max(out.w, _sizeHint * 2);
        return out;
    }

    void InputDialogWidget::setGeometry(const Box2I& value)
    {
        IMouseWidget::setGeometry(value);
        _layout->setGeometry(value);
    }

    void InputDialogWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        _sizeHint = event.style->getSizeRole(SizeRole::ScrollArea, event.displayScale);
    }

    struct InputDialog::Private
    {
        std::shared_ptr<InputDialogWidget> widget;

        std::function<void(const std::string&)> callback;
    };

    void InputDialog::_init(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::string& text,
        const std::string& value,
        const std::string& confirm,
        const std::string& cancel,
        const std::shared_ptr<IWidget>& parent)
    {
        IDialog::_init(context, "ftk::InputDialog", parent);
        FTK_P();

        setTitle(title);

        p.widget = InputDialogWidget::create(
            context, text, value, confirm, cancel, shared_from_this());

        p.widget->setCallback(
            [this](const std::string& value)
            {
                if (_p->callback)
                {
                    _p->callback(value);
                }
            });
        p.widget->setCancelCallback(
            [this]
            {
                close();
            });
    }

    InputDialog::InputDialog() :
        _p(new Private)
    {}

    InputDialog::~InputDialog()
    {}

    std::shared_ptr<InputDialog> InputDialog::create(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::string& text,
        const std::string& value,
        const std::string& confirm,
        const std::string& cancel,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<InputDialog>(new InputDialog);
        out->_init(context, title, text, value, confirm, cancel, parent);
        return out;
    }

    void InputDialog::setCallback(const std::function<void(const std::string&)>& value)
    {
        _p->callback = value;
    }

    std::shared_ptr<IWidget> InputDialog::getKeyFocus() const
    {
        // Typing is what the dialog is for; the field also selects its
        // contents on focus, so typing replaces the suggestion and Return
        // keeps it.
        return _p->widget->getLineEdit();
    }
}
