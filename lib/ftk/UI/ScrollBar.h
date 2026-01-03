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
        FTK_API virtual ~ScrollBar();

        //! Create a new widget.
        FTK_API static std::shared_ptr<ScrollBar> create(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Scroll Size
        ///@{

        FTK_API int getScrollSize() const;
        FTK_API void setScrollSize(int);
        
        ///@}

        //! \name Scroll Position
        ///@{

        FTK_API int getScrollPos() const;
        FTK_API void setScrollPos(int);
        FTK_API void setScrollPosCallback(const std::function<void(int)>&);
        
        ///@}

        FTK_API Size2I getSizeHint() const override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void mouseEnterEvent(MouseEnterEvent&) override;
        FTK_API void mouseLeaveEvent() override;
        FTK_API void mouseMoveEvent(MouseMoveEvent&) override;
        FTK_API void mousePressEvent(MouseClickEvent&) override;
        FTK_API void mouseReleaseEvent(MouseClickEvent&) override;

    private:
        Box2I _getHandleGeometry() const;
        int _getScrollPosMax() const;
        float _getScrollScale() const;

        FTK_PRIVATE();
    };

    ///@}
}
