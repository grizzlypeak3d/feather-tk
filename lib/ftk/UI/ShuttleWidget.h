// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the DJV project.

#pragma once

#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! Shuttle widget.
    class FTK_API_TYPE ShuttleWidget : public IMouseWidget
    {
        FTK_NON_COPYABLE(ShuttleWidget);

    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        ShuttleWidget();

    public:
        FTK_API ~ShuttleWidget();

        FTK_API static std::shared_ptr<ShuttleWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        FTK_API void setCallback(const std::function<void(int)>&);
        FTK_API void setDeltaCallback(const std::function<void(int)>&);
        FTK_API void setActiveCallback(const std::function<void(bool)>&);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void styleEvent(const StyleEvent&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_API void mouseLeaveEvent() override;
        FTK_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void mouseReleaseEvent(MouseClickEvent&) override;

    private:
        FTK_PRIVATE();
    };
}