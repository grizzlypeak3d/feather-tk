// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{
        
    //! Grid layout.
    class FTK_API_TYPE GridLayout : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        GridLayout();

    public:
        FTK_API virtual ~GridLayout();

        //! Create a new layout.
        FTK_API static std::shared_ptr<GridLayout> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the grid position of the given child.
        std::pair<int, int> getGridPos(const std::shared_ptr<IWidget>&) const;

        //! Get the grid row of the given child.
        int getGridRow(const std::shared_ptr<IWidget>&) const;

        //! Get the grid column of the given child.
        int getGridColumn(const std::shared_ptr<IWidget>&) const;

        //! Set a child position within the grid.
        FTK_API void setGridPos(
            const std::shared_ptr<IWidget>& child,
            int row,
            int column);

        //! Get the odd rows background role.
        FTK_API ColorRole getRowBackgroundRole(ColorRole) const;

        //! Get the odd rows background role.
        FTK_API void setRowBackgroundRole(ColorRole);

        //! Get the margin role.
        FTK_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_API void setMarginRole(SizeRole);

        //! Get the spacing role.
        FTK_API SizeRole getSpacingRole() const;

        //! Set the spacing role.
        FTK_API void setSpacingRole(SizeRole);

        //! Remove all children from the layout.
        FTK_API void clear();

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void childRemoveEvent(const ChildRemoveEvent&) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
