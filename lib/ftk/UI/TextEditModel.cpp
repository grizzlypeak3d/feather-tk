// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/TextEditModel.h>

#include <ftk/UI/ClipboardSystem.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Command.h>
#include <ftk/Core/String.h>

namespace ftk
{
    namespace
    {
        const std::vector<std::string> textEditClear({ "" });
    }

    TextEditPos::TextEditPos(int line, int chr) :
        line(line),
        chr(chr)
    {}

    bool TextEditPos::isValid() const
    {
        return line != -1 && chr != -1;
    }

    bool TextEditPos::operator == (const TextEditPos& other) const
    {
        return line == other.line && chr == other.chr;
    }

    bool TextEditPos::operator != (const TextEditPos& other) const
    {
        return !(*this == other);
    }

    bool TextEditPos::operator < (const TextEditPos& other) const
    {
        return line < other.line ||
            (line == other.line && chr < other.chr);
    }

    bool TextEditPos::operator > (const TextEditPos& other) const
    {
        return line > other.line ||
            (line == other.line && chr > other.chr);
    }

    TextEditSelection::TextEditSelection(const TextEditPos& first) :
        first(first)
    {}

    TextEditSelection::TextEditSelection(const TextEditPos& first, const TextEditPos& second) :
        first(first),
        second(second)
    {}

    bool TextEditSelection::isValid() const
    {
        return
            first.isValid() &&
            second.isValid() &&
            first != second;
    }

    TextEditPos TextEditSelection::min() const
    {
        return std::min(first, second);
    }

    TextEditPos TextEditSelection::max() const
    {
        return std::max(first, second);
    }

    bool TextEditSelection::operator == (const TextEditSelection& other) const
    {
        return
            first == other.first &&
            second == other.second;
    }

    bool TextEditSelection::operator != (const TextEditSelection& other) const
    {
        return !(*this == other);
    }

    bool TextEditModelOptions::operator != (const TextEditModelOptions& other) const
    {
        return !(*this == other);
    }

    bool TextEditModelOptions::operator == (const TextEditModelOptions& other) const
    {
        return tabSpaces == other.tabSpaces;
    }

    namespace
    {
        //! Undo/redo command for a line-range replacement. The affected lines
        //! [lineStart, lineStart + oldLines.size()) are replaced by newLines on
        //! exec, and the inverse on undo, restoring the cursor and selection.
        class TextEditCommand : public ICommand
        {
        public:
            TextEditCommand(
                const std::shared_ptr<ObservableList<std::string> >& text,
                const std::shared_ptr<Observable<TextEditPos> >& cursor,
                const std::shared_ptr<Observable<TextEditSelection> >& selection,
                int lineStart,
                const std::vector<std::string>& oldLines,
                const std::vector<std::string>& newLines,
                const TextEditPos& cursorBefore,
                const TextEditSelection& selectionBefore,
                const TextEditPos& cursorAfter,
                const TextEditSelection& selectionAfter) :
                _text(text),
                _cursor(cursor),
                _selection(selection),
                _lineStart(lineStart),
                _oldLines(oldLines),
                _newLines(newLines),
                _cursorBefore(cursorBefore),
                _selectionBefore(selectionBefore),
                _cursorAfter(cursorAfter),
                _selectionAfter(selectionAfter)
            {}

            void exec() override
            {
                _text->replaceItems(
                    _lineStart,
                    _lineStart + _oldLines.size(),
                    _newLines);
                _cursor->setIfChanged(_cursorAfter);
                _selection->setIfChanged(_selectionAfter);
            }

            void undo() override
            {
                _text->replaceItems(
                    _lineStart,
                    _lineStart + _newLines.size(),
                    _oldLines);
                _cursor->setIfChanged(_cursorBefore);
                _selection->setIfChanged(_selectionBefore);
            }

