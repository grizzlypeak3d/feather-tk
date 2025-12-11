// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Scroll bar.
    class FTK_API_TYPE ScrollBar : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::shared_ptr<IWidget>& parent);

        ScrollBar();

    public:
        virtual ~ScrollBar();

        //! Create a new widget.
        static std::shared_ptr<ScrollBar> create(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the scroll size.
        int getScrollSize() const;

        //! Set the scroll size.
        void setScrollSize(int);

        //! Get the scroll position.
        int getScrollPos() const;

        //! Set the scroll position.
        void setScrollPos(int);

        //! Set the scroll position callback.
        void setScrollPosCallback(const std::function<void(int)>&);

        void sizeHintEvent(const SizeHintEvent&) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;
        void mouseEnterEvent(MouseEnterEvent&) override;
        void mouseLeaveEvent() override;
        void mouseMoveEvent(MouseMoveEvent&) override;
        void mousePressEvent(MouseClickEvent&) override;
        void mouseReleaseEvent(MouseClickEvent&) override;

    private:
        Box2I _getHandleGeometry() const;
        int _getScrollPosMax() const;
        float _getScrollScale() const;

        FTK_PRIVATE();
    };

    ///@}
}
