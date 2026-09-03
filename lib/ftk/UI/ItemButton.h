// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IButton.h>

namespace ftk
{
    //! \name Buttons
    ///@{

    //! A button that draws the list item states around a widget, for
    //! list rows built from more than a line of text.
    class FTK_UI_API_TYPE ItemButton : public IButton
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        ItemButton();

    public:
        FTK_UI_API virtual ~ItemButton();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ItemButton> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the widget.
        FTK_UI_API const std::shared_ptr<IWidget>& getWidget() const;

        //! Set the widget.
        FTK_UI_API void setWidget(const std::shared_ptr<IWidget>&);

        //! Set a callback for when the item gains or loses the key focus.
        FTK_UI_API void setFocusCallback(const std::function<void(bool)>&);

        //! Set the drag and drop data the item offers. Setting it makes
        //! the item draggable: a press that moves past the drag length
        //! starts a drag instead of a click.
        FTK_UI_API void setDragDropDataCallback(
            const std::function<std::shared_ptr<IDragDropData>(void)>&);

        //! Set the image shown under the cursor while the item is
        //! dragged.
        FTK_UI_API void setDragDropCursorCallback(
            const std::function<std::shared_ptr<Image>(void)>&);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_UI_API void mousePressEvent(MouseClickEvent&) override;
        FTK_UI_API void mouseReleaseEvent(MouseClickEvent&) override;
        FTK_UI_API void keyFocusEvent(bool) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
