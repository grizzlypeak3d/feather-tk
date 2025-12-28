// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    class Action;
    class ToolButton;

    //! \name Composite Widgets
    ///@{
        
    //! Tool bar.
    class FTK_API_TYPE ToolBar : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::shared_ptr<IWidget>& parent);

        ToolBar();

    public:
        FTK_API virtual ~ToolBar();

        //! Create a new widget.
        FTK_API static std::shared_ptr<ToolBar> create(
            const std::shared_ptr<Context>&,
            Orientation = Orientation::Horizontal,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Add an action.
        FTK_API std::shared_ptr<ToolButton> addAction(const std::shared_ptr<Action>&);

        //! Add a widget.
        FTK_API void addWidget(const std::shared_ptr<IWidget>&);

        //! Clear the tool bar.
        FTK_API void clear();

        //! Get the margin role.
        FTK_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_API void setMarginRole(SizeRole);

        //! Get the spacing role.
        FTK_API SizeRole getSpacingRole() const;

        //! Set the spacing role.
        FTK_API void setSpacingRole(SizeRole);

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
