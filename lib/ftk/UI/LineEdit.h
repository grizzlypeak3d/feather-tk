// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    class LineEditModel;
    class Menu;

    //! \name Text Widgets
    ///@{
        
    //! Text line edit.
    //! 
    //! \todo Double click to select text.
    class FTK_UI_API_TYPE LineEdit : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<LineEditModel>&,
            const std::shared_ptr<IWidget>& parent);

        LineEdit();

    public:
        FTK_UI_API virtual ~LineEdit();

        //! Create a new widget
        FTK_UI_API static std::shared_ptr<LineEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget
        FTK_UI_API static std::shared_ptr<LineEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<LineEditModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the model.
        FTK_UI_API const std::shared_ptr<LineEditModel>& getModel() const;

        //! \name Text
        ///@{

        FTK_UI_API const std::string& getText() const;
        FTK_UI_API void setText(const std::string&);
        FTK_UI_API void clearText();

        //! Set the callback. The callback is triggered when the enter
        //! key is clicked and optionally when key focus is lost.
        FTK_UI_API void setCallback(const std::function<void(const std::string&)>&);

        //! Get whether the callback is triggered when focus is lost.
        FTK_UI_API bool hasCallbackOnFocusLost() const;

        //! Set whether the callback is triggered when focus is lost.
        FTK_UI_API void setCallbackOnFocusLost(bool);

        //! Set the text changed callback. This callback is triggered
        //! whenever the text is changed.
        FTK_UI_API void setTextChangedCallback(const std::function<void(const std::string&)>&);

        //! Get the formatting text.
        FTK_UI_API const std::string& getFormat() const;

        //! Set the formatting text.
        FTK_UI_API void setFormat(const std::string&);

        //! Set the focus callback.
        FTK_UI_API void setFocusCallback(const std::function<void(bool)>&);

        FTK_UI_API bool isReadOnly() const;
        FTK_UI_API void setReadOnly(bool);

        ///@}

        //! \name Selection
        ///@{

        FTK_UI_API void selectAll();
        FTK_UI_API void clearSelection();

        //! Get whether all of the text is selected when key focus is
        //! gained.
        FTK_UI_API bool hasSelectAllOnFocus() const;

        //! Set whether all of the text is selected when key focus is
        //! gained. Numeric edits use this so that typing replaces the
        //! value; a click can still place the cursor once the widget
        //! has focus.
        FTK_UI_API void setSelectAllOnFocus(bool);

        ///@}

        //! \name Font
        ///@{

        FTK_UI_API FontType getFont() const;
        FTK_UI_API void setFont(FontType);

        ///@}

        //! \name Border
        ///@{

        FTK_UI_API ColorRole getBorderRole() const;
        FTK_UI_API void setBorderRole(ColorRole);

        ///@}

        //! \name Well
        ///@{

        FTK_UI_API ColorRole getWellRole() const;
        FTK_UI_API void setWellRole(ColorRole);

        ///@}

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void setVisible(bool) override;
        FTK_UI_API void setEnabled(bool) override;
        FTK_UI_API void tickEvent(
            bool,
            bool,
            const TickEvent&) override;
        FTK_UI_API void clipEvent(const Box2I&, bool) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_UI_API void mousePressEvent(MouseClickEvent&) override;
        //! Get the in-progress input method composition. Empty except
        //! while text is being composed.
        FTK_UI_API const std::string& getPreedit() const;

        FTK_UI_API void keyFocusEvent(bool) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;
        FTK_UI_API void textEvent(TextEvent&) override;
        FTK_UI_API void textEditingEvent(TextEditingEvent&) override;

    private:
        void _textInputAreaUpdate();
        std::shared_ptr<Menu> _createContextMenu();

        Box2I _getAlignGeometry() const;
        Box2I _getMarginGeometry() const;
        Box2I _getTextGeometry() const;

        int _toCursor(int) const;
        int _toPos(int) const;

        void _scrollUpdate(int);

        FTK_PRIVATE();
    };
        
    ///@}
}
