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

        //! Add an action that the widget added last stands for, rather
        //! than a button of its own: when the widget does not fit, the
        //! action is listed in its place (see ToolBarRow).
        FTK_UI_API void addOverflowAction(const std::shared_ptr<Action>&);

        //! Get the actions, as buttons and as stood for by widgets, in the
        //! order they were added.
        FTK_UI_API const std::vector<std::shared_ptr<Action> >& getActions() const;

        //! \name Overflow
        //! For a row that is short of room (ToolBarRow): the widgets at
        //! the end of the tool bar are hidden, one at a time, and their
        //! actions are listed instead. A divider left at the end goes
        //! with what followed it, and is not counted. A widget hidden by
        //! anyone else is left out of the count.
        ///@{

        //! Get how many widgets can be hidden: all of them but dividers.
        FTK_UI_API size_t getOverflowMax() const;

        //! Get the width of the tool bar with the given number hidden.
        FTK_UI_API int getOverflowWidth(size_t) const;

        //! Get how many are hidden.
        FTK_UI_API size_t getOverflow() const;

        //! Hide the given number from the end.
        FTK_UI_API void setOverflow(size_t);

        //! Get the actions of what is hidden, in order.
        FTK_UI_API std::vector<std::shared_ptr<Action> > getOverflowActions() const;

        ///@}

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

        //! Get whether the buttons are grouped.
        FTK_UI_API bool isGrouped() const;

        //! Set whether the buttons are grouped. A run of widgets that are
        //! segments, IWidget::isSegment(), is drawn as one control: they
        //! touch and have a background, and only the corners at the ends of
        //! the run are rounded. Any other widget ends a run.
        FTK_UI_API void setGrouped(bool);

        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;


    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