        private:
            std::shared_ptr<ObservableList<std::string> > _text;
            std::shared_ptr<Observable<TextEditPos> > _cursor;
            std::shared_ptr<Observable<TextEditSelection> > _selection;
            int _lineStart;
            std::vector<std::string> _oldLines;
            std::vector<std::string> _newLines;
            TextEditPos _cursorBefore;
            TextEditSelection _selectionBefore;
            TextEditPos _cursorAfter;
            TextEditSelection _selectionAfter;
        };
    }

    struct TextEditModel::Private
    {
        std::weak_ptr<Context> context;
        std::shared_ptr<ObservableList<std::string> > text;
        std::shared_ptr<Observable<bool> > readOnly;
        std::shared_ptr<Observable<TextEditPos> > cursor;
        std::shared_ptr<Observable<TextEditSelection> > selection;
        int pageRows = 0;
        std::shared_ptr<Observable<TextEditModelOptions> > options;
        std::shared_ptr<CommandStack> commandStack;
    };

    void TextEditModel::_init(
        const std::shared_ptr<Context>& context,
        const std::vector<std::string>& text)
    {
        FTK_P();
        p.context = context;
        p.text = ObservableList<std::string>::create(!text.empty() ? text : textEditClear);
        p.readOnly = Observable<bool>::create(false);
        p.cursor = Observable<TextEditPos>::create(TextEditPos(0, 0));
        p.selection = Observable<TextEditSelection>::create();
        p.options = Observable<TextEditModelOptions>::create();
        p.commandStack = CommandStack::create();
    }

    TextEditModel::TextEditModel() :
        _p(new Private)
    {}

    TextEditModel::~TextEditModel()
    {}

    std::shared_ptr<TextEditModel> TextEditModel::create(
        const std::shared_ptr<Context>& context,
        const std::vector<std::string>& text)
    {
        auto out = std::shared_ptr<TextEditModel>(new TextEditModel);
        out->_init(context, text);
        return out;
    }

    const std::vector<std::string>& TextEditModel::getText() const
    {
        return _p->text->get();
    }

    std::shared_ptr<IObservableList<std::string> > TextEditModel::observeText() const
    {
        return _p->text;
    }

    void TextEditModel::setText(const std::vector<std::string>& value)
    {
        FTK_P();
        const bool changed = value != p.text->get();
        if (changed)
        {
            p.cursor->setIfChanged(TextEditPos(0, 0));
            p.selection->setIfChanged(TextEditSelection());
            p.text->setIfChanged(!value.empty() ? value : textEditClear);
            p.commandStack->clear();
        }
    }

    void TextEditModel::clearText()
    {
        setText({});
    }

    bool TextEditModel::isReadOnly() const
    {
        return _p->readOnly->get();
    }

    std::shared_ptr<IObservable<bool> > TextEditModel::observeReadOnly() const
    {
        return _p->readOnly;
    }

    void TextEditModel::setReadOnly(bool value)
    {
        _p->readOnly->setIfChanged(value);
    }

    const TextEditPos& TextEditModel::getCursor() const
    {
        return _p->cursor->get();
    }

    std::shared_ptr<IObservable<TextEditPos> > TextEditModel::observeCursor() const
    {
        return _p->cursor;
    }

    void TextEditModel::setCursor(const TextEditPos& value)
    {
        FTK_P();
        const auto& text = p.text->get();
        TextEditPos tmp = value;
        tmp.line = clamp(tmp.line, 0, static_cast<int>(text.size()) - 1);
        tmp.chr = tmp.line < text.size() ?
            clamp(tmp.chr, 0, static_cast<int>(text[tmp.line].size())) :
            0;
        if (p.cursor->setIfChanged(tmp))
        {
            p.selection->setIfChanged(TextEditSelection());
        }
    }

    const TextEditSelection& TextEditModel::getSelection() const
    {
        return _p->selection->get();
    }

    std::shared_ptr<IObservable<TextEditSelection> > TextEditModel::observeSelection() const
    {
        return _p->selection;
    }

