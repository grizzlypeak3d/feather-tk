// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/BellowsPrivate.h>

#include <ftk/UI/RowLayout.h>

#include <algorithm>

namespace ftk
{
    namespace
    {
        //! The header: the title button across it, a check widget over the
        //! room the button leaves between its arrow and its text, and a
        //! tool widget at the right.
        class BellowsHeader : public IWidget
        {
        protected:
            void _init(
                const std::shared_ptr<Context>& context,
                const std::shared_ptr<BellowsButton>& button)
            {
                IWidget::_init(context, "ftk::BellowsHeader", nullptr);
                // The title button only paints its own geometry, so without
                // this a tool widget sits on a hole in the header.
                setBackgroundRole(ColorRole::Header);
                _button = button;
                _button->setParent(shared_from_this());
            }

        public:
            static std::shared_ptr<BellowsHeader> create(
                const std::shared_ptr<Context>& context,
                const std::shared_ptr<BellowsButton>& button)
            {
                auto out = std::shared_ptr<BellowsHeader>(new BellowsHeader);
                out->_init(context, button);
                return out;
            }

            const std::shared_ptr<IWidget>& getCheckWidget() const { return _checkWidget; }
            const std::shared_ptr<IWidget>& getToolWidget() const { return _toolWidget; }

            void setCheckWidget(const std::shared_ptr<IWidget>& value)
            {
                if (value == _checkWidget)
                    return;
                if (_checkWidget)
                {
                    if (auto button = std::dynamic_pointer_cast<IButton>(_checkWidget))
                    {
                        button->setMouseHighlight(true);
                        button->setHoveredCallback(nullptr);
                        button->setAcceptsKeyFocus(_checkAcceptsKeyFocus);
                    }
                    _button->setSpaceCallback(nullptr);
                    _checkWidget->setParent(nullptr);
                }
                _checkWidget = value;
                _button->setIndentHovered(false);
                if (_checkWidget)
                {
                    // After the button, so it is drawn over it and takes
                    // its own clicks.
                    _checkWidget->setParent(shared_from_this());
                    // One highlight for the whole header: the box's own
                    // inside it, appearing as the header's went out, read
                    // as a widget within a widget.
                    if (auto button = std::dynamic_pointer_cast<IButton>(_checkWidget))
                    {
                        button->setMouseHighlight(false);
                        // One tab stop for the header too, rather than a
                        // focus ring inside the header's: Return opens and
                        // closes it, and Space checks the box, as Space
                        // does a focused check box.
                        _checkAcceptsKeyFocus = button->acceptsKeyFocus();
                        button->setAcceptsKeyFocus(false);
                        std::weak_ptr<IButton> checkWeak(button);
                        _button->setSpaceCallback(
                            [checkWeak]
                            {
                                if (auto check = checkWeak.lock())
                                {
                                    if (check->isEnabled())
                                    {
                                        check->click();
                                    }
                                }
                            });
                        std::weak_ptr<BellowsButton> weak(_button);
                        button->setHoveredCallback(
                            [weak](bool value)
                            {
                                if (auto header = weak.lock())
                                {
                                    header->setIndentHovered(value);
                                }
                            });
                    }
                }
                setSizeUpdate();
                setDrawUpdate();
            }

            void setToolWidget(const std::shared_ptr<IWidget>& value)
            {
                if (value == _toolWidget)
                    return;
                if (_toolWidget)
                {
                    _toolWidget->setParent(nullptr);
                }
                _toolWidget = value;
                if (_toolWidget)
                {
                    _toolWidget->setParent(shared_from_this());
                }
                setSizeUpdate();
                setDrawUpdate();
            }

            void sizeHintEvent(const SizeHintEvent& event) override
            {
                IWidget::sizeHintEvent(event);
                const int pad = event.style->getSizeRole(SizeRole::LabelPad, event.displayScale);
                const Size2I checkHint = _checkWidget ? _checkWidget->getSizeHint() : Size2I();
                _button->setTextIndent(_checkWidget ? checkHint.w + pad : 0);
                const Size2I buttonHint = _button->getSizeHint();
                const Size2I toolHint = _toolWidget ? _toolWidget->getSizeHint() : Size2I();
                _sizeHint = Size2I(
                    buttonHint.w + toolHint.w,
                    std::max(std::max(buttonHint.h, toolHint.h), checkHint.h));
            }

