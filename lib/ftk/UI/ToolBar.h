// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IContainer.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    class Action;
    class ToolButton;

    //! \name Composite Widgets
    ///@{
        
    //! Tool bar.
    class FTK_UI_API_TYPE ToolBar : public IContainer
    {
    protected:
        FTK_UI_API void _init(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::shared_ptr<IWidget>& parent);

        FTK_UI_API ToolBar();

    public:
        FTK_UI_API virtual ~ToolBar();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ToolBar> create(
            const std::shared_ptr<Context>&,
            Orientation = Orientation::Horizontal,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Add an action.
        FTK_UI_API std::shared_ptr<ToolButton> addAction(const std::shared_ptr<Action>&);

        //! Add a widget.
        FTK_UI_API void addWidget(const std::shared_ptr<IWidget>&);

        //! Clear the tool bar.
        FTK_UI_API void clear();

        //! Get the margin role.
        FTK_UI_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_UI_API void setMarginRole(SizeRole);

        //! Get the spacing role.
        FTK_UI_API SizeRole getSpacingRole() const;

        //! Set the spacing role.
        FTK_UI_API void setSpacingRole(SizeRole);


    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
