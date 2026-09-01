// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/ChoiceDialog.h>

#include <ftk/UI/Divider.h>
#include <ftk/UI/Label.h>
#include <ftk/UI/PushButton.h>
#include <ftk/UI/RowLayout.h>
#include <ftk/UI/ScrollWidget.h>

namespace ftk
{
    class ChoiceDialogWidget : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::vector<std::string>& choices,
            const std::shared_ptr<IWidget>& parent);

        ChoiceDialogWidget();

    public:
        virtual ~ChoiceDialogWidget();

        static std::shared_ptr<ChoiceDialogWidget> create(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::vector<std::string>& choices,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setCallback(const std::function<void(int)>&);

        std::shared_ptr<PushButton> getLastButton() const;

        Size2I getSizeHint() const override;
        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        std::shared_ptr<Label> _label;
        std::shared_ptr<ScrollWidget> _scrollWidget;
        std::vector<std::shared_ptr<PushButton> > _buttons;
        std::shared_ptr<VerticalLayout> _layout;
        std::function<void(int)> _callback;
        int _sizeHint = 0;
    };

    void ChoiceDialogWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const std::vector<std::string>& choices,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::ChoiceDialogWidget", parent);

        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);

        _label = Label::create(context, text);
        _label->setMarginRole(SizeRole::Margin);
        _label->setAlign(HAlign::Left, VAlign::Top);

        _scrollWidget = ScrollWidget::create(context);
        _scrollWidget->setBorder(false);
        _scrollWidget->setSizeHintRole(SizeRole::ScrollAreaSmall);
        _scrollWidget->setWidget(_label);

        _layout = VerticalLayout::create(context, shared_from_this());
        _layout->setSpacingRole(SizeRole::None);
        _scrollWidget->setParent(_layout);
        Divider::create(context, Orientation::Vertical, _layout);
        auto hLayout = HorizontalLayout::create(context, _layout);
        hLayout->setMarginRole(SizeRole::MarginSmall);
        hLayout->setSpacingRole(SizeRole::SpacingSmall);
        hLayout->addSpacer(SizeRole::None, Stretch::Expanding);
        for (size_t i = 0; i < choices.size(); ++i)
        {
            auto button = PushButton::create(context, choices[i], hLayout);
            const int index = static_cast<int>(i);
            button->setClickedCallback(
                [this, index]
                {
                    if (_callback)
                    {
                        _callback(index);
                    }
                });
            _buttons.push_back(button);
        }
    }

    ChoiceDialogWidget::ChoiceDialogWidget()
    {}

    ChoiceDialogWidget::~ChoiceDialogWidget()
    {}

    std::shared_ptr<ChoiceDialogWidget> ChoiceDialogWidget::create(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const std::vector<std::string>& choices,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ChoiceDialogWidget>(new ChoiceDialogWidget);
        out->_init(context, text, choices, parent);
        return out;
    }

    void ChoiceDialogWidget::setCallback(const std::function<void(int)>& value)
    {
        _callback = value;
    }

    std::shared_ptr<PushButton> ChoiceDialogWidget::getLastButton() const
    {
        return !_buttons.empty() ? _buttons.back() : nullptr;
    }

    Size2I ChoiceDialogWidget::getSizeHint() const
    {
        Size2I out = _layout->getSizeHint();
        out.w = std::max(out.w, _sizeHint * 2);
        return out;
    }

    void ChoiceDialogWidget::setGeometry(const Box2I& value)
    {
        IMouseWidget::setGeometry(value);
        _layout->setGeometry(value);
    }

    void ChoiceDialogWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        _sizeHint = event.style->getSizeRole(SizeRole::ScrollArea, event.displayScale);
    }

    struct ChoiceDialog::Private
    {
        std::shared_ptr<ChoiceDialogWidget> widget;

        std::function<void(int)> callback;
    };

    void ChoiceDialog::_init(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::string& text,
        const std::vector<std::string>& choices,
        const std::shared_ptr<IWidget>& parent)
    {
        IDialog::_init(context, "ftk::ChoiceDialog", parent);
        FTK_P();

        setTitle(title);

        p.widget = ChoiceDialogWidget::create(context, text, choices, shared_from_this());

        p.widget->setCallback(
            [this](int value)
            {
                if (_p->callback)
                {
                    _p->callback(value);
                }
            });
    }

    ChoiceDialog::ChoiceDialog() :
        _p(new Private)
    {}

    ChoiceDialog::~ChoiceDialog()
    {}

    std::shared_ptr<ChoiceDialog> ChoiceDialog::create(
        const std::shared_ptr<Context>& context,
        const std::string& title,
        const std::string& text,
        const std::vector<std::string>& choices,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<ChoiceDialog>(new ChoiceDialog);
        out->_init(context, title, text, choices, parent);
        return out;
    }

    void ChoiceDialog::setCallback(const std::function<void(int)>& value)
    {
        _p->callback = value;
    }

    std::shared_ptr<IWidget> ChoiceDialog::getKeyFocus() const
    {
        return _p->widget->getLastButton();
    }
}
