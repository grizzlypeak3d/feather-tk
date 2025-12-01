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
    struct TextEditPos
    {
        TextEditPos() = default;
        TextEditPos(int line, int chr);

        int line = -1;
        int chr  = -1;

        bool isValid() const;

        bool operator == (const TextEditPos&) const;
        bool operator != (const TextEditPos&) const;
        bool operator < (const TextEditPos&) const;
        bool operator > (const TextEditPos&) const;
    };

    //! Text edit selection.
    struct TextEditSelection
    {
        TextEditSelection() = default;
        TextEditSelection(const TextEditPos&);
        TextEditSelection(const TextEditPos&, const TextEditPos&);

        TextEditPos first;
        TextEditPos second;

        //! Get whether the selection is valid.
        bool isValid() const;

        //! Get the minimum.
        TextEditPos min() const;

        //! Get the maximum.
        TextEditPos max() const;

        bool operator == (const TextEditSelection&) const;
        bool operator != (const TextEditSelection&) const;
    };

    //! Text edit model options.
    struct TextEditModelOptions
    {
        int  tabSpaces = 4;

        bool operator == (const TextEditModelOptions&) const;
        bool operator != (const TextEditModelOptions&) const;
    };

    //! Text edit model.
    //! 
    //! \todo Implement undo/redo.
    class TextEditModel : public std::enable_shared_from_this<TextEditModel>
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::vector<std::string>&);

        TextEditModel();

    public:
        virtual ~TextEditModel();

        //! Create a new text edit model.
        static std::shared_ptr<TextEditModel> create(
            const std::shared_ptr<Context>&,
            const std::vector<std::string>& = {});

        //! \name Text
        ///@{

        const std::vector<std::string>& getText() const;
        std::shared_ptr<IObservableList<std::string> > observeText() const;

        //! Set the text. Setting the text will also set the cursor to the
        //! beginning and clear the selection.
        void setText(const std::vector<std::string>&);

        void clearText();

        ///@}

        //! \name Cursor
        ///@{

        const TextEditPos& getCursor() const;
        std::shared_ptr<IObservable<TextEditPos> > observeCursor() const;

        //! Set the cursor. Setting the cursor clears the selection.
        void setCursor(const TextEditPos&);

        ///@}

        //! \name Selection
        ///@{

        const TextEditSelection& getSelection() const;
        std::shared_ptr<IObservable<TextEditSelection> > observeSelection() const;
        void setSelection(const TextEditSelection&);
        void selectAll();
        void clearSelection();

        ///@}

        //! \name Undo
        ///@{

        void undo();
        void redo();

        ///@}

        //! \name Clipboard
        ///@{

        void cut();
        void copy();
        void paste();

        ///@}

        //! \name Input
        ///@{

        //! Handle text input.
        void input(const std::string&);

        //! Handle key input.
        bool key(Key, int modifiers = 0);

        ///@}

        //! \name Options
        ///@{

        const TextEditModelOptions& getOptions() const;
        std::shared_ptr<IObservable<TextEditModelOptions> > observeOptions() const;
        void setOptions(const TextEditModelOptions&);

        ///@}

        //! Set the number of rows in a page.
        void setPageRows(int);

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

    void to_json(nlohmann::json&, const TextEditModelOptions&);

    void from_json(const nlohmann::json&, TextEditModelOptions&);

    std::ostream& operator << (std::ostream&, const TextEditPos&);
    std::ostream& operator << (std::ostream&, const TextEditSelection&);

    ///@}
}