    void TextEditModel::setSelection(const TextEditSelection& value)
    {
        FTK_P();
        const auto& text = p.text->get();
        TextEditSelection tmp = value;
        tmp.first.line = clamp(tmp.first.line, 0, static_cast<int>(text.size()) - 1);
        tmp.first.chr =  tmp.first.line < text.size() ?
            clamp(tmp.first.chr, 0, static_cast<int>(text[tmp.first.line].size())) :
            0;
        tmp.second.line = clamp(tmp.second.line, 0, static_cast<int>(text.size()) - 1);
        tmp.second.chr = tmp.second.line < text.size() ?
            clamp(tmp.second.chr, 0, static_cast<int>(text[tmp.second.line].size())) :
            0;
        p.selection->setIfChanged(tmp);
    }

    void TextEditModel::selectAll()
    {
        FTK_P();
        p.selection->setIfChanged(_getSelectAll());
    }

    void TextEditModel::clearSelection()
    {
        FTK_P();
        p.selection->setIfChanged(TextEditSelection());
    }

    void TextEditModel::undo()
    {
        FTK_P();
        if (p.readOnly->get())
            return;
        p.commandStack->undo();
    }

    void TextEditModel::redo()
    {
        FTK_P();
        if (p.readOnly->get())
            return;
        p.commandStack->redo();
    }

    std::shared_ptr<IObservable<bool> > TextEditModel::observeHasUndo() const
    {
        return _p->commandStack->observeHasUndo();
    }

    std::shared_ptr<IObservable<bool> > TextEditModel::observeHasRedo() const
    {
        return _p->commandStack->observeHasRedo();
    }

    void TextEditModel::cut()
    {
        FTK_P();
        if (p.readOnly->get())
            return;
        if (auto context = p.context.lock())
        {
            const TextEditSelection selection = p.selection->get();
            if (selection.isValid())
            {
                auto clipboard = context->getSystem<ClipboardSystem>();
                const auto lines = _getSelection(selection);
                clipboard->setText(join(lines, '\n'));
                _replace(selection, {}, selection.min(), TextEditSelection());
            }
        }
    }

    void TextEditModel::copy()
    {
        FTK_P();
        if (auto context = p.context.lock())
        {
            const TextEditSelection& selection = p.selection->get();
            if (selection.isValid())
            {
                auto clipboard = context->getSystem<ClipboardSystem>();
                const auto lines = _getSelection(selection);
                clipboard->setText(join(lines, '\n'));
            }
        }
    }

    void TextEditModel::paste()
    {
        FTK_P();
        if (p.readOnly->get())
            return;
        if (auto context = p.context.lock())
        {
            auto clipboard = context->getSystem<ClipboardSystem>();
            const std::string clipboardText = clipboard->getText();
            if (!clipboardText.empty())
            {
                const auto lines = splitLines(clipboardText);
                const TextEditSelection selection = p.selection->get();
                const TextEditPos pos = selection.isValid() ?
                    selection.min() :
                    p.cursor->get();
                TextEditPos cursor = pos;
                if (1 == lines.size())
                {
                    cursor.chr = pos.chr + static_cast<int>(lines.front().size());
                }
                else
                {
                    cursor.line = pos.line + static_cast<int>(lines.size()) - 1;
                    cursor.chr = static_cast<int>(lines.back().size());
                }
                _replace(
                    selection.isValid() ? selection : TextEditSelection(pos, pos),
                    lines,
                    cursor,
                    TextEditSelection());
            }
        }
    }

    void TextEditModel::input(const std::string& value)
    {
        FTK_P();
        if (p.readOnly->get())
            return;
        const TextEditSelection selection = p.selection->get();
        const TextEditPos pos = selection.isValid() ?
            selection.min() :
            p.cursor->get();
        TextEditPos cursor = pos;
        cursor.chr = pos.chr + static_cast<int>(value.size());
        _replace(
            selection.isValid() ? selection : TextEditSelection(pos, pos),
            { value },
            cursor,
            TextEditSelection());
    }

