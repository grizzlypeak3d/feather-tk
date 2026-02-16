// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/LineEdit.h>

#include <ftk/UI/App.h>
#include <ftk/UI/ClipboardSystem.h>
#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/LayoutUtil.h>
#include <ftk/UI/LineEditModel.h>

#include <ftk/Core/RenderUtil.h>

#include <optional>

namespace ftk
{
    struct LineEdit::Private
    {
        std::shared_ptr<LineEditModel> model;
        std::shared_ptr<FontSystem> fontSystem;
        std::function<void(const std::string&)> callback;
        bool callbackOnFocusLost = true;
        std::function<void(const std::string&)> textChangedCallback;
        std::string format = "                    ";
        std::function<void(bool)> focusCallback;
        FontRole fontRole = FontRole::Label;
        ColorRole borderRole = ColorRole::Border;

        int cursorStart = -1;
        bool cursorVisible = false;
        std::chrono::steady_clock::time_point cursorTimer;
        int scroll = 0;
        Box2I textBox;

        std::shared_ptr<Observer<std::string> > textObserver;
        std::shared_ptr<Observer<int> > cursorObserver;
        std::shared_ptr<Observer<LineEditSelection> > selectionObserver;

        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
            int border = 0;
            int keyFocus = 0;
            FontInfo fontInfo;
            FontMetrics fontMetrics;
            Size2I textSize;
            Size2I formatSize;
            std::vector<Box2I> glyphBoxes;
            Size2I sizeHint;
            Box2I g;
            Box2I g2;
            Box2I g3;
        };
        SizeData size;

