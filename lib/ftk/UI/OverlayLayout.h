// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Overlay layout.
    class FTK_API_TYPE OverlayLayout : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        OverlayLayout();

    public:
        FTK_API virtual ~OverlayLayout();

        //! Create a new layout.
        FTK_API static std::shared_ptr<OverlayLayout> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the margin role.
        FTK_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_API void setMarginRole(SizeRole);

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API Box2I getChildrenClipRect() const override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
