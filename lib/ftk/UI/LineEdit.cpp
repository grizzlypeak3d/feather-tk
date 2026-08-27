// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/LineEdit.h>

#include <ftk/UI/App.h>
#include <ftk/UI/ClipboardSystem.h>
#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/IWindow.h>
#include <ftk/UI/LayoutUtil.h>
#include <ftk/UI/LineEditModel.h>
#include <ftk/UI/Menu.h>

#include <ftk/Core/RenderUtil.h>
#include <ftk/Core/String.h>

#include <optional>

namespace ftk
{
    struct LineEdit::Private
    {
        std::shared_ptr<LineEditModel> model;
        std::shared_ptr<FontSystem> fontSystem;
        std::function<void(const std::string&)> callback;
        bool callbackOnFocusLost = true;
        bool selectAllOnFocus = false;
        std::function<void(const std::string&)> textChangedCallback;
        std::string format = std::string(20, '#');
        std::function<void(bool)> focusCallback;
        FontType font = FontType::Regular;
        ColorRole borderRole = ColorRole::Border;

        std::weak_ptr<Menu> contextMenu;
        bool contextMenuFocus = false;

        int cursorStart = -1;
        bool cursorVisible = false;
        std::string preedit;
        int preeditCursorBytes = 0;
        std::chrono::steady_clock::time_point cursorTimer;
        int scroll = 0;
        Box2I textBox;

        std::shared_ptr<Observer<std::string> > textObserver;
        std::shared_ptr<Observer<int> > cursorObserver;
        std::shared_ptr<Observer<LineEditSelection> > selectionObserver;

        struct SizeData
        {
            bool init = true;
            int margin = 0;
            int border = 0;
            int keyFocus = 0;
            int cornerRadius = 0;
            FontInfo fontInfo;
            FontMetrics fontMetrics;
            Size2I textSize;
            Size2I formatSize;
            std::vector<Box2I> glyphBoxes;
            Size2I sizeHint;
        };
        SizeData size;

        struct DrawData
        {
            Box2I g;
            Box2I g2;
            Box2I g3;
            TriMesh2F bgMesh;
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
        setContextMenuCallback([this] { return _createContextMenu(); });

        p.model = model;

        p.fontSystem = context->getSystem<FontSystem>();