    bool TextEditModel::key(Key key, int modifiers)
    {
        FTK_P();
        bool out = false;
        const bool readOnly = p.readOnly->get();
        switch (key)
        {
        case Key::Left:
        case Key::Right:
        case Key::Up:
        case Key::Down:
        case Key::Home:
        case Key::End:
        case Key::PageUp:
        case Key::PageDown:
            _move(key, modifiers);
            out = true;
            break;

        case Key::Backspace:
            if (!readOnly)
            {
                _backspace();
                out = true;
            }
            break;
        case Key::Delete:
            if (!readOnly)
            {
                _delete();
                out = true;
            }
            break;

        case Key::Return:
        case Key::KeypadEnter:
            if (!readOnly)
            {
                _return();
                out = true;
            }
            break;

        case Key::A:
            if (static_cast<int>(commandKeyModifier) == modifiers)
            {
                selectAll();
                out = true;
            }
            break;

        case Key::C:
            if (static_cast<int>(commandKeyModifier) == modifiers)
            {
                copy();
                out = true;
            }
            break;

        case Key::X:
            if (static_cast<int>(commandKeyModifier) == modifiers &&
                !readOnly)
            {
                cut();
                out = true;
            }
            break;

        case Key::V:
            if (static_cast<int>(commandKeyModifier) == modifiers &&
                !readOnly)
            {
                paste();
                out = true;
            }
            break;

        case Key::Y:
            if (static_cast<int>(commandKeyModifier) == modifiers &&
                !readOnly)
            {
                redo();
                out = true;
            }
            break;

        case Key::Z:
            if (static_cast<int>(commandKeyModifier) == modifiers &&
                !readOnly)
            {
                undo();
                out = true;
            }
            break;

        case Key::Tab:
            if (!readOnly)
            {
                _tab(modifiers);
                out = true;
            }
            break;

        default: break;
        }
        if (!out &&
            !readOnly &&
            (0 == modifiers || static_cast<int>(KeyModifier::Shift) == modifiers))
        {
            out = !isControlKey(key);
        }
        return out;
    }

    const TextEditModelOptions& TextEditModel::getOptions() const
    {
        return _p->options->get();
    }

    std::shared_ptr<IObservable<TextEditModelOptions> > TextEditModel::observeOptions() const
    {
        return _p->options;
    }

    void TextEditModel::setOptions(const TextEditModelOptions& value)
    {
        _p->options->setIfChanged(value);
    }

    void TextEditModel::setPageRows(int value)
    {
        _p->pageRows = value;
    }

    TextEditPos TextEditModel::_getNext(const TextEditPos& value) const
    {
        FTK_P();
        TextEditPos out = value;
        const auto& text = p.text->get();
        if (out.line >= 0 &&
            out.line < static_cast<int>(text.size()))
        {
            if (out.chr < static_cast<int>(text[out.line].size()))
            {
                ++out.chr;
            }
            else if (out.line < static_cast<int>(text.size()) - 1)
            {
                out.chr = 0;
                ++out.line;
            }
        }
        return out;
    }

    TextEditPos TextEditModel::_getPrev(const TextEditPos& value) const
    {
        FTK_P();
        TextEditPos out = value;
        const auto& text = p.text->get();
        if (out.line >= 0 &&
            out.line < static_cast<int>(text.size()))
        {
            if (out.chr > 0)
            {
                --out.chr;
            }
            else if (out.line > 0)
            {
                --out.line;
                out.chr = static_cast<int>(text[out.line].size());
            }
        }
        return out;
    }

    TextEditSelection TextEditModel::_getSelectAll() const
    {
        FTK_P();
        const auto& text = p.text->get();
        return !text.empty() ?
            TextEditSelection(
                TextEditPos(0, 0),
                TextEditPos(
                    static_cast<int>(text.size()) - 1,
                    static_cast<int>(text.back().size()))) :
            TextEditSelection();
    }