        struct DrawData
        {
            TriMesh2F border;
            TriMesh2F keyFocus;
            std::vector<std::shared_ptr<Glyph> > glyphs;
        };
        std::optional<DrawData> draw;
    };

    void LineEdit::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<LineEditModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::LineEdit", parent);
        FTK_P();

        setHAlign(HAlign::Fill);
        setAcceptsKeyFocus(true);
        _setMouseHoverEnabled(true);
        _setMousePressEnabled(true);

        p.model = model;

        p.fontSystem = context->getSystem<FontSystem>();

        p.textObserver = Observer<std::string>::create(
            p.model->observeText(),
            [this](const std::string& text)
            {
                FTK_P();
                p.size.displayScale.reset();
                p.size.glyphBoxes = p.fontSystem->getBoxes(text, p.size.fontInfo);
                setSizeUpdate();
                setDrawUpdate();
                if (p.textChangedCallback)
                {
                    p.textChangedCallback(text);
                }
            });

        p.cursorObserver = Observer<int>::create(
            p.model->observeCursor(),
            [this](int index)
            {
                FTK_P();
                if (hasKeyFocus())
                {
                    p.cursorVisible = true;
                    p.cursorTimer = std::chrono::steady_clock::now();
                    setDrawUpdate();
                }
                _scrollUpdate(index);
            });

        p.selectionObserver = Observer<LineEditSelection>::create(
            p.model->observeSelection(),
            [this](const LineEditSelection&)
            {
                setDrawUpdate();
            });
    }

    LineEdit::LineEdit() :
        _p(new Private)
    {}

    LineEdit::~LineEdit()
    {}

    std::shared_ptr<LineEdit> LineEdit::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<LineEdit>(new LineEdit);
        out->_init(context, LineEditModel::create(context), parent);
        return out;
    }

    std::shared_ptr<LineEdit> LineEdit::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<LineEditModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<LineEdit>(new LineEdit);
        out->_init(context, model, parent);
        return out;
    }

    const std::shared_ptr<LineEditModel>& LineEdit::getModel() const
    {
        return _p->model;
    }

    const std::string& LineEdit::getText() const
    {
        return _p->model->getText();
    }

    void LineEdit::setText(const std::string& value)
    {
        _p->model->setText(value);
    }

    void LineEdit::clearText()
    {
        _p->model->clearText();
    }

    void LineEdit::setCallback(const std::function<void(const std::string&)>& value)
    {
        _p->callback = value;
    }

    bool LineEdit::hasCallbackOnFocusLost() const
    {
        return _p->callbackOnFocusLost;
    }

    void LineEdit::setCallbackOnFocusLost(bool value)
    {
        _p->callbackOnFocusLost = value;
    }

    void LineEdit::setTextChangedCallback(const std::function<void(const std::string&)>& value)
    {
        _p->textChangedCallback = value;
    }

    const std::string& LineEdit::getFormat() const
    {
        return _p->format;
    }

    void LineEdit::setFormat(const std::string& value)
    {
        FTK_P();
        if (value == p.format)
            return;
        p.format = value;
        p.size.displayScale.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    void LineEdit::setFocusCallback(const std::function<void(bool)>& value)
    {
        _p->focusCallback = value;
    }

    bool LineEdit::isReadOnly() const
    {
        return _p->model->isReadOnly();
    }

    void LineEdit::setReadOnly(bool value)
    {
        _p->model->setReadOnly(value);
    }

    void LineEdit::selectAll()
    {
        _p->model->selectAll();
    }

    void LineEdit::clearSelection()
    {
        _p->model->clearSelection();
    }

    FontRole LineEdit::getFontRole() const
    {
        return _p->fontRole;
    }

    void LineEdit::setFontRole(FontRole value)
    {
        FTK_P();
        if (value == p.fontRole)
            return;
        p.fontRole = value;
        setSizeUpdate();
        setDrawUpdate();
    }

    ColorRole LineEdit::getBorderRole() const
    {
        return _p->borderRole;
    }

    void LineEdit::setBorderRole(ColorRole value)
    {
        FTK_P();
        if (value == p.borderRole)
            return;
        p.borderRole = value;
        setDrawUpdate();
    }

    Size2I LineEdit::getSizeHint() const
    {
        return _p->size.sizeHint;
    }

    void LineEdit::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IMouseWidget::setGeometry(value);
        FTK_P();
        if (changed)
        {
            p.size.g = align(getGeometry(), getSizeHint(), getHAlign(), getVAlign());
            p.size.g2 = margin(p.size.g, -p.size.keyFocus);
            p.size.g3 = margin(p.size.g2, -p.size.margin * 2, -p.size.margin);

            // Adjust scroll position if necessary.
            const Box2I textBox = p.size.g3;
            const int diff = textBox.w() - p.textBox.w();
            if (diff > 0)
            {
                p.scroll = std::max(0, p.scroll - diff);
            }
            p.textBox = textBox;

            p.draw.reset();
        }
    }

    void LineEdit::setVisible(bool value)
    {
        FTK_P();
        const bool changed = value != isVisible(false);
        IMouseWidget::setVisible(value);
        if (changed && !isVisible(false))
        {
            if (p.cursorVisible)
            {
                p.cursorVisible = false;
                setDrawUpdate();
            }
        }
    }

    void LineEdit::setEnabled(bool value)
    {
        FTK_P();
        const bool changed = value != isEnabled(false);
        IMouseWidget::setEnabled(value);
        if (changed && !isEnabled(false))
        {
            if (p.cursorVisible)
            {
                p.cursorVisible = false;
                setDrawUpdate();
            }
        }
    }

    void LineEdit::tickEvent(
        bool parentsVisible,
        bool parentsEnabled,
        const TickEvent& event)
    {
        IMouseWidget::tickEvent(parentsVisible, parentsEnabled, event);
        FTK_P();
        if (hasKeyFocus())
        {
            const auto now = std::chrono::steady_clock::now();
            const std::chrono::duration<float> diff = now - p.cursorTimer;
            if (diff.count() > .7F)
            {
                p.cursorVisible = !p.cursorVisible;
                setDrawUpdate();
                p.cursorTimer = now;
            }
        }
    }

    void LineEdit::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        FTK_P();
        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale.value() != event.displayScale))
        {
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(SizeRole::MarginInside, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.fontInfo = event.style->getFontRole(p.fontRole, event.displayScale);
            p.size.fontMetrics = event.fontSystem->getMetrics(p.size.fontInfo);
            const auto& text = p.model->getText();
            p.size.textSize = event.fontSystem->getSize(text, p.size.fontInfo);
            p.size.formatSize = event.fontSystem->getSize(p.format, p.size.fontInfo);
            p.size.glyphBoxes = p.fontSystem->getBoxes(text, p.size.fontInfo);

            p.size.sizeHint = Size2I(p.size.formatSize.w, p.size.fontMetrics.lineHeight);
            p.size.sizeHint = margin(
                p.size.sizeHint,
                p.size.margin * 2 + p.size.keyFocus,
                p.size.margin + p.size.keyFocus);

            p.draw.reset();
        }
    }

    void LineEdit::clipEvent(const Box2I& clipRect, bool clipped)
    {
        IMouseWidget::clipEvent(clipRect, clipped);
        FTK_P();
        if (clipped)
        {
            p.draw.reset();
        }
    }

    void LineEdit::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IMouseWidget::drawEvent(drawRect, event);
        FTK_P();

        if (!p.draw.has_value())
        {
            p.draw = Private::DrawData();
            p.draw->border = border(p.size.g, p.size.border);
            p.draw->keyFocus = border(p.size.g, p.size.keyFocus);
        }

        const bool enabled = isEnabled();

        // Draw the background.
        event.render->drawRect(
            p.size.g,
            event.style->getColorRole(ColorRole::Base));

        // Draw the focus and border.
        const bool keyFocus = hasKeyFocus();
        event.render->drawMesh(
            keyFocus ? p.draw->keyFocus : p.draw->border,
            event.style->getColorRole(keyFocus ? ColorRole::KeyFocus : ColorRole::Border));

        // Enable clipping.
        const ClipRectEnabledState clipRectEnabledState(event.render);
        const ClipRectState clipRectState(event.render);
        event.render->setClipRectEnabled(true);
        event.render->setClipRect(intersect(p.size.g2, drawRect));

        // Draw the selection.
        const std::string& text = p.model->getText();
        const LineEditSelection& selection = p.model->getSelection();
        if (selection.isValid())
        {
            const std::string text0 = text.substr(0, selection.min());
            const int x0 = event.fontSystem->getSize(text0, p.size.fontInfo).w;
            const std::string text1 = text.substr(0, selection.max());
            const int x1 = event.fontSystem->getSize(text1, p.size.fontInfo).w;
            event.render->drawRect(
                Box2I(p.size.g3.x() - p.scroll + x0,
                    p.size.g3.y(),
                    x1 - x0 + 1,
                    p.size.g3.h()),
                event.style->getColorRole(ColorRole::Checked));
        }

        // Draw the text.
        const V2I pos(
            p.size.g3.x() - p.scroll,
            p.size.g3.y() + p.size.g3.h() / 2 - p.size.fontMetrics.lineHeight / 2);
        if (!text.empty() && p.draw->glyphs.empty())
        {
            p.draw->glyphs = event.fontSystem->getGlyphs(text, p.size.fontInfo);
        }
        event.render->drawText(
            p.draw->glyphs,
            p.size.fontMetrics,
            pos,
            event.style->getColorRole(enabled ?
                ColorRole::Text :
                ColorRole::TextDisabled));

        // Draw the cursor.
        if (p.cursorVisible)
        {
            event.render->drawRect(
                Box2I(
                    p.size.g3.x() - p.scroll + _toPos(p.model->getCursor()),
                    p.size.g3.y(),
                    p.size.border,
                    p.size.g3.h()),
                event.style->getColorRole(ColorRole::Text));
        }
    }

    void LineEdit::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        FTK_P();
        if (_isMousePressed())
        {
            const int cursor = _toCursor(event.pos.x - p.size.g3.min.x + p.scroll);
            p.model->setCursor(cursor);
            p.model->setSelection(LineEditSelection(p.cursorStart, cursor));
        }
    }

    void LineEdit::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        p.cursorStart = _toCursor(event.pos.x - p.size.g3.min.x + p.scroll);
        p.model->setCursor(p.cursorStart);
        takeKeyFocus();
    }

    void LineEdit::keyFocusEvent(bool value)
    {
        IMouseWidget::keyFocusEvent(value);
        FTK_P();
        if (value)
        {
            p.cursorVisible = true;
            p.cursorTimer = std::chrono::steady_clock::now();
            setDrawUpdate();
        }
        else
        {
            p.model->clearSelection();
            p.cursorVisible = false;
            setDrawUpdate();
            if (p.callback && p.callbackOnFocusLost)
            {
                p.callback(p.model->getText());
            }
        }
        if (p.focusCallback)
        {
            p.focusCallback(value);
        }
    }

    void LineEdit::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        if (hasKeyFocus())
        {
            switch (event.key)
            {
            case Key::Return:
                if (!p.model->isReadOnly())
                {
                    event.accept = true;
                    if (p.callback)
                    {
                        p.callback(p.model->getText());
                    }
                }
                break;
            case Key::Escape:
                event.accept = true;
                releaseKeyFocus();
                break;
            default:
                event.accept = p.model->key(event.key, event.modifiers);
                break;
            }
        }
        if (!event.accept)
        {
            IMouseWidget::keyPressEvent(event);
        }
    }

    void LineEdit::keyReleaseEvent(KeyEvent& event)
    {
        IMouseWidget::keyReleaseEvent(event);
        event.accept = true;
    }

    void LineEdit::textEvent(TextEvent& event)
    {
        FTK_P();
        if (!p.model->isReadOnly())
        {
            event.accept = true;
            p.model->input(event.text);
        }
    }

    int LineEdit::_toCursor(int value) const
    {
        FTK_P();
        int out = 0;
        for (;
            out < p.size.glyphBoxes.size() && p.size.glyphBoxes[out].max.x < value;
            ++out)
            ;
        return out;
    }

    int LineEdit::_toPos(int value) const
    {
        FTK_P();
        int out = 0;
        if (value >= 0 && value < p.size.glyphBoxes.size())
        {
            out = p.size.glyphBoxes[value].min.x;
        }
        else if (value >= p.size.glyphBoxes.size() && !p.size.glyphBoxes.empty())
        {
            out = p.size.glyphBoxes.back().max.x;
        }
        return out;
    }

    void LineEdit::_scrollUpdate(int value)
    {
        FTK_P();
        const int pos = _toPos(value);
        if (pos > p.scroll + p.size.g3.w())
        {
            p.scroll = pos - p.size.g3.w();
            setDrawUpdate();
        }
        else if (pos < p.scroll)
        {
            p.scroll = pos;
            setDrawUpdate();
        }
    }
}
