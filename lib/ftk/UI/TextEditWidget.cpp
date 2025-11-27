// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/TextEditPrivate.h>

#include <ftk/UI/App.h>
#include <ftk/UI/ClipboardSystem.h>
#include <ftk/UI/DrawUtil.h>
#include <ftk/UI/IWindow.h>
#include <ftk/UI/LayoutUtil.h>
#include <ftk/UI/ScrollArea.h>

#include <ftk/Core/Format.h>
#include <ftk/Core/RenderUtil.h>

#include <optional>

namespace ftk
{
    struct TextEditWidget::Private
    {
        TextEditOptions options;
        std::shared_ptr<TextEditModel> model;
        std::function<void(const std::vector<std::string>&)> textCallback;
        std::function<void(bool)> focusCallback;
        TextEditPos cursorStart;
        bool cursorVisible = false;
        std::chrono::steady_clock::time_point cursorTimer;
        V2I autoScroll;
        std::chrono::steady_clock::time_point autoScrollTimer;
        TextEditSelection selection;
        std::shared_ptr<FontSystem> fontSystem;
        int mousePress = 0;

        struct SizeData
        {
            std::optional<float> displayScale;
            int margin = 0;
            int border = 0;
            FontInfo fontInfo;
            FontMetrics fontMetrics;
            std::optional<Size2I> textSize;
        };
        SizeData size;

        std::shared_ptr<ListObserver<std::string> > textObserver;
        std::shared_ptr<ValueObserver<TextEditPos> > cursorObserver;
        std::shared_ptr<ValueObserver<TextEditSelection> > selectionObserver;
    };

    void TextEditWidget::_init(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<TextEditModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        IMouseWidget::_init(context, "ftk::TextEditWidget", parent);
        FTK_P();

        setAcceptsKeyFocus(true);
        _setMouseHoverEnabled(true);

        p.model = model;
        
        p.fontSystem = context->getSystem<FontSystem>();

        p.textObserver = ListObserver<std::string>::create(
            p.model->observeText(),
            [this](const std::vector<std::string>& value)
            {
                FTK_P();
                if (p.textCallback)
                {
                    p.textCallback(value);
                }
                p.size.textSize.reset();
                setSizeUpdate();
                setDrawUpdate();
            });

        p.cursorObserver = ValueObserver<TextEditPos>::create(
            p.model->observeCursor(),
            [this](const TextEditPos&)
            {
                _cursorReset();
                setDrawUpdate();
            });

        p.selectionObserver = ValueObserver<TextEditSelection>::create(
            p.model->observeSelection(),
            [this](const TextEditSelection& value)
            {
                _p->selection = value;
                setDrawUpdate();
            });
    }

    TextEditWidget::TextEditWidget() :
        _p(new Private)
    {}

    TextEditWidget::~TextEditWidget()
    {}

    std::shared_ptr<TextEditWidget> TextEditWidget::create(
        const std::shared_ptr<Context>& context,
        const std::shared_ptr<TextEditModel>& model,
        const std::shared_ptr<IWidget>& parent)
    {
        auto out = std::shared_ptr<TextEditWidget>(new TextEditWidget);
        out->_init(context, model, parent);
        return out;
    }

    const std::vector<std::string>& TextEditWidget::getText() const
    {
        return _p->model->getText();
    }

    void TextEditWidget::setText(const std::vector<std::string>& value)
    {
        _p->model->setText(value);
    }

    void TextEditWidget::clearText()
    {
        _p->model->clearText();
    }

    void TextEditWidget::setTextCallback(const std::function<void(const std::vector<std::string>&)>& value)
    {
        _p->textCallback = value;
    }

    void TextEditWidget::setFocusCallback(const std::function<void(bool)>& value)
    {
        _p->focusCallback = value;
    }

    void TextEditWidget::selectAll()
    {
        _p->model->selectAll();
    }

    void TextEditWidget::clearSelection()
    {
        _p->model->clearSelection();
    }

    void TextEditWidget::setOptions(const TextEditOptions& value)
    {
        FTK_P();
        if (value == p.options)
            return;
        p.options = value;
        p.size.displayScale.reset();
        p.size.textSize.reset();
        setSizeUpdate();
        setDrawUpdate();
    }

    Box2I TextEditWidget::getCursorBox(bool margin) const
    {
        FTK_P();
        const auto& text = p.model->getText();
        const TextEditPos& cursor = p.model->getCursor();
        V2I pos(p.size.margin, p.size.margin);
        if (cursor.line >= 0 && cursor.line < text.size())
        {
            pos.y += p.size.fontMetrics.lineHeight * cursor.line;
            const std::string& line = text[cursor.line];
            pos.x += p.fontSystem->getSize(line.substr(0, cursor.chr), p.size.fontInfo).w;
        }

        Box2I out = Box2I(pos.x, pos.y, p.size.border, p.size.fontMetrics.lineHeight);
        if (margin)
        {
            out = ftk::margin(out, p.size.margin);
        }
        return out;
    }