    std::vector<std::string> TextEditModel::_getSelection(const TextEditSelection& selection) const
    {
        FTK_P();
        std::vector<std::string> out;
        const auto& text = p.text->get();
        const TextEditPos min = selection.min();
        const TextEditPos max = selection.max();
        if (min.line == max.line)
        {
            out.push_back(text[min.line].substr(min.chr, max.chr - min.chr));
        }
        else
        {
            out.push_back(text[min.line].substr(min.chr));
            for (int i = min.line + 1; i < max.line; ++i)
            {
                out.push_back(text[i]);
            }
            if (max.line < text.size())
            {
                out.push_back(text[max.line].substr(0, max.chr));
            }
        }
        return out;
    }

    std::string TextEditModel::_getTabSpaces() const
    {
        return std::string(_p->options->get().tabSpaces, ' ');
    }

    void TextEditModel::_move(Key key, int modifiers)
    {
        FTK_P();
        const auto& text = p.text->get();
        TextEditPos cursor = p.cursor->get();
        TextEditSelection selection = p.selection->get();
        if (static_cast<int>(KeyModifier::Shift) == modifiers)
        {
            if (!selection.isValid())
            {
                selection = TextEditSelection(cursor);
            }
        }
        else
        {
            selection = TextEditSelection();
        }

        switch (key)
        {
        case Key::Left:
            cursor = _getPrev(cursor);
            break;
        case Key::Right:
            cursor = _getNext(cursor);
            break;

        case Key::Up:
            if (cursor.line > 0)
            {
                --cursor.line;
                cursor.chr = std::min(cursor.chr, static_cast<int>(text[cursor.line].size()));
            }
            break;
        case Key::Down:
            if (cursor.line < static_cast<int>(text.size() - 1))
            {
                ++cursor.line;
                cursor.chr = cursor.line < static_cast<int>(text.size()) ?
                    std::min(cursor.chr, static_cast<int>(text[cursor.line].size())) :
                    0;
            }
            break;

        case Key::Home:
            if (cursor.chr > 0)
            {
                cursor.chr = 0;
            }
            break;
        case Key::End:
            if (cursor.line < static_cast<int>(text.size()) &&
                cursor.chr < static_cast<int>(text[cursor.line].size()))
            {
                cursor.chr = static_cast<int>(text[cursor.line].size());
            }
            break;

        case Key::PageUp:
            if (cursor.line > 0)
            {
                cursor.line = std::max(0, cursor.line - p.pageRows);
                cursor.chr = std::min(cursor.chr, static_cast<int>(text[cursor.line].size()));
            }
            break;
        case Key::PageDown:
            if (cursor.line < static_cast<int>(text.size()))
            {
                cursor.line = std::min(cursor.line + p.pageRows, static_cast<int>(text.size()) - 1);
                cursor.chr = std::min(cursor.chr, static_cast<int>(text[cursor.line].size()));
            }
            break;

        default: break;
        }

        if (static_cast<int>(KeyModifier::Shift) == modifiers)
        {
            selection.second = cursor;
        }
        p.cursor->setIfChanged(cursor);
        p.selection->setIfChanged(selection);
    }

    void TextEditModel::_backspace()
    {
        FTK_P();
        const TextEditPos cursor = p.cursor->get();
        const TextEditSelection selection = p.selection->get();
        if (selection.isValid())
        {
            // Remove the selection.
            _replace(selection, {}, selection.min(), TextEditSelection());
        }
        else
        {
            const TextEditPos prev = _getPrev(cursor);
            if (cursor != prev)
            {
                _replace(TextEditSelection(cursor, prev), {}, prev, TextEditSelection());
            }
        }
    }

