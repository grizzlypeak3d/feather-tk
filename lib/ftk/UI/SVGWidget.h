// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

#include <ftk/Core/Image.h>

namespace ftk
{
    //! \name Images
    ///@{
        
    //! SVG widget.
    class FTK_API_TYPE SvgWidget : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        SvgWidget();

    public:
        FTK_API virtual ~SvgWidget();

        //! Create a new widget.
        FTK_API static std::shared_ptr<SvgWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the SVG data.
        FTK_API const std::vector<uint8_t>& getSVGData() const;

        //! Set the SVG data.
        FTK_API void setSVGData(const std::vector<uint8_t>&);

        //! Get the SVG width.
        FTK_API int getSVGWidth() const;

        //! Set the SVG width.
        FTK_API void setSVGWidth(int);

        //! Get the margin role.
        FTK_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_API void setMarginRole(SizeRole);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
