// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/LineEditModel.h>

#include <ftk/UI/ClipboardSystem.h>

#include <ftk/Core/Command.h>

#include <optional>
#include <regex>

namespace ftk
{
    namespace
    {
        //! One undoable text edit: replace [pos, pos + removed.size()) with
        //! the inserted string. The inverse swaps removed and inserted. The
        //! cursor and selection are restored to their pre-edit values on undo.
        class LineEditCommand : public ICommand
        {
        public:
            LineEditCommand(
                const std::shared_ptr<Observable<std::string> >& text,
                const std::shared_ptr<Observable<int> >& cursor,
                const std::shared_ptr<Observable<LineEditSelection> >& selection,
                int pos,
                const std::string& removed,
                const std::string& inserted,
                int cursorBefore,
                const LineEditSelection& selectionBefore,
                int cursorAfter,
                const LineEditSelection& selectionAfter) :
                _text(text),
                _cursor(cursor),
                _selection(selection),
                _pos(pos),
                _removed(removed),
                _inserted(inserted),
                _cursorBefore(cursorBefore),
                _selectionBefore(selectionBefore),
                _cursorAfter(cursorAfter),
                _selectionAfter(selectionAfter)
            {}

            void exec() override
            {
                std::string text = _text->get();
                text.replace(_pos, _removed.size(), _inserted);
                _text->setIfChanged(text);
                _cursor->setIfChanged(_cursorAfter);
                _selection->setIfChanged(_selectionAfter);
            }

            void undo() override
            {
                std::string text = _text->get();
                text.replace(_pos, _inserted.size(), _removed);
                _text->setIfChanged(text);
                _cursor->setIfChanged(_cursorBefore);
                _selection->setIfChanged(_selectionBefore);
            }

        private:
            std::shared_ptr<Observable<std::string> > _text;
            std::shared_ptr<Observable<int> > _cursor;
            std::shared_ptr<Observable<LineEditSelection> > _selection;
            int _pos = 0;
            std::string _removed;
            std::string _inserted;
            int _cursorBefore = 0;
            LineEditSelection _selectionBefore;
            int _cursorAfter = 0;
            LineEditSelection _selectionAfter;
        };
    }

    LineEditSelection::LineEditSelection(int first) :
        first(first)
    {}

    LineEditSelection::LineEditSelection(int first, int second) :
        first(first),
        second(second)
    {}

    bool LineEditSelection::isValid() const
    {
        return
            first != -1 &&
            second != -1 &&
            first != second;
    }

    int LineEditSelection::min() const
    {
        return std::min(first, second);
    }

    int LineEditSelection::max() const
    {
        return std::max(first, second);
    }

    bool LineEditSelection::operator == (const LineEditSelection& other) const
    {
        return
            first == other.first &&
            second == other.second;
    }

    bool LineEditSelection::operator != (const LineEditSelection& other) const
    {
        return !(*this == other);
    }

    struct LineEditModel::Private
    {
        std::weak_ptr<Context> context;
        std::shared_ptr<Observable<std::string> > text;
        std::shared_ptr<Observable<bool> > readOnly;
        std::shared_ptr<Observable<int> > cursor;
        std::shared_ptr<Observable<LineEditSelection> > selection;
        std::string regex;
        std::optional<std::regex> regexCompiled;
        std::shared_ptr<CommandStack> commandStack;
    };

    void LineEditModel::_init(
        const std::shared_ptr<Context>& context,
        const std::string& text)
    {
        FTK_P();
        p.context = context;
        p.text = Observable<std::string>::create(text);
        p.readOnly = Observable<bool>::create(false);
        p.cursor = Observable<int>::create(0);
        p.selection = Observable<LineEditSelection>::create();
        p.commandStack = CommandStack::create();
    }

    LineEditModel::LineEditModel() :
        _p(new Private)
    {}

    LineEditModel::~LineEditModel()
    {}

    std::shared_ptr<LineEditModel> LineEditModel::create(
        const std::shared_ptr<Context>& context,
        const std::string& text)
    {
        auto out = std::shared_ptr<LineEditModel>(new LineEditModel);
        out->_init(context, text);
        return out;
    }

    const std::string& LineEditModel::getText() const
    {
        return _p->text->get();
    }

    std::shared_ptr<IObservable<std::string> > LineEditModel::observeText() const
    {
        return _p->text;
    }

    void LineEditModel::setText(const std::string& value)
    {
        FTK_P();
        if (p.text->setIfChanged(value))
        {
            p.cursor->setIfChanged(0);
            p.selection->setIfChanged(LineEditSelection());
            p.commandStack->clear();
        }
    }

    void LineEditModel::clearText()
    {
        setText(std::string());
    }