    void TextEditWidget::setGeometry(const Box2I& value)
    {
        IMouseWidget::setGeometry(value);
        FTK_P();
        if (auto parent = getParentT<ScrollArea>())
        {
            const Size2I size = parent->getGeometry().size();
            p.model->setPageRows(size.h / p.size.fontMetrics.lineHeight);
        }
    }

    void TextEditWidget::setVisible(bool value)
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

    void TextEditWidget::setEnabled(bool value)
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

    void TextEditWidget::tickEvent(
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
            if (diff.count() > p.options.cursorBlink)
            {
                p.cursorVisible = !p.cursorVisible;
                setDrawUpdate();
                p.cursorTimer = now;
            }
        }
        else if (p.cursorVisible)
        {
            p.cursorVisible = false;
            setDrawUpdate();
        }

        if (p.autoScroll.x != 0 || p.autoScroll.y != 0)
        {
            const auto now = std::chrono::steady_clock::now();
            const std::chrono::duration<float> diff = now - p.autoScrollTimer;
            if (diff.count() > p.options.autoScrollTimeout)
            {
                auto cursor = p.model->getCursor();
                cursor.line += p.autoScroll.y;
                cursor.chr += p.autoScroll.x;
                p.model->setCursor(cursor);
                p.model->setSelection(TextEditSelection(p.cursorStart, cursor));
                p.autoScrollTimer = now;
            }
        }
    }

    void TextEditWidget::sizeHintEvent(const SizeHintEvent& event)
    {
        IMouseWidget::sizeHintEvent(event);
        FTK_P();

        if (!p.size.displayScale.has_value() ||
            (p.size.displayScale.has_value() && p.size.displayScale != event.displayScale))
        {
            p.size = Private::SizeData();
            p.size.displayScale = event.displayScale;
            p.size.margin = event.style->getSizeRole(SizeRole::MarginInside, event.displayScale);
            p.size.border = event.style->getSizeRole(SizeRole::Border, event.displayScale);
            p.size.fontInfo = p.options.fontInfo;
            p.size.fontInfo.size *= event.displayScale;
            p.size.fontMetrics = event.fontSystem->getMetrics(p.size.fontInfo);
        }
        
        if (!p.size.textSize.has_value())
        {
            p.size.textSize = Size2I();
            const auto& text = p.model->getText();
            for (size_t i = 0; i < text.size(); ++i)
            {
                p.size.textSize->w = std::max(
                    event.fontSystem->getSize(text[i], p.size.fontInfo).w,
                    p.size.textSize->w);
                p.size.textSize->h += p.size.fontMetrics.lineHeight;
            }
        }

        setSizeHint(margin(p.size.textSize.value(), p.size.margin));
    }

    void TextEditWidget::drawEvent(
        const Box2I& drawRect,
        const DrawEvent& event)
    {
        IMouseWidget::drawEvent(drawRect, event);
        FTK_P();

        const Box2I& g = getGeometry();

        // Draw the selection.
        const auto& text = p.model->getText();
        const Box2I g2(
            g.min.x + p.size.margin,
            g.min.y + p.size.margin,
            g.w() - p.size.margin * 2,
            g.h() - p.size.margin * 2);
        if (p.selection.isValid() &&
            p.selection.first.line >= 0 &&
            p.selection.first.line < static_cast<int>(text.size()) &&
            p.selection.second.line >= 0 &&
            p.selection.second.line < static_cast<int>(text.size()))
        {
            std::vector<Box2I> boxes;
            const TextEditPos min = p.selection.min();
            const TextEditPos max = p.selection.max();
            if (min.line == max.line)
            {
                const std::string& line = text[min.line];
                const int w0 = event.fontSystem->getSize(line.substr(0, min.chr), p.size.fontInfo).w;
                const int w1 = event.fontSystem->getSize(line.substr(0, max.chr), p.size.fontInfo).w;
                boxes.push_back(Box2I(
                    g2.min.x + w0,
                    g2.min.y + min.line * p.size.fontMetrics.lineHeight,
                    std::max(w1 - w0, p.size.border * 2),
                    p.size.fontMetrics.lineHeight));
            }
            else
            {
                int w0 = event.fontSystem->getSize(text[min.line].substr(0, min.chr), p.size.fontInfo).w;
                int w1 = event.fontSystem->getSize(text[min.line], p.size.fontInfo).w;
                boxes.push_back(Box2I(
                    g2.min.x + w0,
                    g2.min.y + min.line * p.size.fontMetrics.lineHeight,
                    std::max(w1 - w0, p.size.border * 2),
                    p.size.fontMetrics.lineHeight));
                for (int i = min.line + 1; i < max.line; ++i)
                {
                    w0 = event.fontSystem->getSize(text[i], p.size.fontInfo).w;
                    boxes.push_back(Box2I(
                        g2.min.x,
                        g2.min.y + i * p.size.fontMetrics.lineHeight,
                        std::max(w0, p.size.border * 2),
                        p.size.fontMetrics.lineHeight));
                }
                w0 = event.fontSystem->getSize(text[max.line].substr(0, max.chr), p.size.fontInfo).w;
                boxes.push_back(Box2I(
                    g2.min.x,
                    g2.min.y + max.line * p.size.fontMetrics.lineHeight,
                    std::max(w0, p.size.border * 2),
                    p.size.fontMetrics.lineHeight));
            }
            for (const auto& box : boxes)
            {
                if (intersects(box, drawRect))
                {
                    event.render->drawRect(box, event.style->getColorRole(ColorRole::Checked));
                }
            }
        }

        // Draw the text.
        V2I pos(g2.min);
        const bool enabled = isEnabled();
        const Color4F textColor = event.style->getColorRole(enabled ?
            ColorRole::Text :
            ColorRole::TextDisabled);
        for (const auto& line : text)
        {
            const Box2I g3(pos.x, pos.y, p.size.textSize->w, p.size.fontMetrics.lineHeight);
            if (intersects(g3, drawRect))
            {
                event.render->drawText(
                    event.fontSystem->getGlyphs(line, p.size.fontInfo),
                    p.size.fontMetrics,
                    pos,
                    textColor);
            }
            pos.y += p.size.fontMetrics.lineHeight;
        }

        // Draw the cursor.
        if (p.cursorVisible)
        {
            const Box2I cursor = getCursorBox();
            event.render->drawRect(
                Box2I(
                    g.min.x + cursor.min.x,
                    g.min.y + cursor.min.y,
                    cursor.w(),
                    cursor.h()),
                event.style->getColorRole(ColorRole::Text));
        }
    }

    void TextEditWidget::mouseMoveEvent(MouseMoveEvent& event)
    {
        IMouseWidget::mouseMoveEvent(event);
        FTK_P();
        if (1 == p.mousePress)
        {
            event.accept = true;
            V2I autoScroll;
            if (auto parent = getParentT<ScrollArea>())
            {
                const Box2I& g = parent->getGeometry();
                if (event.pos.x > g.max.x)
                {
                    autoScroll.x = 1;
                }
                else if (event.pos.x < g.min.x)
                {
                    autoScroll.x = -1;
                }
                if (event.pos.y > g.max.y)
                {
                    autoScroll.y = 1;
                }
                else if (event.pos.y < g.min.y)
                {
                    autoScroll.y = -1;
                }
            }
            if (autoScroll.x != 0 || autoScroll.y != 0)
            {
                if (autoScroll != p.autoScroll)
                {
                    p.autoScrollTimer = std::chrono::steady_clock::now();
                }
            }
            p.autoScroll = autoScroll;

            const TextEditPos cursor = _getCursorPos(event.pos);
            p.model->setCursor(cursor);
            p.model->setSelection(TextEditSelection(p.cursorStart, cursor));
        }
    }

    void TextEditWidget::mousePressEvent(MouseClickEvent& event)
    {
        IMouseWidget::mousePressEvent(event);
        FTK_P();
        if (1 == event.button)
        {
            event.accept = true;
            p.mousePress = event.button;
            p.cursorStart = _getCursorPos(event.pos);
            p.model->setCursor(p.cursorStart);
            takeKeyFocus();
        }
    }

    void TextEditWidget::mouseReleaseEvent(MouseClickEvent& event)
    {
        IMouseWidget::mouseReleaseEvent(event);
        FTK_P();
        event.accept = true;
        p.autoScroll = V2I();
        p.mousePress = 0;
    }

    void TextEditWidget::keyFocusEvent(bool value)
    {
        IMouseWidget::keyFocusEvent(value);
        FTK_P();
        if (p.focusCallback)
        {
            p.focusCallback(value);
        }
    }

    void TextEditWidget::keyPressEvent(KeyEvent& event)
    {
        FTK_P();
        if (hasKeyFocus())
        {
            switch (event.key)
            {
            case Key::Escape:
                event.accept = true;
                if (auto window = getWindow())
                {
                    window->setKeyFocus(window->getNextKeyFocus(shared_from_this()));
                }
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

    void TextEditWidget::keyReleaseEvent(KeyEvent& event)
    {
        IMouseWidget::keyReleaseEvent(event);
        event.accept = true;
    }

    void TextEditWidget::textEvent(TextEvent& event)
    {
        IMouseWidget::textEvent(event);
        event.accept = true;
        _p->model->input(event.text);
    }

    TextEditPos TextEditWidget::_getCursorPos(const V2I& value) const
    {
        FTK_P();
        TextEditPos out(0, 0);
        const Box2I& g = getGeometry();
        const auto& text = p.model->getText();
        out.line = clamp(
            (value.y - g.min.y - p.size.margin) / p.size.fontMetrics.lineHeight,
            0,
            static_cast<int>(text.size()) - 1);
        if (out.line >= 0 && out.line < text.size())
        {
            const auto boxes = p.fontSystem->getBoxes(text[out.line], p.size.fontInfo);
            int end = value.x - g.min.x - p.size.margin;
            for (;
                out.chr < boxes.size() && boxes[out.chr].max.x < end;
                ++out.chr)
                ;
        }
        return out;
    }

    void TextEditWidget::_cursorReset()
    {
        FTK_P();
        p.cursorVisible = true;
        p.cursorTimer = std::chrono::steady_clock::now();
    }
}