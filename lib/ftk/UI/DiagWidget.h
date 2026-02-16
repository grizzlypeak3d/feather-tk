// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    class DiagModel;

    //! \name Debugging
    ///@{
        
    //! Diagnostics widget.
    class FTK_API_TYPE DiagWidget : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DiagModel>&,
            const std::shared_ptr<IWidget>& parent);

        DiagWidget();

    public:
        FTK_API virtual ~DiagWidget();

        //! Create a new widget.
        FTK_API static std::shared_ptr<DiagWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<DiagModel>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the margin role.
        FTK_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_API void setMarginRole(SizeRole);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
