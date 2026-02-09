// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/TextEditPrivate.h>

#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/ScrollWidget.h>

#include <optional>

namespace ftk
{
    bool TextEditOptions::operator == (const TextEditOptions& other) const
    {
        return
            fontInfo == other.fontInfo &&
            cursorBlink == other.cursorBlink &&
            autoScrollTimeout == other.autoScrollTimeout;
    }

    bool TextEditOptions::operator != (const TextEditOptions& other) const
    {
        return !(*this == other);
    }

    struct TextEdit::Private
    {
        std::shared_ptr<Observable<TextEditOptions> > options;
        std::shared_ptr<TextEditModel> model;
        std::shared_ptr<TextEditWidget> widget;
        std::shared_ptr<ScrollWidget> scrollWidget;
        SizeRole marginRole = SizeRole::None;

        std::shared_ptr<Observer<TextEditPos> > cursorObserver;

        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
            int border = 0;
            int keyFocus = 0;
        };
        SizeData size;

        struct DrawData
        {
            TriMesh2F border;
            TriMesh2F keyFocus;
        };
        std::optional<DrawData> draw;
    };

    void TextEdit::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<TextEditModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IWidget::_init(context, "ftk::TextEdit", parent);
        FTK_P();

        setAcceptsKeyFocus(true);

        p.options = Observable<TextEditOptions>::create();

        p.model = model;

        p.widget = TextEditWidget::create(context, p.model);
        p.widget->setStretch(Stretch::Expanding);

        p.scrollWidget = ScrollWidget::create(context, ScrollType::Both, shared_from_this());
        p.scrollWidget->setBorder(false);
        p.scrollWidget->setWidget(p.widget);

        p.widget->setFocusCallback(
            [this](bool value)
            {
                setDrawUpdate();
            });

        p.cursorObserver = Observer<TextEditPos>::create(
            p.model->observeCursor(),
            [this](const TextEditPos&)
            {
                _p->scrollWidget->scrollTo(_p->widget->getCursorBox(true));
            });
    }

    TextEdit::TextEdit() :
        _p(new Private)
    {}

    TextEdit::~TextEdit()
    {}

    std::shared_ptr<TextEdit> TextEdit::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<TextEdit>(new TextEdit);
        out->_init(context, TextEditModel::create(context), parent);
        return out;
    }

    std::shared_ptr<TextEdit> TextEdit::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<TextEditModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<TextEdit>(new TextEdit);
        out->_init(context, model, parent);
        return out;
    }

    const std::shared_ptr<TextEditModel>& TextEdit::getModel() const
    {
        return _p->model;
    }

    const std::vector<std::string>& TextEdit::getText() const
    {
        return _p->model->getText();
    }

    void TextEdit::setText(const std::vector<std::string>& value)
    {
        _p->model->setText(value);
    }

    void TextEdit::clearText()
    {
        _p->model->clearText();
    }

    void TextEdit::setCallback(const std::function<void(const std::vector<std::string>&)>& value)
    {
        _p->widget->setCallback(value);
    }

    bool TextEdit::isReadOnly() const
    {
        return _p->widget->isReadOnly();
    }

    void TextEdit::setReadOnly(bool value)
    {
        _p->widget->setReadOnly(value);
    }

    void TextEdit::selectAll()
    {
        _p->model->selectAll();
    }

    void TextEdit::clearSelection()
    {
        _p->model->clearSelection();
    }

    const TextEditOptions& TextEdit::getOptions() const
    {
        return _p->options->get();
    }

    std::shared_ptr<IObservable<TextEditOptions> > TextEdit::observeOptions() const
    {
        return _p->options;
    }

    void TextEdit::setOptions(const TextEditOptions& value)
    {
        FTK_P();
        if (p.options->setIfChanged(value))
        {
            p.widget->setOptions(value);
        }
    }

    SizeRole TextEdit::getMarginRole() const
    {
        return _p->marginRole;
    }

    void TextEdit::setMarginRole(SizeRole value)
    {
        FTK_P();
        if (value == p.marginRole)
            return;
        p.marginRole = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    void TextEdit::takeKeyFocus()
    {
        _p->widget->takeKeyFocus();
    }
    
    Size2I TextEdit::getSizeHint() const
    {
        FTK_P();
        return margin(
            _p->scrollWidget->getSizeHint(),
            p.size.margin + std::max(p.size.border, p.size.keyFocus));
    }

    void TextEdit::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IWidget::setGeometry(value);
        FTK_P();
        p.scrollWidget->setGeometry(margin(
            value,
            -(p.size.margin + std::max(p.size.border, p.size.keyFocus))));
        if (changed)
        {
            p.draw.reset();
        }
    }

    void TextEdit::sizeHintEvent(const SizeHintEvent& event)
    {
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(p.marginRole, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.draw.reset();
        }
    }

    void TextEdit::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            const Box2I& g = getGeometry();
            p.draw->border = border(
                margin(g, -(p.size.margin + std::max(p.size.border, p.size.keyFocus)) + p.size.border),
                p.size.border);
            p.draw->keyFocus = border(
                margin(g, -(p.size.margin + std::max(p.size.border, p.size.keyFocus)) + p.size.keyFocus),
                p.size.keyFocus);
        }

        // Draw the focus and border.
        const bool keyFocus = p.widget->hasKeyFocus();
        event.render->drawMesh(
            keyFocus ? p.draw->keyFocus : p.draw->border,
            event.style->getColorRole(keyFocus ? ColorRole::KeyFocus : ColorRole::Border));
    }

    void to_json(nlohmann::json& json, const TextEditOptions& value)
    {
        json["FontInfo"] = value.fontInfo;
        json["CursorBlink"] = value.cursorBlink;
        json["AutoScrollTimeout"] = value.autoScrollTimeout;
    }

    void from_json(const nlohmann::json& json, TextEditOptions& value)
    {
        json.at("FontInfo").get_to(value.fontInfo);
        json.at("CursorBlink").get_to(value.cursorBlink);
        json.at("AutoScrollTimeout").get_to(value.autoScrollTimeout);
    }
}