    void TextEditModel::_delete()
    {
        FTK_P();
        const TextEditPos cursor = p.cursor->get();
        const TextEditSelection selection = p.selection->get();
        if (selection.isValid())
        {
            // Remove the selection.
            _replace(selection, {}, selection.min(), TextEditSelection());
        }
        else
        {
            const TextEditPos next = _getNext(cursor);
            if (cursor != next)
            {
                _replace(TextEditSelection(cursor, next), {}, cursor, TextEditSelection());
            }
        }
    }

    void TextEditModel::_return()
    {
        FTK_P();
        const TextEditPos cursor = p.cursor->get();
        const TextEditSelection selection = p.selection->get();
        if (selection.isValid())
        {
            // Remove the selection.
            _replace(selection, {}, selection.second, TextEditSelection());
        }
        else
        {
            // Break the line at the cursor (when the cursor is at the start of
            // a line this inserts an empty line above it).
            _replace(
                TextEditSelection(cursor, cursor),
                { std::string(), std::string() },
                TextEditPos(cursor.line + 1, 0),
                TextEditSelection());
        }
    }

    void TextEditModel::_tab(int modifiers)
    {
        FTK_P();
        const auto& text = p.text->get();
        const TextEditPos cursor = p.cursor->get();
        const TextEditSelection selection = p.selection->get();
        if (0 == modifiers &&
            selection.isValid())
        {
            // Indent.
            const TextEditPos min = selection.min();
            const TextEditPos max = selection.max();
            TextEditSelection range(min, max);
            range.first.chr = 0;
            range.second.chr = max.line < static_cast<int>(text.size()) ?
                static_cast<int>(text[max.line].size()) : 0;
            std::vector<std::string> lines = _getSelection(range);
            const std::string indent = _getTabSpaces();
            for (auto& line : lines)
            {
                line.insert(0, indent);
            }
            const int tabSpaces = p.options->get().tabSpaces;
            TextEditPos cursorAfter = cursor;
            cursorAfter.chr += tabSpaces;
            TextEditSelection selectionAfter = selection;
            selectionAfter.first.chr += tabSpaces;
            selectionAfter.second.chr += tabSpaces;
            _replace(range, lines, cursorAfter, selectionAfter);
        }
        else if (
            static_cast<int>(KeyModifier::Shift) == modifiers &&
            selection.isValid())
        {
            // Un-indent.
            const TextEditPos min = selection.min();
            const TextEditPos max = selection.max();
            TextEditSelection range(min, max);
            range.first.chr = 0;
            range.second.chr = max.line < static_cast<int>(text.size()) ?
                static_cast<int>(text[max.line].size()) : 0;
            std::vector<std::string> lines = _getSelection(range);
            const int tabSpaces = p.options->get().tabSpaces;
            int lastSpacesRemoved = 0;
            for (auto& line : lines)
            {
                int j = 0;
                for (;
                    j < tabSpaces &&
                    j < line.size() &&
                    ' ' == line[j];
                    ++j)
                    ;
                if (j > 0)
                {
                    line.erase(0, j);
                }
                lastSpacesRemoved = j;
            }
            TextEditPos cursorAfter = cursor;
            cursorAfter.chr = std::max(0, cursor.chr - lastSpacesRemoved);
            TextEditSelection selectionAfter = selection;
            selectionAfter.first.chr = std::max(0, selection.first.chr - lastSpacesRemoved);
            selectionAfter.second.chr = std::max(0, selection.second.chr - lastSpacesRemoved);
            _replace(range, lines, cursorAfter, selectionAfter);
        }
        else if (
            cursor.line >= 0 &&
            cursor.line < static_cast<int>(text.size()) &&
            cursor.chr >= 0 &&
            cursor.chr <= text[cursor.line].size())
        {
            // Insert spaces.
            const int tabSpaces = p.options->get().tabSpaces;
            TextEditPos cursorAfter = cursor;
            cursorAfter.chr += tabSpaces;
            _replace(
                TextEditSelection(cursor, cursor),
                { _getTabSpaces() },
                cursorAfter,
                TextEditSelection());
        }
    }

