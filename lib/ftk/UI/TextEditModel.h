// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/Event.h>

#include <ftk/Core/ObservableList.h>
#include <ftk/Core/Observable.h>

namespace ftk
{
    //! \name Text Widgets
    ///@{

    //! Text edit position.
    struct FTK_UI_API_TYPE TextEditPos
    {
        TextEditPos() = default;
        FTK_UI_API TextEditPos(int line, int chr);

        //! The line, or -1 for none.
        int line = -1;
        //! The character, as a byte offset into the line, or -1 for none.
        int chr  = -1;

        //! Get whether the position is valid: neither the line nor the
        //! character is -1.
        FTK_UI_API bool isValid() const;

        bool operator == (const TextEditPos&) const = default;
        FTK_UI_API bool operator < (const TextEditPos&) const;
        FTK_UI_API bool operator > (const TextEditPos&) const;
    };

    //! Text edit selection.
    struct FTK_UI_API_TYPE TextEditSelection
    {
        TextEditSelection() = default;
        FTK_UI_API TextEditSelection(const TextEditPos&);
        FTK_UI_API TextEditSelection(const TextEditPos&, const TextEditPos&);

        //! The end the selection started from.
        TextEditPos first;
        //! The end the selection extends to.
        TextEditPos second;

        //! Get whether the selection is valid.
        FTK_UI_API bool isValid() const;

        //! Get the minimum.
        FTK_UI_API TextEditPos min() const;

        //! Get the maximum.
        FTK_UI_API TextEditPos max() const;

        bool operator == (const TextEditSelection&) const = default;
    };

    //! Text edit model options.
    struct FTK_UI_API_TYPE TextEditModelOptions
    {
        //! The number of spaces the Tab key inserts, and that it indents or,
        //! with Shift held, un-indents the selected lines by.
        int tabSpaces = 4;

        bool operator == (const TextEditModelOptions&) const = default;
    };

    //! Text edit model.
    //! 
    class FTK_UI_API_TYPE TextEditModel : public std::enable_shared_from_this<TextEditModel>
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::vector<std::string>&);

        TextEditModel();

    public:
        FTK_UI_API virtual ~TextEditModel();

        //! Create a new text edit model.
        FTK_UI_API static std::shared_ptr<TextEditModel> create(
            const std::shared_ptr<Context>&,
            const std::vector<std::string>& = {});

        //! \name Text
        ///@{

        //! Get the text, one string for each line.
        FTK_UI_API const std::vector<std::string>& getText() const;
        //! Observe the text.
        FTK_UI_API std::shared_ptr<IObservableList<std::string> > observeText() const;

        //! Set the text. Setting the text will also set the cursor to the
        //! beginning and clear the selection.
        FTK_UI_API void setText(const std::vector<std::string>&);

        //! Clear the text, leaving one empty line. Like setting the text, this
        //! also clears the undo history.
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

        //! Get the cursor position.
        FTK_UI_API const TextEditPos& getCursor() const;
        //! Observe the cursor position.
        FTK_UI_API std::shared_ptr<IObservable<TextEditPos> > observeCursor() const;

        //! Set the cursor. Setting the cursor clears the selection.
        FTK_UI_API void setCursor(const TextEditPos&);

        ///@}

        //! \name Selection
        ///@{

        //! Get the selection.
        FTK_UI_API const TextEditSelection& getSelection() const;
        //! Observe the selection.
        FTK_UI_API std::shared_ptr<IObservable<TextEditSelection> > observeSelection() const;
        //! Set the selection. Its ends are clamped to the text.
        FTK_UI_API void setSelection(const TextEditSelection&);
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
        //! Nothing is done while the text is read only.
        FTK_UI_API void paste();

        ///@}

        //! \name Input
        ///@{

        //! Handle text input.
        FTK_UI_API void input(const std::string&);

        //! Handle key input.
        FTK_UI_API bool key(Key, int modifiers = 0);

        ///@}

        //! \name Options
        ///@{

        //! Get the options.
        FTK_UI_API const TextEditModelOptions& getOptions() const;
        //! Observe the options.
        FTK_UI_API std::shared_ptr<IObservable<TextEditModelOptions> > observeOptions() const;
        //! Set the options.
        FTK_UI_API void setOptions(const TextEditModelOptions&);

        ///@}

        //! Set the number of rows in a page.
        FTK_UI_API void setPageRows(int);

    private:
        TextEditPos _getNext(const TextEditPos&) const;
        TextEditPos _getPrev(const TextEditPos&) const;

        TextEditSelection _getSelectAll() const;
        std::vector<std::string> _getSelection(const TextEditSelection&) const;

        std::string _getTabSpaces() const;

        void _move(Key, int modifiers);
        void _backspace();
        void _delete();
        void _return();
        void _tab(int modifiers);

        void _replace(
            const TextEditSelection&,
            const std::vector<std::string>&,
            const TextEditPos& cursorAfter,
            const TextEditSelection& selectionAfter);
        void _edit(
            int lineStart,
            int lineEnd,
            const std::vector<std::string>& newLines,
            const TextEditPos& cursorAfter,
            const TextEditSelection& selectionAfter);

        FTK_PRIVATE();
    };

    FTK_UI_API void to_json(nlohmann::json&, const TextEditModelOptions&);

    FTK_UI_API void from_json(const nlohmann::json&, TextEditModelOptions&);

    FTK_UI_API std::ostream& operator << (std::ostream&, const TextEditPos&);
    FTK_UI_API std::ostream& operator << (std::ostream&, const TextEditSelection&);

    ///@}
}
