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
        std::function<void(const std::string&)> textCallback;
        bool textCallbackOnFocusLost = true;
        std::function<void(const std::string&)> textChangedCallback;
        std::string format = "                    ";
        std::function<void(bool)> focusCallback;
        FontRole fontRole = FontRole::Label;
        ColorRole borderRole = ColorRole::Border;
        std::vector<Box2I> glyphBoxes;
        int cursorStart = -1;
        bool cursorVisible = false;
        std::chrono::steady_clock::time_point cursorTimer;
        int scroll = 0;

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
        };
        SizeData size;

        struct DrawData
        {
            Box2I g2;
            Box2I g3;
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
        if (!p.model)
        {
            p.model = LineEditModel::create(context);
        }

        p.fontSystem = context->getSystem<FontSystem>();

        p.textObserver = Observer<std::string>::create(
            p.model->observeText(),
            [this](const std::string& text)
            {
                FTK_P();
                p.glyphBoxes = p.fontSystem->getBoxes(text, p.size.fontInfo);
                p.size.displayScale.reset();
                setSizeUpdate();
                setDrawUpdate();
            });

        p.cursorObserver = Observer<int>::create(
            p.model->observeCursor(),
            [this](int index)
            {
                if (hasKeyFocus())
                {
                    _p->cursorVisible = true;
                    _p->cursorTimer = std::chrono::steady_clock::now();
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
        out->_init(context, nullptr, parent);
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

    void LineEdit::setTextCallback(const std::function<void(const std::string&)>& value)
    {
        _p->textCallback = value;
    }

    bool LineEdit::hasTextCallbackOnFocusLost() const
    {
        return _p->textCallbackOnFocusLost;
    }

    void LineEdit::setTextCallbackOnFocusLost(bool value)
    {
        _p->textCallbackOnFocusLost = value;
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

    void LineEdit::setGeometry(const Box2I& value)
    {
        const bool changed = value != getGeometry();
        IMouseWidget::setGeometry(value);
        FTK_P();
        if (changed)
        {
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
            p.size.textSize = event.fontSystem->getSize(p.model->getText(), p.size.fontInfo);
            p.size.formatSize = event.fontSystem->getSize(p.format, p.size.fontInfo);
            p.draw.reset();
        }

        Size2I sizeHint(p.size.formatSize.w, p.size.fontMetrics.lineHeight);
        sizeHint = margin(
            sizeHint,
            p.size.margin * 2 + p.size.keyFocus,
            p.size.margin + p.size.keyFocus);
        setSizeHint(sizeHint);
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
            const Box2I g = align(getGeometry(), getSizeHint(), getHAlign(), getVAlign());
            p.draw->g2 = margin(g, -p.size.keyFocus);
            p.draw->g3 = margin(
                p.draw->g2,
                -p.size.margin * 2,
                -p.size.margin);
            p.draw->border = border(g, p.size.border);
            p.draw->keyFocus = border(g, p.size.keyFocus);
        }

        const bool enabled = isEnabled();

        // Draw the focus and border.
        const bool keyFocus = hasKeyFocus();
        event.render->drawMesh(
            keyFocus ? p.draw->keyFocus : p.draw->border,
            event.style->getColorRole(keyFocus ? ColorRole::KeyFocus : ColorRole::Border));


        // Draw the background.
        event.render->drawRect(
            p.draw->g2,
            event.style->getColorRole(ColorRole::Base));

        // Enable clipping.
        const ClipRectEnabledState clipRectEnabledState(event.render);
        const ClipRectState clipRectState(event.render);
        event.render->setClipRectEnabled(true);
        event.render->setClipRect(intersect(p.draw->g2, drawRect));

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
                Box2I(p.draw->g3.x() - p.scroll + x0,
                    p.draw->g3.y(),
                    x1 - x0 + 1,
                    p.draw->g3.h()),
                event.style->getColorRole(ColorRole::Checked));
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
            event.style->getColorRole(enabled ?
                ColorRole::Text :
                ColorRole::TextDisabled));

        // Draw the cursor.
        if (p.cursorVisible)
        {
            event.render->drawRect(
                Box2I(
                    p.draw->g3.x() - p.scroll + _toPos(p.model->getCursor()),
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
        if (_isMousePressed() && p.draw.has_value())
        {
            const int cursor = _toCursor(event.pos.x - p.draw->g3.min.x + p.scroll);
            p.model->setCursor(cursor);
            p.model->setSelection(LineEditSelection(p.cursorStart, cursor));
        }
    }

    void LineEdit::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        if (p.draw.has_value())
        {
            p.cursorStart = _toCursor(event.pos.x - p.draw->g3.min.x + p.scroll);
            p.model->setCursor(p.cursorStart);
        }
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
            if (p.textCallback && p.textCallbackOnFocusLost)
            {
                p.textCallback(p.model->getText());
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
            event.accept = p.model->key(event.key, event.modifiers);
            if (!event.accept)
            {
                switch (event.key)
                {
                case Key::Return:
                    event.accept = true;
                    if (p.textCallback)
                    {
                        p.textCallback(p.model->getText());
                    }
                    break;
                case Key::Escape:
                    event.accept = true;
                    releaseKeyFocus();
                    break;
                default: break;
                }
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
        event.accept = true;
        p.model->input(event.text);
    }

    int LineEdit::_toCursor(int value) const
    {
        FTK_P();
        int out = 0;
        for (;
            out < p.glyphBoxes.size() && p.glyphBoxes[out].max.x < value;
            ++out)
            ;
        return out;
    }

    int LineEdit::_toPos(int value) const
    {
        FTK_P();
        int out = 0;
        if (value >= 0 && value < p.glyphBoxes.size())
        {
            out = p.glyphBoxes[value].min.x;
        }
        else if (value >= p.glyphBoxes.size() && !p.glyphBoxes.empty())
        {
            out = p.glyphBoxes.back().max.x;
        }
        return out;
    }

    void LineEdit::_scrollUpdate(int value)
    {
        FTK_P();
        if (p.draw.has_value())
        {
            const int pos = _toPos(value);
            if (pos > p.scroll + p.draw->g3.w())
            {
                p.scroll = pos - p.draw->g3.w();
                setDrawUpdate();
            }
            else if (pos < p.scroll)
            {
                p.scroll = pos;
                setDrawUpdate();
            }
        }
    }
}