        p.textObserver = Observer<std::string>::create(
            p.model->observeText(),
            [this](const std::string& text)
            {
                FTK_P();
                p.size.init = true;
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
                    _textInputAreaUpdate();
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
        p.size.init = true;
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

    bool LineEdit::hasSelectAllOnFocus() const
    {
        return _p->selectAllOnFocus;
    }

    void LineEdit::setSelectAllOnFocus(bool value)
    {
        _p->selectAllOnFocus = value;
    }

    void LineEdit::clearSelection()
    {
        _p->model->clearSelection();
    }

    FontType LineEdit::getFont() const
    {
        return _p->font;
    }

    void LineEdit::setFont(FontType value)
    {
        FTK_P();
        if (value == p.font)
            return;
        p.font = value;
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
            // Adjust scroll position if necessary.
            const Box2I textBox = _getTextGeometry();
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

    void LineEdit::styleEvent(const StyleEvent& event)
    {
        IMouseWidget::styleEvent(event);
        FTK_P();
        if (event.hasChanges())
        {
            p.size.init = true;
            p.draw.reset();
        }
    }

    void LineEdit::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        FTK_P();
        if (p.size.init)
        {
            p.size.init = false;
            p.size.margin = event.style->getSizeRole(SizeRole::MarginInside, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.keyFocus = event.style->getSizeRole(SizeRole::KeyFocus, event.displayScale);
            p.size.cornerRadius = event.style->getSizeRole(SizeRole::CornerRadius, event.displayScale);
            p.size.fontInfo = event.style->getFont(p.font, event.displayScale);
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
            p.draw->g = _getAlignGeometry();
            p.draw->g2 = _getMarginGeometry();
            p.draw->g3 = _getTextGeometry();
            p.draw->bgMesh = rect(p.draw->g, p.size.cornerRadius);
            p.draw->border = border(p.draw->g, p.size.border, p.size.cornerRadius);
            p.draw->keyFocus = border(p.draw->g, p.size.keyFocus, p.size.cornerRadius);
        }

        const bool enabled = isEnabled();

        // Draw the background.
        event.render->drawMesh(
            p.draw->bgMesh,
            event.style->getColorRole(ColorRole::Well));

        // Draw the focus and border.
        const bool keyFocus = hasKeyFocus();
        event.render->drawMesh(
            keyFocus ? p.draw->keyFocus : p.draw->border,
            event.style->getColorRole(keyFocus ? ColorRole::KeyFocus : ColorRole::Border));

        // Enable clipping.
        const ClipRectEnabledState clipRectEnabledState(event.render);
        const ClipRectState clipRectState(event.render);
        event.render->setClipRectEnabled(true);
        event.render->setClipRect(intersect(p.draw->g2, drawRect));

        // Splice in the input method composition: it is drawn as part
        // of the text with an underline, and the cursor inside it,
        // until it is committed or canceled.
        const std::string& modelText = p.model->getText();
        const int modelCursor = p.model->getCursor();
        std::string text = modelText;
        if (!p.preedit.empty())
        {
            text.insert(modelCursor, p.preedit);
        }

        // Draw the selection.
        const LineEditSelection& selection = p.model->getSelection();
        if (p.preedit.empty() && selection.isValid())
        {
            const std::string text0 = text.substr(0, selection.min());
            const int x0 = event.fontSystem->getSize(text0, p.size.fontInfo).w;
            const std::string text1 = text.substr(0, selection.max());
            const int x1 = event.fontSystem->getSize(text1, p.size.fontInfo).w;
            event.render->drawRect(
                Box2I(p.draw->g3.x() - p.scroll + x0,
                    p.draw->g3.y(),
                    x1 - x0 + 1,
                    p.draw->g3.h()),
                event.style->getColorRole(ColorRole::Checked, enabled));
        }

        // Draw the text.
        const V2I pos(
            p.draw->g3.x() - p.scroll,
            p.draw->g3.y() + p.draw->g3.h() / 2 - p.size.fontMetrics.lineHeight / 2);
        if (!text.empty() && p.draw->glyphs.empty())
        {
            p.draw->glyphs = event.fontSystem->getGlyphs(text, p.size.fontInfo);
        }
        event.render->drawText(
            p.draw->glyphs,
            p.size.fontMetrics,
            pos,
            event.style->getColorRole(ColorRole::Text, enabled));

        // Underline the composition.
        if (!p.preedit.empty())
        {
            const int x0 = event.fontSystem->getSize(
                text.substr(0, modelCursor), p.size.fontInfo).w;
            const int x1 = event.fontSystem->getSize(
                text.substr(0, modelCursor + p.preedit.size()),
                p.size.fontInfo).w;
            // Directly under the glyphs, kept inside the text geometry
            // in case a fallback font's line is taller than it.
            const int underlineY = std::min(
                pos.y + p.size.fontMetrics.lineHeight,
                p.draw->g3.y() + p.draw->g3.h() - p.size.border);
            event.render->drawRect(
                Box2I(
                    p.draw->g3.x() - p.scroll + x0,
                    underlineY,
                    x1 - x0,
                    p.size.border),
                event.style->getColorRole(ColorRole::Text, enabled));
        }

        // Draw the cursor.
        if (p.cursorVisible)
        {
            const int cursorPos = p.preedit.empty() ?
                _toPos(modelCursor) :
                event.fontSystem->getSize(
                    text.substr(0, modelCursor + p.preeditCursorBytes),
                    p.size.fontInfo).w;
            event.render->drawRect(
                Box2I(
                    p.draw->g3.x() - p.scroll + cursorPos,
                    p.draw->g3.y(),
                    p.size.border,
                    p.draw->g3.h()),
                event.style->getColorRole(ColorRole::Text));
        }
    }

    void LineEdit::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        FTK_P();
        if (_isMousePressed())
        {
            const int cursor = _toCursor(event.pos.x - _getTextGeometry().min.x + p.scroll);
            p.model->setCursor(cursor);
            p.model->setSelection(LineEditSelection(p.cursorStart, cursor));
        }
    }

    void LineEdit::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        if (MouseButton::Left == event.button)
        {
            p.cursorStart = _toCursor(event.pos.x - _getTextGeometry().min.x + p.scroll);
            p.model->setCursor(p.cursorStart);
            takeKeyFocus();
        }
    }

    void LineEdit::keyFocusEvent(bool value)
    {
        IMouseWidget::keyFocusEvent(value);
        FTK_P();
        // The context menu acts on the editing session, so focus moving
        // into it and back is not the user leaving the field: nothing is
        // committed, the selection stays, and clients hear nothing.
        if (!value)
        {
            const auto contextMenu = p.contextMenu.lock();
            if (contextMenu && contextMenu->isOpen())
            {
                p.contextMenuFocus = true;
                return;
            }
        }
        else if (p.contextMenuFocus)
        {
            p.contextMenuFocus = false;
            return;
        }
        p.contextMenuFocus = false;
        if (value)
        {
            if (p.selectAllOnFocus)
            {
                p.model->selectAll();
            }
            p.cursorVisible = true;
            p.cursorTimer = std::chrono::steady_clock::now();
            setDrawUpdate();
            _textInputAreaUpdate();
        }
        else
        {
            p.model->clearSelection();
            p.cursorVisible = false;
            if (!p.preedit.empty())
            {
                p.preedit.clear();
                p.preeditCursorBytes = 0;
                p.draw.reset();
            }
            setDrawUpdate();
            if (p.callback && p.callbackOnFocusLost)
            {
                p.callback(p.model->getText());
            }
        }
        if (auto window = getWindow())
        {
            window->setTextInput(value);
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
            // Committing replaces the composition.
            if (!p.preedit.empty())
            {
                p.preedit.clear();
                p.preeditCursorBytes = 0;
                p.draw.reset();
            }
            p.model->input(event.text);
        }
    }

    void LineEdit::textEditingEvent(TextEditingEvent& event)
    {
        FTK_P();
        if (!p.model->isReadOnly())
        {
            event.accept = true;
            p.preedit = event.text;
            // The cursor arrives in code points; the drawing measures
            // in bytes.
            size_t bytes = 0;
            for (int i = 0; i < event.cursor && bytes < p.preedit.size(); ++i)
            {
                bytes = utf8Next(p.preedit, bytes);
            }
            p.preeditCursorBytes = static_cast<int>(bytes);
            p.draw.reset();
            setDrawUpdate();
        }
    }

    const std::string& LineEdit::getPreedit() const
    {
        return _p->preedit;
    }

    std::shared_ptr<Menu> LineEdit::_createContextMenu()
    {
        FTK_P();
        auto context = getContext();
        if (!context)
            return nullptr;

        // The window opens the menu instead of delivering the press, so
        // place the cursor here. An existing selection is left alone so
        // that right clicking inside it can still act on it.
        const LineEditSelection selection = p.model->getSelection();
        const int cursor = _toCursor(
            _getMousePos().x - _getTextGeometry().min.x + p.scroll);
        if (!selection.isValid() ||
            cursor < selection.min() ||
            cursor > selection.max())
        {
            p.model->setCursor(cursor);
        }
        takeKeyFocus();

        // Hold the model rather than the widget so the actions stay valid
        // if the line edit is destroyed while the menu is open.
        auto model = p.model;
        const bool readOnly = model->isReadOnly();
        const bool hasSelection = model->getSelection().isValid();
        const bool hasClipboard =
            !context->getSystem<ClipboardSystem>()->getText().empty();

        auto out = Menu::create(context);

        auto undo = Action::create(
            "Undo",
            "Undo",
            KeyShortcut(Key::Z, commandKeyModifier),
            [model] { model->undo(); });
        out->addAction(undo);
        out->setEnabled(undo, !readOnly && model->observeHasUndo()->get());

        auto redo = Action::create(
            "Redo",
            "Redo",
            KeyShortcut(Key::Y, commandKeyModifier),
            [model] { model->redo(); });
        out->addAction(redo);
        out->setEnabled(redo, !readOnly && model->observeHasRedo()->get());

        out->addDivider();

        auto cut = Action::create(
            "Cut",
            "Cut",
            KeyShortcut(Key::X, commandKeyModifier),
            [model] { model->cut(); });
        out->addAction(cut);
        out->setEnabled(cut, !readOnly && hasSelection);

        auto copy = Action::create(
            "Copy",
            "Copy",
            KeyShortcut(Key::C, commandKeyModifier),
            [model] { model->copy(); });
        out->addAction(copy);
        out->setEnabled(copy, hasSelection);

        auto paste = Action::create(
            "Paste",
            "Paste",
            KeyShortcut(Key::V, commandKeyModifier),
            [model] { model->paste(); });
        out->addAction(paste);
        out->setEnabled(paste, !readOnly && hasClipboard);

        out->addDivider();

        auto selectAll = Action::create(
            "Select All",
            KeyShortcut(Key::A, commandKeyModifier),
            [model] { model->selectAll(); });
        out->addAction(selectAll);
        out->setEnabled(selectAll, !model->getText().empty());

        p.contextMenu = out;
        return out;
    }

    Box2I LineEdit::_getAlignGeometry() const
    {
        return getGeometry();
    }

    Box2I LineEdit::_getMarginGeometry() const
    {
        FTK_P();
        return margin(_getAlignGeometry(), -p.size.keyFocus);
    }

    Box2I LineEdit::_getTextGeometry() const
    {
        FTK_P();
        return margin(_getMarginGeometry(), -p.size.margin * 2, -p.size.margin);
    }

    int LineEdit::_toCursor(int value) const
    {
        FTK_P();
        // The glyph boxes are per code point and the model cursor is a
        // byte index, so the found glyph converts back to bytes; the
        // two only agree for ASCII.
        int glyph = 0;
        for (;
            glyph < static_cast<int>(p.size.glyphBoxes.size()) &&
                p.size.glyphBoxes[glyph].max.x < value;
            ++glyph)
            ;
        const std::string& text = p.model->getText();
        size_t out = 0;
        for (int i = 0; i < glyph && out < text.size(); ++i)
        {
            out = utf8Next(text, out);
        }
        return static_cast<int>(out);
    }

    int LineEdit::_toPos(int value) const
    {
        FTK_P();
        const int glyph = static_cast<int>(
            utf8Count(p.model->getText(), value));
        int out = 0;
        if (glyph >= 0 && glyph < static_cast<int>(p.size.glyphBoxes.size()))
        {
            out = p.size.glyphBoxes[glyph].min.x;
        }
        else if (glyph >= static_cast<int>(p.size.glyphBoxes.size()) &&
            !p.size.glyphBoxes.empty())
        {
            out = p.size.glyphBoxes.back().max.x;
        }
        return out;
    }

    void LineEdit::_textInputAreaUpdate()
    {
        FTK_P();
        if (hasKeyFocus())
        {
            if (auto window = getWindow())
            {
                const Box2I g = _getTextGeometry();
                window->setTextInputArea(Box2I(
                    g.min.x - p.scroll + _toPos(p.model->getCursor()),
                    g.min.y,
                    p.size.border,
                    g.h()));
            }
        }
    }

    void LineEdit::_scrollUpdate(int value)
    {
        FTK_P();
        const int pos = _toPos(value);
        const Box2I g = _getTextGeometry();
        if (pos > p.scroll + g.w())
        {
            p.scroll = pos - g.w();
            setDrawUpdate();
        }
        else if (pos < p.scroll)
        {
            p.scroll = pos;
            setDrawUpdate();
        }
    }
}
