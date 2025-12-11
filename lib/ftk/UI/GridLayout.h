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
        virtual ~GridLayout();

        //! Create a new layout.
        static std::shared_ptr<GridLayout> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set a child position within the grid.
        void setGridPos(
            const std::shared_ptr<IWidget>& child,
            int row,
            int column);

        //! Get the odd rows background role.
        ColorRole getRowBackgroundRole(ColorRole) const;

        //! Get the odd rows background role.
        void setRowBackgroundRole(ColorRole);

        //! Get the margin role.
        SizeRole getMarginRole() const;

        //! Set the margin role.
        void setMarginRole(SizeRole);

        //! Get the spacing role.
        SizeRole getSpacingRole() const;

        //! Set the spacing role.
        void setSpacingRole(SizeRole);

        //! Remove all children from the layout.
        void clear();

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void childRemoveEvent(const ChildRemoveEvent&) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