    bool LineEditModel::isReadOnly() const
    {
        return _p->readOnly->get();
    }

    std::shared_ptr<IObservable<bool> > LineEditModel::observeReadOnly() const
    {
        return _p->readOnly;
    }

    void LineEditModel::setReadOnly(bool value)
    {
        _p->readOnly->setIfChanged(value);
    }

    int LineEditModel::getCursor() const
    {
        return _p->cursor->get();
    }

    std::shared_ptr<IObservable<int> > LineEditModel::observeCursor() const
    {
        return _p->cursor;
    }

    void LineEditModel::setCursor(int value)
    {
        FTK_P();
        const auto& text = p.text->get();
        int tmp = clamp(value, 0, static_cast<int>(text.size()));
        if (p.cursor->setIfChanged(tmp))
        {
            p.selection->setIfChanged(LineEditSelection());
        }
    }

    const LineEditSelection& LineEditModel::getSelection() const
    {
        return _p->selection->get();
    }

    std::shared_ptr<IObservable<LineEditSelection> > LineEditModel::observeSelection() const
    {
        return _p->selection;
    }

    void LineEditModel::setSelection(const LineEditSelection& value)
    {
        FTK_P();
        const auto& text = p.text->get();
        const LineEditSelection tmp(
            clamp(value.first, 0, static_cast<int>(text.size())),
            clamp(value.second, 0, static_cast<int>(text.size())));
        p.selection->setIfChanged(tmp);
    }

    void LineEditModel::selectAll()
    {
        FTK_P();
        LineEditSelection selection;
        const auto& text = p.text->get();
        if (!text.empty())
        {
            selection.first = 0;
            selection.second = static_cast<int>(text.size());
        }
        p.selection->setIfChanged(selection);
    }

    void LineEditModel::clearSelection()
    {
        FTK_P();
        p.selection->setIfChanged(LineEditSelection());
    }

    void LineEditModel::undo()
    {
        FTK_P();
        if (p.readOnly->get())
            return;
        p.commandStack->undo();
    }

    void LineEditModel::redo()
    {
        FTK_P();
        if (p.readOnly->get())
            return;
        p.commandStack->redo();
    }

    std::shared_ptr<IObservable<bool> > LineEditModel::observeHasUndo() const
    {
        return _p->commandStack->observeHasUndo();
    }

    std::shared_ptr<IObservable<bool> > LineEditModel::observeHasRedo() const
    {
        return _p->commandStack->observeHasRedo();
    }

    void LineEditModel::cut()
    {
        FTK_P();
        if (p.readOnly->get())
            return;
        if (auto context = p.context.lock())
        {
            const LineEditSelection selection = p.selection->get();
            if (selection.isValid())
            {
                auto clipboard = context->getSystem<ClipboardSystem>();
                clipboard->setText(p.text->get().substr(
                    selection.min(),
                    selection.max() - selection.min()));
                _edit(
                    selection.min(),
                    selection.max() - selection.min(),
                    std::string(),
                    selection.min(),
                    LineEditSelection());
            }
        }
    }

    void LineEditModel::copy()
    {
        FTK_P();
        if (auto context = p.context.lock())
        {
            const LineEditSelection& selection = p.selection->get();
            if (selection.isValid())
            {
                auto clipboard = context->getSystem<ClipboardSystem>();
                clipboard->setText(p.text->get().substr(
                    selection.min(),
                    selection.max() - selection.min()));
            }
        }
    }

    void LineEditModel::paste()
    {
        FTK_P();
        if (p.readOnly->get())
            return;
        if (auto context = p.context.lock())
        {
            auto clipboard = context->getSystem<ClipboardSystem>();
            const std::string clipboardText = clipboard->getText();
            if (!clipboardText.empty() &&
                (!p.regexCompiled.has_value() ||
                    std::regex_match(clipboardText, p.regexCompiled.value())))
            {
                const LineEditSelection selection = p.selection->get();
                const int pos = selection.isValid() ?
                    selection.min() :
                    p.cursor->get();
                const int removeCount = selection.isValid() ?
                    (selection.max() - selection.min()) :
                    0;
                _edit(
                    pos,
                    removeCount,
                    clipboardText,
                    pos + static_cast<int>(clipboardText.size()),
                    LineEditSelection());
            }
        }
    }

    void LineEditModel::input(const std::string& value)
    {
        FTK_P();
        if (p.readOnly->get())
            return;
        if (!p.regexCompiled.has_value() ||
            std::regex_match(value, p.regexCompiled.value()))
        {
            const LineEditSelection selection = p.selection->get();
            const int pos = selection.isValid() ?
                selection.min() :
                p.cursor->get();
            const int removeCount = selection.isValid() ?
                (selection.max() - selection.min()) :
                0;
            _edit(
                pos,
                removeCount,
                value,
                pos + static_cast<int>(value.size()),
                LineEditSelection());
        }
    }

