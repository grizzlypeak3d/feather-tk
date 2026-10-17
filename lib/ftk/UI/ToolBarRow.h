// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    class ToolBar;

    //! \name Layouts
    ///@{

    //! A row of tool bars, with a divider between each and the next, that
    //! fits whatever width it is given. When the tool bars do not fit, whole
    //! tool bars leave the row from its end, right to left, and a button at
    //! the end opens a menu of their actions. A tool bar is never cut in
    //! two: a button alone at the edge of a window is a button nobody can
    //! find. Nor does one leave from the middle, which read as buttons
    //! vanishing at random; the row shrinks from its end, as text wraps.
    //! The last tool bar can be kept beside the button regardless, for the
    //! one a window cannot do without.
    //!
    //! A tool bar that is hidden is not in the row at all, so hiding one
    //! leaves room for the others rather than a gap. Widgets that are not
    //! actions, an edit or a slider, have no place in a menu; a tool bar
    //! names the actions that stand for them with
    //! ToolBar::addOverflowAction().
    class FTK_UI_API_TYPE ToolBarRow : public IWidget
    {
    protected:
        FTK_UI_API void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        FTK_UI_API ToolBarRow();

    public:
        FTK_UI_API virtual ~ToolBarRow();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ToolBarRow> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Add a tool bar at the end of the row.
        FTK_UI_API void addToolBar(const std::shared_ptr<ToolBar>&);

        //! Get whether the last tool bar stays when the others do not fit.
        FTK_UI_API bool isKeepLast() const;

        //! Set whether the last tool bar stays when the others do not fit.
        //! The others leave right to left before it, and it goes only when
        //! it does not fit on its own.
        FTK_UI_API void setKeepLast(bool);

        //! Get the tool bars that did not fit, left to right.
        FTK_UI_API std::vector<std::shared_ptr<ToolBar> > getOverflow() const;

        //! Get the margin role.
        FTK_UI_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_UI_API void setMarginRole(SizeRole);

        //! Get the spacing role.
        FTK_UI_API SizeRole getSpacingRole() const;

        //! Set the spacing role.
        FTK_UI_API void setSpacingRole(SizeRole);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void childRemoveEvent(const ChildRemoveEvent&) override;

    private:
        void _openMenu();

        FTK_PRIVATE();
    };

    ///@}
}
