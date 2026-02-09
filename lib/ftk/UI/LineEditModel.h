// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Event.h>

#include <ftk/Core/Observable.h>

namespace ftk
{
    //! \name Text Widgets
    ///@{

    //! Line edit selection.
    struct FTK_API_TYPE LineEditSelection
    {
        LineEditSelection() = default;
        FTK_API LineEditSelection(int);
        FTK_API LineEditSelection(int, int);

        int first = -1;
        int second = -1;

        //! Get whether the selection is valid.
        FTK_API bool isValid() const;

        //! Get the minimum.
        FTK_API int min() const;

        //! Get the maximum.
        FTK_API int max() const;

        FTK_API bool operator == (const LineEditSelection&) const;
        FTK_API bool operator != (const LineEditSelection&) const;
    };
        
    //! Line edit model.
    //! 
    //! \todo Implement undo/redo.
    class FTK_API_TYPE LineEditModel : public std::enable_shared_from_this<LineEditModel>
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& = "");

        LineEditModel();

    public:
        FTK_API virtual ~LineEditModel();

        //! Create a new model
        FTK_API static std::shared_ptr<LineEditModel> create(
            const std::shared_ptr<Context>&,
            const std::string& = "");

        //! \name Text
        ///@{

        FTK_API const std::string& getText() const;
        FTK_API std::shared_ptr<IObservable<std::string> > observeText() const;

        //! Set the text. Setting the text will also set the cursor to the
        //! beginning and clear the selection.
        FTK_API void setText(const std::string&);

        FTK_API void clearText();

        FTK_API bool isReadOnly() const;
        FTK_API std::shared_ptr<IObservable<bool> > observeReadOnly() const;
        FTK_API void setReadOnly(bool);

        ///@}

        //! \name Cursor
        ///@{

        FTK_API int getCursor() const;
        FTK_API std::shared_ptr<IObservable<int> > observeCursor() const;

        //! Set the cursor. Setting the cursor clears the selection.
        FTK_API void setCursor(int);

        ///@}

        //! \name Selection
        ///@{

        FTK_API const LineEditSelection& getSelection() const;
        FTK_API std::shared_ptr<IObservable<LineEditSelection> > observeSelection() const;
        FTK_API void setSelection(const LineEditSelection&);
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

    private:
        void _move(Key, int modifiers);
        void _backspace();
        void _delete();

        void _replace(
            const LineEditSelection&,
            const std::string&);

        FTK_PRIVATE();
    };
        
    ///@}
}