    void TextEditModel::_replace(
        const TextEditSelection& selection,
        const std::vector<std::string>& value,
        const TextEditPos& cursorAfter,
        const TextEditSelection& selectionAfter)
    {
        FTK_P();
        const TextEditPos min = selection.min();
        const TextEditPos max = selection.max();
        const auto& text = p.text->get();
        int lineStart = 0;
        int lineEnd = 0;
        std::vector<std::string> newLines;
        if (selection == _getSelectAll())
        {
            lineStart = 0;
            lineEnd = static_cast<int>(text.size());
            newLines = !value.empty() ? value : textEditClear;
        }
        else if (min.line == max.line && value.size() <= 1)
        {
            const std::string& line = text[min.line];
            newLines.push_back(
                line.substr(0, min.chr) +
                (!value.empty() ? value.front() : std::string()) +
                line.substr(max.chr));
            lineStart = min.line;
            lineEnd = min.line + 1;
        }
        else if (min.line == max.line)
        {
            const std::string& line = text[min.line];
            newLines.push_back(line.substr(0, min.chr) + value.front());
            for (size_t i = 1; i + 1 < value.size(); ++i)
            {
                newLines.push_back(value[i]);
            }
            newLines.push_back(value.back() + line.substr(max.chr));
            lineStart = min.line;
            lineEnd = min.line + 1;
        }
        else if (value.size() <= 1)
        {
            std::string tmp = text[min.line].substr(0, min.chr);
            if (!value.empty())
            {
                tmp += value.front();
            }
            tmp += text[max.line].substr(max.chr);
            newLines.push_back(tmp);
            lineStart = min.line;
            lineEnd = max.line + 1;
        }
        else
        {
            newLines.push_back(text[min.line].substr(0, min.chr) + value.front());
            for (size_t i = 1; i + 1 < value.size(); ++i)
            {
                newLines.push_back(value[i]);
            }
            newLines.push_back(value.back() + text[max.line].substr(max.chr));
            lineStart = min.line;
            lineEnd = max.line + 1;
        }
        _edit(lineStart, lineEnd, newLines, cursorAfter, selectionAfter);
    }

    void TextEditModel::_edit(
        int lineStart,
        int lineEnd,
        const std::vector<std::string>& newLines,
        const TextEditPos& cursorAfter,
        const TextEditSelection& selectionAfter)
    {
        FTK_P();
        const auto& text = p.text->get();
        lineStart = std::max(0, std::min(lineStart, static_cast<int>(text.size())));
        lineEnd = std::max(lineStart, std::min(lineEnd, static_cast<int>(text.size())));
        const std::vector<std::string> oldLines(
            text.begin() + lineStart,
            text.begin() + lineEnd);
        const TextEditPos cursorBefore = p.cursor->get();
        const TextEditSelection selectionBefore = p.selection->get();
        if (oldLines == newLines &&
            cursorAfter == cursorBefore &&
            selectionAfter == selectionBefore)
        {
            return;
        }
        auto command = std::make_shared<TextEditCommand>(
            p.text,
            p.cursor,
            p.selection,
            lineStart,
            oldLines,
            newLines,
            cursorBefore,
            selectionBefore,
            cursorAfter,
            selectionAfter);
        p.commandStack->push(command);
    }

    void to_json(nlohmann::json& json, const TextEditModelOptions& value)
    {
        json["TabSpaces"] = value.tabSpaces;
    }

    void from_json(const nlohmann::json& json, TextEditModelOptions& value)
    {
        json.at("TabSpaces").get_to(value.tabSpaces);
    }

    std::ostream& operator << (std::ostream& os, const TextEditPos& value)
    {
        os << value.line << ":" << value.chr;
        return os;
    }

    std::ostream& operator << (std::ostream& os, const TextEditSelection& value)
    {
        os << value.first << "->" << value.second;
        return os;
    }
}
