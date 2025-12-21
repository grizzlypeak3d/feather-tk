// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UI/LineEditModel.h>

#include <ftk/UI/ClipboardSystem.h>

namespace ftk
{
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
        std::shared_ptr<Observable<int> > cursor;
        std::shared_ptr<Observable<LineEditSelection> > selection;
    };

    void LineEditModel::_init(
        const std::shared_ptr<Context>& context,
        const std::string& text)
    {
        FTK_P();
        p.context = context;
        p.text = Observable<std::string>::create(text);
        p.cursor = Observable<int>::create(0);
        p.selection = Observable<LineEditSelection>::create();
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
        }
    }

    void LineEditModel::clearText()
    {
        setText(std::string());
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
            selection.second = text.size();
        }
        p.selection->setIfChanged(selection);
    }

    void LineEditModel::clearSelection()
    {
        FTK_P();
        p.selection->setIfChanged(LineEditSelection());
    }

    void LineEditModel::undo()
    {}

    void LineEditModel::redo()
    {}

    void LineEditModel::cut()
    {
        FTK_P();
        if (auto context = p.context.lock())
        {
            auto clipboard = context->getSystem<ClipboardSystem>();
            std::string clipboardText;
            int cursor = p.cursor->get();
            LineEditSelection selection = p.selection->get();
            if (selection.isValid())
            {
                clipboardText = p.text->get().substr(
                    selection.min(),
                    selection.max() - selection.min());
                _replace(selection, "");
                cursor = selection.min();
                selection = LineEditSelection();
            }
            clipboard->setText(clipboardText);
            p.cursor->setIfChanged(cursor);
            p.selection->setIfChanged(selection);
        }
    }

    void LineEditModel::copy()
    {
        FTK_P();
        if (auto context = p.context.lock())
        {
            auto clipboard = context->getSystem<ClipboardSystem>();
            std::string clipboardText;
            const LineEditSelection& selection = p.selection->get();
            if (selection.isValid())
            {
                clipboardText = p.text->get().substr(
                    selection.min(),
                    selection.max() - selection.min());
            }
            clipboard->setText(clipboardText);
        }
    }

    void LineEditModel::paste()
    {
        FTK_P();
        if (auto context = p.context.lock())
        {
            auto clipboard = context->getSystem<ClipboardSystem>();
            const std::string clipboardText = clipboard->getText();
            if (!clipboardText.empty())
            {
                int cursor = p.cursor->get();
                LineEditSelection selection = p.selection->get();
                if (selection.isValid())
                {
                    _replace(selection, clipboardText);
                    cursor = selection.min() + clipboardText.size();
                    selection = LineEditSelection();
                }
                else
                {
                    std::string text = p.text->get();
                    text.insert(cursor, clipboardText);
                    p.text->setIfChanged(text);
                    cursor += clipboardText.size();
                }
                p.cursor->setIfChanged(cursor);
                p.selection->setIfChanged(selection);
            }
        }
    }

    void LineEditModel::input(const std::string& value)
    {
        FTK_P();
        int cursor = p.cursor->get();
        LineEditSelection selection = p.selection->get();

        if (selection.isValid())
        {
            // Replace the selection.
            _replace(selection, value);
            cursor = selection.min() + value.size();
            selection = LineEditSelection();
        }
        else
        {
            // Insert text at the cursor.
            std::string text = p.text->get();
            text.insert(cursor, value);
            cursor += value.size();
            p.text->setIfChanged(text);
        }

        p.cursor->setIfChanged(cursor);
        p.selection->setIfChanged(selection);
    }

    bool LineEditModel::key(Key key, int modifiers)
    {
        FTK_P();
        bool out = false;
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
            _backspace();
            out = true;
            break;
        case Key::Delete:
            _delete();
            out = true;
            break;

        case Key::A:
            if (static_cast<int>(KeyModifier::Control) == modifiers)
            {
                selectAll();
                out = true;
            }
            break;

        case Key::C:
            if (static_cast<int>(KeyModifier::Control) == modifiers)
            {
                copy();
                out = true;
            }
            break;

        case Key::X:
            if (static_cast<int>(KeyModifier::Control) == modifiers)
            {
                cut();
                out = true;
            }
            break;

        case Key::V:
            if (static_cast<int>(KeyModifier::Control) == modifiers)
            {
                paste();
                out = true;
            }
            break;

        case Key::Y:
            if (static_cast<int>(KeyModifier::Control) == modifiers)
            {
                redo();
                out = true;
            }
            break;

        case Key::Z:
            if (static_cast<int>(KeyModifier::Control) == modifiers)
            {
                undo();
                out = true;
            }
            break;

        default: break;
        }
        return out;
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
            cursor = text.size();
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
        int cursor = p.cursor->get();
        LineEditSelection selection = p.selection->get();
        if (selection.isValid())
        {
            // Remove the selection.
            _replace(selection, "");
            cursor = selection.min();
            selection = LineEditSelection();
        }
        else
        {
            const int prev = std::max(cursor - 1, 0);
            if (cursor != prev)
            {
                _replace(LineEditSelection(cursor, prev), {});
                cursor = prev;
            }
        }
        p.cursor->setIfChanged(cursor);
        p.selection->setIfChanged(selection);
    }

    void LineEditModel::_delete()
    {
        FTK_P();
        int cursor = p.cursor->get();
        LineEditSelection selection = p.selection->get();
        if (selection.isValid())
        {
            // Remove the selection.
            _replace(selection, {});
            cursor = selection.min();
            selection = LineEditSelection();
        }
        else
        {
            const auto& text = p.text->get();
            const int next = std::min(cursor + 1, static_cast<int>(text.size()));
            if (cursor != next)
            {
                _replace(LineEditSelection(cursor, next), {});
            }
        }
        p.cursor->setIfChanged(cursor);
        p.selection->setIfChanged(selection);
    }

    void LineEditModel::_replace(
        const LineEditSelection& selection,
        const std::string& value)
    {
        FTK_P();
        if (selection.isValid())
        {
            std::string text = p.text->get();
            text.replace(selection.min(), selection.max() - selection.min(), value);
            p.text->setIfChanged(text);
        }
    }
}
