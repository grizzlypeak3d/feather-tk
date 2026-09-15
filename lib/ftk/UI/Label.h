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
    //! 
    //! \todo Add text wrapping.
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

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void clipEvent(const Box2I&, bool) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
