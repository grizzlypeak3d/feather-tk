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
        virtual ~Label();

        //! Create a new widget.
        static std::shared_ptr<Label> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        static std::shared_ptr<Label> create(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the text.
        const std::string& getText() const;

        //! Set the text.
        void setText(const std::string&);

        //! Get the text color role.
        ColorRole getTextRole() const;

        //! Set the text color role.
        void setTextRole(ColorRole);

        //! Get the margin role. If the horizontal and vertical margin roles
        //! are different the horizontal role is returned.
        SizeRole getMarginRole() const;

        //! Get the horizontal margin role.
        SizeRole getHMarginRole() const;

        //! Get the vertical margin role.
        SizeRole getVMarginRole() const;

        //! Set the margin roles.
        void setMarginRole(SizeRole);

        //! Set the margin roles.
        void setMarginRole(SizeRole horizontal, SizeRole vertical);

        //! Set the horizontal margin role.
        void setHMarginRole(SizeRole);

        //! Set the vertical margin role.
        void setVMarginRole(SizeRole);

        //! Get the font role.
        FontRole getFontRole() const;

        //! Set the font role.
        void setFontRole(FontRole);

        //! Get the font information.
        const FontInfo& getFontInfo() const;

        //! Set the font information.
        void setFontInfo(const FontInfo&);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void clipEvent(const Box2I&, bool) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
