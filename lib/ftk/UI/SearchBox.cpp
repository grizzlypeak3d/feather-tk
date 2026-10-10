// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/SearchBox.h>

#include <ftk/UI/LineEdit.h>
#include <ftk/UI/ToolButton.h>

namespace ftk
{
    struct SearchBox::Private
    {
        std::shared_ptr<LineEdit> lineEdit;
        std::shared_ptr<ToolButton> button;

        std::function<void(const std::string&)> callback;
        std::function<void(void)> returnCallback;

        int keyFocus = 0;
    };

    void SearchBox::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        IContainer::_init(context, "ftk::SearchBox", parent);
        FTK_P();

        // The button is inside the field, over its right end, and the
        // field keeps its text clear of it.
        p.lineEdit = LineEdit::create(context);
        _setWidget(p.lineEdit);

        p.button = ToolButton::create(context, shared_from_this());

        _widgetUpdate();

        p.lineEdit->setTextChangedCallback(
            [this](const std::string& value)
            {
                _widgetUpdate();
                if (_p->callback)
                {
                    _p->callback(value);
                }
            });

        // The return key only: leaving the field is not asking for
        // anything.
        p.lineEdit->setCallbackOnFocusLost(false);
        p.lineEdit->setCallback(
            [this](const std::string&)
            {
                if (_p->returnCallback)
                {
                    _p->returnCallback();
                }
            });

        p.button->setClickedCallback(
            [this]
            {
                _p->lineEdit->clearText();
                _widgetUpdate();
                if (_p->callback)
                {
                    _p->callback(std::string());
                }
            });
    }

    SearchBox::SearchBox() :
        _p(new Private)
    {}

    SearchBox::~SearchBox()
    {}

    std::shared_ptr<SearchBox> SearchBox::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<SearchBox>(new SearchBox);
        out->_init(context, parent);
        return out;
    }

    const std::string& SearchBox::getText() const
    {
        return _p->lineEdit->getText();
    }

    void SearchBox::setText(const std::string& value)
    {
        _p->lineEdit->setText(value);
        _widgetUpdate();
    }

    void SearchBox::setCallback(const std::function<void(const std::string&)>& value)
    {
        _p->callback = value;
    }

    void SearchBox::setReturnCallback(const std::function<void(void)>& value)
    {
        _p->returnCallback = value;
    }

    void SearchBox::takeKeyFocus()
    {
        _p->lineEdit->takeKeyFocus();
    }

    Size2I SearchBox::getSizeHint() const
    {
        FTK_P();
        Size2I out = p.lineEdit->getSizeHint();
        const Size2I button = p.button->getSizeHint();
        out.w += button.w;
        out.h = std::max(out.h, button.h + p.keyFocus * 2);
        return out;
    }

    void SearchBox::setGeometry(const Box2I& value)
    {
        IContainer::setGeometry(value);
        FTK_P();
        const Box2I g = margin(value, -p.keyFocus);
        const int w = std::min(p.button->getSizeHint().w, g.w());
        p.button->setGeometry(Box2I(g.max.x + 1 - w, g.min.y, w, g.h()));
        p.lineEdit->setRightInset(w);
    }

    void SearchBox::sizeHintEvent(const SizeHintEvent& event)
    {
        IContainer::sizeHintEvent(event);
        _p->keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
    }

    void SearchBox::_widgetUpdate()
    {
        FTK_P();
        const std::string& text = p.lineEdit->getText();
        p.button->setIcon(!text.empty() ? "Clear" : "Search");
        p.button->setEnabled(!text.empty());
    }
}