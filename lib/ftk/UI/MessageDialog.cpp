// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/MessageDialog.h>

#include <ftk/UI/Divider.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ScrollWidget.h>

namespace ftk
{
    class MessageDialogWidget : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent);

        MessageDialogWidget();

    public:
        virtual ~MessageDialogWidget();

        static std::shared_ptr<MessageDialogWidget> create(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent = nullptr);

        Size2I getSizeHint() const override;
        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

        void setCallback(const std::function<void(void)>&);

    private:
        std::shared_ptr<Label> _titleLabel;
        std::shared_ptr<Label> _label;
        std::shared_ptr<ScrollWidget> _scrollWidget;
        std::shared_ptr<PushButton> _okButton;
        std::shared_ptr<VerticalLayout> _layout;
        std::function<void(void)> _callback;
        int _sizeHint = 0;
    };

    void MessageDialogWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::string& text,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::MessageDialogWidget", parent);

        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);

        _titleLabel = Label::create(context, title);
        _titleLabel->setMarginRole(SizeRole::MarginSmall);
        _titleLabel->setBackgroundRole(ColorRole::Button);

        _label = Label::create(context, text);
        _label->setMarginRole(SizeRole::MarginSmall);
        _label->setAlign(HAlign::Left, VAlign::Top);

        _scrollWidget = ScrollWidget::create(context);
        _scrollWidget->setBorder(false);
        _scrollWidget->setSizeHintRole(SizeRole::ScrollAreaSmall);
        _scrollWidget->setWidget(_label);

        _okButton = PushButton::create(context, "OK");

        _layout = VerticalLayout::create(context, shared_from_this());
        _layout->setSpacingRole(SizeRole::None);
        _titleLabel->setParent(_layout);
        Divider::create(context, Orientation::Vertical, _layout);
        _scrollWidget->setParent(_layout);
        Divider::create(context, Orientation::Vertical, _layout);
        auto hLayout = HorizontalLayout::create(context, _layout);
        hLayout->setMarginRole(SizeRole::MarginSmall);
        hLayout->setSpacingRole(SizeRole::SpacingSmall);
        hLayout->addSpacer(Stretch::Expanding);
        _okButton->setParent(hLayout);

        _okButton->setClickedCallback(
            [this]
            {
                if (_callback)
                {
                    _callback();
                }
            });
    }

    MessageDialogWidget::MessageDialogWidget()
    {}

    MessageDialogWidget::~MessageDialogWidget()
    {}

    std::shared_ptr<MessageDialogWidget> MessageDialogWidget::create(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::string& text,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<MessageDialogWidget>(new MessageDialogWidget);
        out->_init(context, title, text, parent);
        return out;
    }

    void MessageDialogWidget::setCallback(const std::function<void(void)>& value)
    {
        _callback = value;
    }
    
    Size2I MessageDialogWidget::getSizeHint() const
    {
        Size2I out = _layout->getSizeHint();
        out.w = std::max(out.w, _sizeHint * 2);
        return out;
    }

    void MessageDialogWidget::setGeometry(const Box2I& value)
    {
        IMouseWidget::setGeometry(value);
        _layout->setGeometry(value);
    }

    void MessageDialogWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        _sizeHint = event.style->getSizeRole(SizeRole::ScrollArea, event.displayScale);
    }

    struct MessageDialog::Private
    {
        std::shared_ptr<MessageDialogWidget> widget;

        std::function<void(void)> callback;
    };

    void MessageDialog::_init(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::string& text,
        const std::shared_ptr<IWidget>& parent)
    {
        IDialog::_init(context, "ftk::MessageDialog", parent);
        FTK_P();

        p.widget = MessageDialogWidget::create(context, title, text, shared_from_this());

        p.widget->setCallback(
            [this]
            {
                if (_p->callback)
                {
                    _p->callback();
                }
            });
    }

    MessageDialog::MessageDialog() :
        _p(new Private)
    {}

    MessageDialog::~MessageDialog()
    {}

    std::shared_ptr<MessageDialog> MessageDialog::create(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::string& text,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<MessageDialog>(new MessageDialog);
        out->_init(context, title, text, parent);
        return out;
    }

    void MessageDialog::setCallback(const std::function<void(void)>& value)
    {
        _p->callback = value;
    }

}