// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! \name Text Widgets
    ///@{
        
    //! Text line edit.
    //! 
    //! \todo Scroll the view with the cursor.
    //! \todo Double click to select text.
    class FTK_API_TYPE LineEdit : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        LineEdit();

    public:
        virtual ~LineEdit();

        //! Create a new widget
        static std::shared_ptr<LineEdit> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the text.
        const std::string& getText() const;

        //! Set the text.
        void setText(const std::string&);

        //! Clear the text.
        void clearText();

        //! Set the text callback.
        void setTextCallback(const std::function<void(const std::string&)>&);

        //! Set the text changed callback.
        void setTextChangedCallback(const std::function<void(const std::string&)>&);

        //! Get the formatting text.
        const std::string& getFormat() const;

        //! Set the formatting text.
        void setFormat(const std::string&);

        //! Set the focus callback.
        void setFocusCallback(const std::function<void(bool)>&);

        //! Select all.
        void selectAll();

        //! Clear the selection.
        void selectNone();

        //! Get the font role.
        FontRole getFontRole() const;

        //! Set the font role.
        void setFontRole(FontRole);

        //! Get the border role.
        ColorRole getBorderRole() const;

        //! Set the border role.
        void setBorderRole(ColorRole);

        void setGeometry(const Box2I&) override;
        void setVisible(bool) override;
        void setEnabled(bool) override;
        void tickEvent(
            bool,
            bool,
            const TickEvent&) override;
        void clipEvent(const Box2I&, bool) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;
        void mouseMoveEvent(MouseMoveEvent&) override;
        void mousePressEvent(MouseClickEvent&) override;
        void keyFocusEvent(bool) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;
        void textEvent(TextEvent&) override;

    private:
        int _getCursorPos(const V2I&) const;

        void _textUpdate();

        FTK_PRIVATE();
    };
        
    ///@}
}
