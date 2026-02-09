// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Event.h>

#include <ftk/Core/ObservableList.h>
#include <ftk/Core/Observable.h>

namespace ftk
{
    //! \name Text Widgets
    ///@{

    //! Text edit position.
    struct FTK_API_TYPE TextEditPos
    {
        TextEditPos() = default;
        FTK_API TextEditPos(int line, int chr);

        int line = -1;
        int chr  = -1;

        FTK_API bool isValid() const;

        FTK_API bool operator == (const TextEditPos&) const;
        FTK_API bool operator != (const TextEditPos&) const;
        FTK_API bool operator < (const TextEditPos&) const;
        FTK_API bool operator > (const TextEditPos&) const;
    };

    //! Text edit selection.
    struct FTK_API_TYPE TextEditSelection
    {
        TextEditSelection() = default;
        FTK_API TextEditSelection(const TextEditPos&);
        FTK_API TextEditSelection(const TextEditPos&, const TextEditPos&);

        TextEditPos first;
        TextEditPos second;

        //! Get whether the selection is valid.
        FTK_API bool isValid() const;

        //! Get the minimum.
        FTK_API TextEditPos min() const;

        //! Get the maximum.
        FTK_API TextEditPos max() const;

        FTK_API bool operator == (const TextEditSelection&) const;
        FTK_API bool operator != (const TextEditSelection&) const;
    };

    //! Text edit model options.
    struct FTK_API_TYPE TextEditModelOptions
    {
        int tabSpaces = 4;

        FTK_API bool operator == (const TextEditModelOptions&) const;
        FTK_API bool operator != (const TextEditModelOptions&) const;
    };

    //! Text edit model.
    //! 
    //! \todo Implement undo/redo.
    class FTK_API_TYPE TextEditModel : public std::enable_shared_from_this<TextEditModel>
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::vector<std::string>&);

        TextEditModel();

    public:
        FTK_API virtual ~TextEditModel();

        //! Create a new text edit model.
        FTK_API static std::shared_ptr<TextEditModel> create(
            const std::shared_ptr<Context>&,
            const std::vector<std::string>& = {});

        //! \name Text
        ///@{

        FTK_API const std::vector<std::string>& getText() const;
        FTK_API std::shared_ptr<IObservableList<std::string> > observeText() const;

        //! Set the text. Setting the text will also set the cursor to the
        //! beginning and clear the selection.
        FTK_API void setText(const std::vector<std::string>&);

        FTK_API void clearText();

        FTK_API bool isReadOnly() const;
        FTK_API std::shared_ptr<IObservable<bool> > observeReadOnly() const;
        FTK_API void setReadOnly(bool);

        ///@}

        //! \name Cursor
        ///@{

        FTK_API const TextEditPos& getCursor() const;
        FTK_API std::shared_ptr<IObservable<TextEditPos> > observeCursor() const;

        //! Set the cursor. Setting the cursor clears the selection.
        FTK_API void setCursor(const TextEditPos&);

        ///@}

        //! \name Selection
        ///@{

        FTK_API const TextEditSelection& getSelection() const;
        FTK_API std::shared_ptr<IObservable<TextEditSelection> > observeSelection() const;
        FTK_API void setSelection(const TextEditSelection&);
        FTK_API void selectAll();
        FTK_API void clearSelection();

        ///@}

        //! \name Undo
        ///@{

        FTK_API void undo();
        FTK_API void redo();

        ///@}

        //! \name Clipboard
        ///@{

        FTK_API void cut();
        FTK_API void copy();
        FTK_API void paste();

        ///@}

        //! \name Input
        ///@{

        //! Handle text input.
        FTK_API void input(const std::string&);

        //! Handle key input.
        FTK_API bool key(Key, int modifiers = 0);

        ///@}

        //! \name Options
        ///@{

        FTK_API const TextEditModelOptions& getOptions() const;
        FTK_API std::shared_ptr<IObservable<TextEditModelOptions> > observeOptions() const;
        FTK_API void setOptions(const TextEditModelOptions&);

        ///@}

        //! Set the number of rows in a page.
        FTK_API void setPageRows(int);

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
            const std::vector<std::string>&);

        FTK_PRIVATE();
    };

    FTK_API void to_json(nlohmann::json&, const TextEditModelOptions&);

    FTK_API void from_json(const nlohmann::json&, TextEditModelOptions&);

    FTK_API std::ostream& operator << (std::ostream&, const TextEditPos&);
    FTK_API std::ostream& operator << (std::ostream&, const TextEditSelection&);

    ///@}
}
