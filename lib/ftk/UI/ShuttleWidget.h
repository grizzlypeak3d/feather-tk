// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the DJV project.

#pragma once

#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! Shuttle widget.
    class ShuttleWidget : public IMouseWidget
    {
        FTK_NON_COPYABLE(ShuttleWidget);

    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        ShuttleWidget();

    public:
        ~ShuttleWidget();

        static std::shared_ptr<ShuttleWidget> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        void setActiveCallback(const std::function<void(bool)>&);
        void setCallback(const std::function<void(int)>&);

        Size2I getSizeHint() const override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;
        void mouseEnterEvent(MouseEnterEvent&) override;
        void mouseLeaveEvent() override;
        void mouseMoveEvent(MouseMoveEvent&) override;
        void mousePressEvent(MouseClickEvent&) override;
        void mouseReleaseEvent(MouseClickEvent&) override;

    private:
        FTK_PRIVATE();
    };
}