// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Spacer widget.
    class FTK_API_TYPE Spacer : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::shared_ptr<IWidget>& parent);

        Spacer();

    public:
        FTK_API virtual ~Spacer();

        //! Create a new widget.
        FTK_API static std::shared_ptr<Spacer> create(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the spacing role.
        FTK_API SizeRole getSpacingRole() const;

        //! Set the spacing role.
        FTK_API void setSpacingRole(SizeRole);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
