// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ItemButton.h>
#include <ftk/UI/RowLayout.h>

namespace ftk
{
    //! \name Buttons
    ///@{

    //! A list of item buttons with one focus and one current item.
    //!
    //! The list is the focus unit, the file browser's model applied to
    //! rows that are widgets: the rows do not take the key focus
    //! themselves, the arrows move the current item, Return activates it,
    //! and Delete removes it. The current item outlives the focus leaving
    //! the list, so a control that acts on it always has a target.
    //!
    //! The rows are the ItemButton children, in order; other children --
    //! dividers, an empty-list label -- are ignored. The caller adds and
    //! removes children the way it would with a layout, and sets the
    //! current item again after rebuilding them.
    class FTK_UI_API_TYPE ItemButtonList : public VerticalLayout
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        ItemButtonList();

    public:
        FTK_UI_API virtual ~ItemButtonList();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ItemButtonList> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the current item, or -1.
        FTK_UI_API int getCurrent() const;

        //! Set the current item. The callback is not called: this is for
        //! restoring the current item, e.g. after rebuilding the rows.
        FTK_UI_API void setCurrent(int);

        //! Set the callback for the current item changing from inside the
        //! list -- the arrow keys browsing it.
        FTK_UI_API void setCurrentCallback(const std::function<void(int)>&);

        //! Set the callback for Return on the current item.
        FTK_UI_API void setActivateCallback(const std::function<void(int)>&);

        //! Set the callback for Delete on the current item.
        FTK_UI_API void setDeleteCallback(const std::function<void(int)>&);

        //! Get the item buttons, in order.
        FTK_UI_API std::vector<std::shared_ptr<ItemButton> > getItems() const;

        FTK_UI_API void drawOverlayEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _setCurrent(int, bool callback);

        //! A press on a row: the keyboard follows the click, entering the
        //! list at the clicked row. Called by ItemButton for rows that do
        //! not take the key focus themselves.
        void _rowPress(const std::shared_ptr<ItemButton>&);

        friend class ItemButton;

        FTK_PRIVATE();
    };

    ///@}
}