            Size2I getSizeHint() const override
            {
                return _sizeHint;
            }

            void setGeometry(const Box2I& value) override
            {
                IWidget::setGeometry(value);
                const int toolW = _toolWidget ? _toolWidget->getSizeHint().w : 0;
                _button->setGeometry(Box2I(value.x(), value.y(), value.w() - toolW, value.h()));
                if (_toolWidget)
                {
                    _toolWidget->setGeometry(Box2I(value.x() + value.w() - toolW, value.y(), toolW, value.h()));
                }
                if (_checkWidget)
                {
                    const Size2I hint = _checkWidget->getSizeHint();
                    _checkWidget->setGeometry(Box2I(
                        _button->getIndentX(),
                        value.y() + (value.h() - hint.h) / 2,
                        hint.w,
                        hint.h));
                }
            }

        private:
            std::shared_ptr<BellowsButton> _button;
            std::shared_ptr<IWidget> _checkWidget;
            std::shared_ptr<IWidget> _toolWidget;
            Size2I _sizeHint;
            bool _checkAcceptsKeyFocus = true;
        };
    }

    struct Bellows::Private
    {
        std::shared_ptr<BellowsButton> button;
        std::shared_ptr<IWidget> widget;
        std::shared_ptr<BellowsHeader> header;
        std::shared_ptr<VerticalLayout> layout;
        std::function<void(bool)> openCallback;
    };

    void Bellows::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::Bellows", parent);
        FTK_P();

        p.button = BellowsButton::create(context);
        p.button->setCheckable(true);
        p.button->setIcon("BellowsClosed");
        p.button->setCheckedIcon("BellowsOpen");
        p.button->setHStretch(Stretch::Expanding);

        p.header = BellowsHeader::create(context, p.button);

        p.layout = VerticalLayout::create(context);
        _setWidget(p.layout);
        p.layout->setSpacingRole(SizeRole::None);
        p.header->setParent(p.layout);

        p.button->setCheckedCallback(
            [this](bool value)
            {
                setOpen(value);
            });
    }

    Bellows::Bellows() :
        _p(new Private)
    {}

    Bellows::~Bellows()
    {}

    std::shared_ptr<Bellows> Bellows::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<Bellows>(new Bellows);
        out->_init(context, parent);
        return out;
    }

    std::shared_ptr<Bellows> Bellows::create(
        const std::shared_ptr<Context>& context,
        const std::string& text,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = create(context, parent);
        out->setText(text);
        return out;
    }

    const std::string& Bellows::getText() const
    {
        return _p->button->getText();
    }

    void Bellows::setText(const std::string& value)
    {
        _p->button->setText(value);
    }

    const std::shared_ptr<IWidget>& Bellows::getWidget() const
    {
        return _p->widget;
    }

    void Bellows::setWidget(const std::shared_ptr<IWidget>& value)
    {
        FTK_P();
        if (value == p.widget)
            return;
        releaseChild(p.layout, p.widget);
        p.widget = value;
        if (p.widget)
        {
            p.widget->setParent(_p->layout);
            p.widget->setVisible(p.button->isChecked());
        }
        setSizeUpdate();
        setDrawUpdate();
    }

    bool Bellows::isOpen() const
    {
        return _p->button->isChecked();
    }

    void Bellows::setOpen(bool value)
    {
        FTK_P();
        p.button->setChecked(value);
        if (p.widget)
        {
            p.widget->setVisible(value);
            if (p.openCallback)
            {
                p.openCallback(value);
            }
        }
    }

    void Bellows::setOpenCallback(const std::function<void(bool)>& value)
    {
        _p->openCallback = value;
    }

    const std::shared_ptr<IWidget>& Bellows::getCheckWidget() const
    {
        return _p->header->getCheckWidget();
    }

    void Bellows::setCheckWidget(const std::shared_ptr<IWidget>& value)
    {
        _p->header->setCheckWidget(value);
    }

    const std::shared_ptr<IWidget>& Bellows::getToolWidget() const
    {
        return _p->header->getToolWidget();
    }

    void Bellows::setToolWidget(const std::shared_ptr<IWidget>& value)
    {
        _p->header->setToolWidget(value);
    }
    
    

    
}
