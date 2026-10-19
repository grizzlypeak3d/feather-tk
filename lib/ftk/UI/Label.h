// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IWidget.h>

#include <ftk/Core/String.h>

namespace ftk
{
    //! \name Text Widgets
    ///@{
        
    //! Text label.
    class FTK_UI_API_TYPE Label : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        Label();

    public:
        FTK_UI_API virtual ~Label();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<Label> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<Label> create(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the text.
        FTK_UI_API const std::string& getText() const;

        //! Set the text.
        FTK_UI_API void setText(const std::string&);

        //! Get the text color role.
        FTK_UI_API ColorRole getTextRole() const;

        //! Set the text color role.
        FTK_UI_API void setTextRole(ColorRole);

        //! Get the margin role. If the horizontal and vertical margin roles
        //! are different the horizontal role is returned.
        FTK_UI_API SizeRole getMarginRole() const;

        //! Get the horizontal margin role.
        FTK_UI_API SizeRole getHMarginRole() const;

        //! Get the vertical margin role.
        FTK_UI_API SizeRole getVMarginRole() const;

        //! Set the margin role.
        FTK_UI_API void setMarginRole(SizeRole);

        //! Set the margin roles.
        FTK_UI_API void setMarginRole(SizeRole horizontal, SizeRole vertical);

        //! Set the horizontal margin role.
        FTK_UI_API void setHMarginRole(SizeRole);

        //! Set the vertical margin role.
        FTK_UI_API void setVMarginRole(SizeRole);

        //! Get the font.
        FTK_UI_API FontType getFont() const;

        //! Set the font.
        FTK_UI_API void setFont(FontType);

        //! Get the font size.
        FTK_UI_API int getFontSize() const;

        //! Set the font size.
        FTK_UI_API void setFontSize(int);

        //! Get the alignment of the text within the label.
        FTK_UI_API HAlign getTextAlign() const;

        //! Set the alignment of the text within the label, for a label
        //! given more room than its text needs. This is separate from the
        //! widget's own alignment, which is where a layout puts the label.
        FTK_UI_API void setTextAlign(HAlign);

        //! Get whether the text is clipped.
        FTK_UI_API bool getClipText() const;

        //! Set whether the text is clipped.
        FTK_UI_API void setClipText(bool);

        //! Get whether the text is elided to fit the width.
        FTK_UI_API bool getElide() const;

        //! Get which end of elided text is kept.
        FTK_UI_API ElideMode getElideMode() const;

        //! Set whether the text is elided to fit the width the label is
        //! given, and which end of it to keep. The size hint is then only as
        //! wide as the ellipsis, so a long text does not widen whatever holds
        //! the label; give the label an expanding stretch for it to use the
        //! room there is. Turning it on sets the horizontal alignment to
        //! fill, since a left aligned widget is laid out at its size hint,
        //! and turning it off sets it back to left. The text is measured in
        //! its font rather than counted in characters, and a character is
        //! never split.
        FTK_UI_API void setElide(bool, ElideMode = ElideMode::Right);

        //! Get the width the text is wrapped to, in characters.
        FTK_UI_API int getWrap() const;

        //! Set a width to wrap the text to, in characters; zero, the
        //! default, does not wrap. Lines are broken between words where
        //! they would pass the width, which is that many characters of
        //! average width in the label's font, so a paragraph can be written
        //! as one string and keeps its shape in another font or at another
        //! display scale. A line break in the text is kept, and a word
        //! longer than the width is left whole. The label's size is the
        //! size of the wrapped text. Not for use with eliding.
        FTK_UI_API void setWrap(int);

        //! Get whether the text is wrapped to the width the label is given.
        FTK_UI_API bool getWrapToFit() const;

        //! Set whether the text is wrapped to the width the label is given
        //! rather than to the wrap width, which is then only the width the
        //! label asks for: a paragraph fills the room a layout gives it, and
        //! still fits when a small window gives it less. The height follows
        //! the width, so the label asks to be measured again when the width
        //! changes. Turning it on sets the horizontal alignment to fill, as
        //! eliding does, and turning it off sets it back to left. Needs a
        //! wrap width.
        FTK_UI_API void setWrapToFit(bool);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void clipEvent(const Box2I&, bool) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };

    //! The width of that many characters of ordinary text in a font: what
    //! a label's wrap width is, and how wide a dialog sized for text is.
    //! Characters as wide as they come in a sentence, spaces included,
    //! rather than any one letter, so the width follows the font and the
    //! display scale.
    FTK_UI_API int getCharsWidth(
        const std::shared_ptr<FontSystem>&,
        const FontInfo&,
        int chars);
        
    ///@}
}
