// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{
        
    //! Grid layout.
    class FTK_UI_API_TYPE GridLayout : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        GridLayout();

    public:
        FTK_UI_API virtual ~GridLayout();

        //! Create a new layout.
        FTK_UI_API static std::shared_ptr<GridLayout> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the grid position of the given child.
        FTK_UI_API std::pair<int, int> getGridPos(const std::shared_ptr<IWidget>&) const;

        //! Get the grid row of the given child.
        FTK_UI_API int getGridRow(const std::shared_ptr<IWidget>&) const;

        //! Get the grid column of the given child.
        FTK_UI_API int getGridColumn(const std::shared_ptr<IWidget>&) const;

        //! Set a child position within the grid.
        FTK_UI_API void setGridPos(
            const std::shared_ptr<IWidget>& child,
            int row,
            int column);

        //! Get the odd rows background role.
        FTK_UI_API ColorRole getRowBackgroundRole() const;

        //! Get the odd rows background role.
        FTK_UI_API void setRowBackgroundRole(ColorRole);

        //! Get the margin role.
        FTK_UI_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_UI_API void setMarginRole(SizeRole);

        //! Get the spacing role.
        FTK_UI_API SizeRole getSpacingRole() const;

        //! Set the spacing role.
        FTK_UI_API void setSpacingRole(SizeRole);

        //! Set the spacing role for each direction. The horizontal spacing
        //! is between columns and the vertical spacing is between rows.
        FTK_UI_API void setSpacingRole(SizeRole horizontal, SizeRole vertical);

        //! Remove all children from the layout.
        FTK_UI_API void clear();

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void childRemoveEvent(const ChildRemoveEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
