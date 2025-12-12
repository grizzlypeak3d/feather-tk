// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! Base class for widgets with simple mouse interactions.
    class FTK_API_TYPE IMouseWidget : public IWidget
    {
        FTK_NON_COPYABLE(IMouseWidget);

    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& objectName,
            const std::shared_ptr<IWidget>& parent);

        IMouseWidget() = default;

    public:
        FTK_API virtual ~IMouseWidget() = 0;

        FTK_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_API void mouseLeaveEvent() override;
        FTK_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void mouseReleaseEvent(MouseClickEvent&) override;

    protected:
        void _setMouseHoverEnabled(bool);
        void _setMousePressEnabled(bool, int button = 0, int modifiers = 0);

        bool _isMouseInside() const;
        const V2I& _getMousePos() const;
        bool _isMousePressed() const;
        const V2I& _getMousePressPos() const;

    private:
        bool _mouseHoverEnabled = false;
        bool _mousePressEnabled = false;
        int _mousePressButton = 0;
        int _mousePressModifiers = 0;
        bool _mouseInside = false;
        V2I _mousePos;
        bool _mousePress = false;
        V2I _mousePressPos;
    };
}