    bool LineEditModel::key(Key key, int modifiers)
    {
        FTK_P();
        bool out = false;
        switch (key)
        {
        case Key::Left:
        case Key::Right:
        case Key::Home:
        case Key::End:
            _move(key, modifiers);
            out = true;
            break;

        case Key::Backspace:
            if (!p.readOnly->get())
            {
                _backspace();
                out = true;
            }
            break;
        case Key::Delete:
            if (!p.readOnly->get())
            {
                _delete();
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
                !p.readOnly->get())
            {
                cut();
                out = true;
            }
            break;

        case Key::V:
            if (static_cast<int>(commandKeyModifier) == modifiers &&
                !p.readOnly->get())
            {
                paste();
                out = true;
            }
            break;

        case Key::Y:
            if (static_cast<int>(commandKeyModifier) == modifiers &&
                !p.readOnly->get())
            {
                redo();
                out = true;
            }
            break;

        case Key::Z:
            if (static_cast<int>(commandKeyModifier) == modifiers &&
                !p.readOnly->get())
            {
                undo();
                out = true;
            }
            break;

        default: break;
        }
        if (!out &&
            key != Key::Tab &&
            (0 == modifiers || static_cast<int>(KeyModifier::Shift) == modifiers))
        {
            out = !isControlKey(key);
        }
        return out;
    }
    
    const std::string& LineEditModel::getRegex() const
    {
        return _p->regex;
    }

    FTK_API void LineEditModel::setRegex(const std::string& value)
    {
        FTK_P();
        p.regex = value;
        p.regexCompiled.reset();
        if (!value.empty())
        {
            try
            {
                p.regexCompiled = std::regex(value);
            }
            catch (const std::exception&)
            {
                // Invalid pattern: fall back to no filtering rather than
                // throwing on every input.
                p.regexCompiled.reset();
            }
        }
    }

    void LineEditModel::_move(Key key, int modifiers)
    {
        FTK_P();
        int cursor = p.cursor->get();
        LineEditSelection selection = p.selection->get();
        if (static_cast<int>(KeyModifier::Shift) == modifiers)
        {
            if (!selection.isValid())
            {
                selection = LineEditSelection(cursor);
            }
        }
        else
        {
            selection = LineEditSelection();
        }

        const auto& text = p.text->get();
        switch (key)
        {
        case Key::Left:
            cursor = std::max(cursor - 1, 0);
            break;
        case Key::Right:
            cursor = std::min(cursor + 1, static_cast<int>(text.size()));
            break;

        case Key::Home:
            cursor = 0;
            break;
        case Key::End:
            cursor = static_cast<int>(text.size());
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

    void LineEditModel::_backspace()
    {
        FTK_P();
        const int cursor = p.cursor->get();
        const LineEditSelection selection = p.selection->get();
        if (selection.isValid())
        {
            // Remove the selection.
            _edit(
                selection.min(),
                selection.max() - selection.min(),
                std::string(),
                selection.min(),
                LineEditSelection());
        }
        else
        {
            const int prev = std::max(cursor - 1, 0);
            if (cursor != prev)
            {
                _edit(prev, cursor - prev, std::string(), prev, LineEditSelection());
            }
        }
    }

    void LineEditModel::_delete()
    {
        FTK_P();
        const int cursor = p.cursor->get();
        const LineEditSelection selection = p.selection->get();
        if (selection.isValid())
        {
            // Remove the selection.
            _edit(
                selection.min(),
                selection.max() - selection.min(),
                std::string(),
                selection.min(),
                LineEditSelection());
        }
        else
        {
            const int next = std::min(cursor + 1, static_cast<int>(p.text->get().size()));
            if (cursor != next)
            {
                _edit(cursor, next - cursor, std::string(), cursor, LineEditSelection());
            }
        }
    }

    void LineEditModel::_edit(
        int pos,
        int removeCount,
        const std::string& insert,
        int cursor,
        const LineEditSelection& selection)
    {
        FTK_P();
        const std::string& text = p.text->get();
        pos = clamp(pos, 0, static_cast<int>(text.size()));
        removeCount = clamp(removeCount, 0, static_cast<int>(text.size()) - pos);
        const std::string removed = text.substr(pos, removeCount);
        if (removed.empty() && insert.empty())
            return;
        auto command = std::make_shared<LineEditCommand>(
            p.text,
            p.cursor,
            p.selection,
            pos,
            removed,
            insert,
            p.cursor->get(),
            p.selection->get(),
            cursor,
            selection);
        p.commandStack->push(command);
    }
}
