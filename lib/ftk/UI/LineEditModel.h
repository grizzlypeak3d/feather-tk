// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/Event.h>

#include <ftk/Core/Observable.h>

namespace ftk
{
    //! \name Text Widgets
    ///@{

    //! Line edit selection.
    struct FTK_UI_API_TYPE LineEditSelection
    {
        LineEditSelection() = default;
        FTK_UI_API LineEditSelection(int);
        FTK_UI_API LineEditSelection(int, int);

        //! The end the selection started from, as a byte offset into the text,
        //! or -1 for none.
        int first = -1;
        //! The end the selection extends to, as a byte offset into the text, or
        //! -1 for none.
        int second = -1;

        //! Get whether the selection is valid.
        FTK_UI_API bool isValid() const;

        //! Get the minimum.
        FTK_UI_API int min() const;

        //! Get the maximum.
        FTK_UI_API int max() const;

        bool operator == (const LineEditSelection&) const = default;
    };
        
    //! Line edit model.
    //! 
    class FTK_UI_API_TYPE LineEditModel : public std::enable_shared_from_this<LineEditModel>
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& = "");

        LineEditModel();

    public:
        FTK_UI_API virtual ~LineEditModel();

        //! Create a new model
        FTK_UI_API static std::shared_ptr<LineEditModel> create(
            const std::shared_ptr<Context>&,
            const std::string& = "");

        //! \name Text
        ///@{

        //! Get the text.
        FTK_UI_API const std::string& getText() const;
        //! Observe the text.
        FTK_UI_API std::shared_ptr<IObservable<std::string> > observeText() const;

        //! Set the text. Setting the text will also set the cursor to the
        //! beginning and clear the selection.
        FTK_UI_API void setText(const std::string&);

        //! Clear the text. Like setting the text, this also clears the undo
        //! history.
        FTK_UI_API void clearText();

        //! Get whether the text is read only. Read only text can still be
        //! selected and copied, but not edited.
        FTK_UI_API bool isReadOnly() const;
        //! Observe whether the text is read only.
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeReadOnly() const;
        //! Set whether the text is read only.
        FTK_UI_API void setReadOnly(bool);

        ///@}

        //! \name Cursor
        ///@{

        //! Get the cursor position, as a byte offset into the text.
        FTK_UI_API int getCursor() const;
        //! Observe the cursor position.
        FTK_UI_API std::shared_ptr<IObservable<int> > observeCursor() const;

        //! Set the cursor. Setting the cursor clears the selection.
        FTK_UI_API void setCursor(int);

        ///@}

        //! \name Selection
        ///@{

        //! Get the selection.
        FTK_UI_API const LineEditSelection& getSelection() const;
        //! Observe the selection.
        FTK_UI_API std::shared_ptr<IObservable<LineEditSelection> > observeSelection() const;
        //! Set the selection. Its ends are clamped to the text.
        FTK_UI_API void setSelection(const LineEditSelection&);
        //! Select all of the text.
        FTK_UI_API void selectAll();
        //! Clear the selection.
        FTK_UI_API void clearSelection();

        ///@}

        //! \name Undo
        ///@{

        //! Undo the last edit. Nothing is done while the text is read only.
        FTK_UI_API void undo();
        //! Redo the last edit that was undone. Nothing is done while the text
        //! is read only.
        FTK_UI_API void redo();

        //! Observe whether there is a command to undo.
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeHasUndo() const;

        //! Observe whether there is a command to redo.
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeHasRedo() const;

        ///@}

        //! \name Clipboard
        ///@{

        //! Copy the selection to the clipboard and remove it. Nothing is done
        //! while the text is read only.
        FTK_UI_API void cut();
        //! Copy the selection to the clipboard.
        FTK_UI_API void copy();
        //! Paste the clipboard's text over the selection, or at the cursor.
        //! Text that does not match the regular expression is not pasted, and
        //! nothing is done while the text is read only.
        FTK_UI_API void paste();

        ///@}

        //! \name Input
        ///@{

        //! Handle text input.
        FTK_UI_API void input(const std::string&);

        //! Handle key input.
        FTK_UI_API bool key(Key, int modifiers = 0);

        //! Get the regular expression for allowable input.
        FTK_UI_API const std::string& getRegex() const;

        //! Set the regular expression for allowable input.
        FTK_UI_API void setRegex(const std::string&);

        ///@}

    private:
        void _move(Key, int modifiers);
        void _backspace();
        void _delete();

        //! The single text-mutation funnel: replace [pos, pos + removeCount)
        //! with the insert string, moving to the given cursor and selection.
        //! Each call is recorded as one undoable command.
        void _edit(
            int pos,
            int removeCount,
            const std::string& insert,
            int cursor,
            const LineEditSelection& selection);

        FTK_PRIVATE();
    };
        
    ///@}
}
