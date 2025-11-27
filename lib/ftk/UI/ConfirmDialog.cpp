// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ConfirmDialog.h>

#include <ftk/UI/Divider.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ScrollWidget.h>

namespace ftk
{
    class ConfirmDialogWidget : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::string& text,
            const std::string& confirm,
            const std::string& cancel,
            const std::shared_ptr<IWidget>& parent);

        ConfirmDialogWidget();

    public:
        virtual ~ConfirmDialogWidget();

        static std::shared_ptr<ConfirmDialogWidget> create(
            const std::shared_ptr<Context>&,
            const std::string& title,
            const std::string& text,
            const std::string& confirm,
            const std::string& cancel,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setCallback(const std::function<void(bool)>&);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        std::shared_ptr<Label> _titleLabel;
        std::shared_ptr<Label> _label;
        std::shared_ptr<ScrollWidget> _scrollWidget;
        std::shared_ptr<PushButton> _okButton;
        std::shared_ptr<PushButton> _cancelButton;
        std::shared_ptr<VerticalLayout> _layout;
        std::function<void(bool)> _callback;
    };

    void ConfirmDialogWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::string& text,
        const std::string& confirm,
        const std::string& cancel,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::ConfirmDialogWidget", parent);

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

        _okButton = PushButton::create(context, confirm);
        _cancelButton = PushButton::create(context, cancel);

        _layout = VerticalLayout::create(context, shared_from_this());
        _layout->setSpacingRole(SizeRole::None);
        _titleLabel->setParent(_layout);
        Divider::create(context, Orientation::Vertical, _layout);
        _scrollWidget->setParent(_layout);
        Divider::create(context, Orientation::Vertical, _layout);
        auto hLayout = HorizontalLayout::create(context, _layout);
        hLayout->setMarginRole(SizeRole::MarginSmall);
        hLayout->setSpacingRole(SizeRole::SpacingSmall);
        hLayout->addSpacer(SizeRole::None, Stretch::Expanding);
        _okButton->setParent(hLayout);
        _cancelButton->setParent(hLayout);

        _okButton->setClickedCallback(
            [this]
            {
                if (_callback)
                {
                    _callback(true);
                }
            });

        _cancelButton->setClickedCallback(
            [this]
            {
                if (_callback)
                {
                    _callback(false);
                }
            });
    }

    ConfirmDialogWidget::ConfirmDialogWidget()
    {}

    ConfirmDialogWidget::~ConfirmDialogWidget()
    {}

    std::shared_ptr<ConfirmDialogWidget> ConfirmDialogWidget::create(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::string& text,
        const std::string& confirm,
        const std::string& cancel,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ConfirmDialogWidget>(new ConfirmDialogWidget);
        out->_init(context, title, text, confirm, cancel, parent);
        return out;
    }

    void ConfirmDialogWidget::setCallback(const std::function<void(bool)>& value)
    {
        _callback = value;
    }

    void ConfirmDialogWidget::setGeometry(const Box2I& value)
    {
        IMouseWidget::setGeometry(value);
        _layout->setGeometry(value);
    }

    void ConfirmDialogWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        const int sa = event.style->getSizeRole(SizeRole::ScrollArea, event.displayScale);
        Size2I sizeHint = _layout->getSizeHint();
        sizeHint.w = std::max(sizeHint.w, sa * 2);
        setSizeHint(sizeHint);
    }

    struct ConfirmDialog::Private
    {
        std::shared_ptr<ConfirmDialogWidget> widget;

        std::function<void(bool)> callback;
    };

    void ConfirmDialog::_init(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::string& text,
        const std::string& confirm,
        const std::string& cancel,
        const std::shared_ptr<IWidget>& parent)
    {
        IDialog::_init(context, "ftk::ConfirmDialog", parent);
        FTK_P();

        p.widget = ConfirmDialogWidget::create(context, title, text, confirm, cancel, shared_from_this());

        p.widget->setCallback(
            [this](bool value)
            {
                if (_p->callback)
                {
                    _p->callback(value);
                }
            });
    }

    ConfirmDialog::ConfirmDialog() :
        _p(new Private)
    {}

    ConfirmDialog::~ConfirmDialog()
    {}

    std::shared_ptr<ConfirmDialog> ConfirmDialog::create(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::string& text,
        const std::string& confirm,
        const std::string& cancel,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ConfirmDialog>(new ConfirmDialog);
        out->_init(context, title, text, confirm, cancel, parent);
        return out;
    }

    void ConfirmDialog::setCallback(const std::function<void(bool)>& value)
    {
        _p->callback = value;
    }
}