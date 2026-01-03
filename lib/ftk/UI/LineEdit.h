// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    class LineEditModel;

    //! \name Text Widgets
    ///@{
        
    //! Text line edit.
    //! 
    //! \todo Double click to select text.
    class FTK_API_TYPE LineEdit : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<LineEditModel>&,
            const std::shared_ptr<IWidget>& parent);

        LineEdit();

    public:
        FTK_API virtual ~LineEdit();

        //! Create a new widget
        FTK_API static std::shared_ptr<LineEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget
        FTK_API static std::shared_ptr<LineEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<LineEditModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the model.
        FTK_API const std::shared_ptr<LineEditModel>& getModel() const;

        //! Get the text.
        FTK_API const std::string& getText() const;

        //! Set the text.
        FTK_API void setText(const std::string&);

        //! Clear the text.
        FTK_API void clearText();

        //! Set the text callback. The text callback is called when the enter
        //! key is clicked, and optionally when the text focus is lost.
        FTK_API void setTextCallback(const std::function<void(const std::string&)>&);

        //! Get whether the text callback is called when focus is lost.
        FTK_API bool hasTextCallbackOnFocusLost() const;

        //! Set whether the text callback is called when focus is lost.
        FTK_API void setTextCallbackOnFocusLost(bool);

        //! Set the text changed callback.
        FTK_API void setTextChangedCallback(const std::function<void(const std::string&)>&);

        //! Get the formatting text.
        FTK_API const std::string& getFormat() const;

        //! Set the formatting text.
        FTK_API void setFormat(const std::string&);

        //! Set the focus callback.
        FTK_API void setFocusCallback(const std::function<void(bool)>&);

        //! Select all.
        FTK_API void selectAll();

        //! Clear the selection.
        FTK_API void clearSelection();

        //! Get the font role.
        FTK_API FontRole getFontRole() const;

        //! Set the font role.
        FTK_API void setFontRole(FontRole);

        //! Get the border role.
        FTK_API ColorRole getBorderRole() const;

        //! Set the border role.
        FTK_API void setBorderRole(ColorRole);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void setVisible(bool) override;
        FTK_API void setEnabled(bool) override;
        FTK_API void tickEvent(
            bool,
            bool,
            const TickEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void keyFocusEvent(bool) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;
        FTK_API void textEvent(TextEvent&) override;

    private:
        int _toCursor(int) const;
        int _toPos(int) const;

        Box2I _getBorderBox() const;
        Box2I _getBackgroundBox() const;
        Box2I _getTextBox() const;
        void _scrollUpdate(int);

        FTK_PRIVATE();
    };
        
    ///@}
}
