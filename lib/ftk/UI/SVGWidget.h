// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IWidget.h>

#include <ftk/Core/Image.h>

namespace ftk
{
    //! \name Images
    ///@{
        
    //! SVG widget.
    class FTK_UI_API_TYPE SvgWidget : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        SvgWidget();

    public:
        FTK_UI_API virtual ~SvgWidget();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<SvgWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the SVG data.
        FTK_UI_API const std::vector<uint8_t>& getSVGData() const;

        //! Set the SVG data.
        FTK_UI_API void setSVGData(const std::vector<uint8_t>&);

        //! Get the SVG width.
        FTK_UI_API int getSVGWidth() const;

        //! Set the SVG width.
        FTK_UI_API void setSVGWidth(int);

        //! Get the margin role.
        FTK_UI_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_UI_API void setMarginRole(SizeRole);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
