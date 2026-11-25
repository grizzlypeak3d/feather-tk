// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the DJV project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IMouseWidget.h>


namespace ftk
{
    //! Shuttle widget.
    class FTK_UI_API_TYPE ShuttleWidget : public IMouseWidget
    {
        FTK_NON_COPYABLE(ShuttleWidget);

    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        ShuttleWidget();

    public:
        FTK_UI_API ~ShuttleWidget();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ShuttleWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the keyboard modifiers held during the drag. Read from
        //! within a delta callback by anything that scales what a notch is
        //! worth.
        FTK_UI_API int getModifiers() const;

        //! Set the callback, called while the shuttle is dragged with the
        //! number of notches it has moved since it was pressed, negative to the
        //! left. A notch is a quarter of the widget's height.
        FTK_UI_API void setCallback(const std::function<void(int)>&);
        //! Set the delta callback, called while the shuttle is dragged with the
        //! number of notches it has moved since the last call.
        FTK_UI_API void setDeltaCallback(const std::function<void(int)>&);
        //! Set the active callback, called with true when the shuttle is
        //! pressed and false when it is released.
        FTK_UI_API void setActiveCallback(const std::function<void(bool)>&);



        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_UI_API void mouseLeaveEvent() override;
        FTK_UI_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_UI_API void mousePressEvent(MouseClickEvent&) override;
        FTK_UI_API void mouseReleaseEvent(MouseClickEvent&) override;

    private:
        FTK_PRIVATE();
    };
}