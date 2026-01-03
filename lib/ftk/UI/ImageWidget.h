// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

#include <ftk/Core/Image.h>

namespace ftk
{
    //! \name Images
    ///@{
        
    //! Image widget.
    class FTK_API_TYPE ImageWidget : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        ImageWidget();

    public:
        FTK_API virtual ~ImageWidget();

        //! Create a new widget.
        FTK_API static std::shared_ptr<ImageWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the image.
        FTK_API const std::shared_ptr<Image>& getImage() const;

        //! Set the image.
        FTK_API void setImage(const std::shared_ptr<Image>&);

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
