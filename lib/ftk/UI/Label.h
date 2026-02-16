// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Text Widgets
    ///@{
        
    //! Text label.
    //! 
    //! \todo Add text wrapping.
    class FTK_API_TYPE Label : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        Label();

    public:
        FTK_API virtual ~Label();

        //! Create a new widget.
        FTK_API static std::shared_ptr<Label> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<Label> create(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the text.
        FTK_API const std::string& getText() const;

        //! Set the text.
        FTK_API void setText(const std::string&);

        //! Get the text color role.
        FTK_API ColorRole getTextRole() const;

        //! Set the text color role.
        FTK_API void setTextRole(ColorRole);

        //! Get the margin role. If the horizontal and vertical margin roles
        //! are different the horizontal role is returned.
        FTK_API SizeRole getMarginRole() const;

        //! Get the horizontal margin role.
        FTK_API SizeRole getHMarginRole() const;

        //! Get the vertical margin role.
        FTK_API SizeRole getVMarginRole() const;

        //! Set the margin role.
        FTK_API void setMarginRole(SizeRole);

        //! Set the margin roles.
        FTK_API void setMarginRole(SizeRole horizontal, SizeRole vertical);

        //! Set the horizontal margin role.
        FTK_API void setHMarginRole(SizeRole);

        //! Set the vertical margin role.
        FTK_API void setVMarginRole(SizeRole);

        //! Get the font role.
        FTK_API FontRole getFontRole() const;

        //! Set the font role.
        FTK_API void setFontRole(FontRole);

        //! Get the font information.
        FTK_API const FontInfo& getFontInfo() const;

        //! Set the font information.
        FTK_API void setFontInfo(const FontInfo&);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void clipEvent(const Box2I&, bool) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
