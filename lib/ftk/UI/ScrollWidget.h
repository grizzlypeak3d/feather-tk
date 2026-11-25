// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/ScrollArea.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Scroll widget information.
    struct FTK_UI_API_TYPE ScrollInfo
    {
        //! The geometry of the area the widget is seen through, in window
        //! coordinates.
        Box2I  viewport;
        //! The size of the widget being scrolled, in pixels.
        Size2I scrollSize;
        //! The scroll position: how far the widget is scrolled, in pixels.
        V2I    scrollPos;

        bool operator == (const ScrollInfo&) const = default;
    };

    //! Scroll widget.
    class FTK_UI_API_TYPE ScrollWidget : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            ScrollType,
            const std::shared_ptr<IWidget>& parent);

        ScrollWidget();

    public:
        FTK_UI_API virtual ~ScrollWidget();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ScrollWidget> create(
            const std::shared_ptr<Context>&,
            ScrollType = ScrollType::Both,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Scroll Widget
        ///@{

        //! Get the widget that is scrolled.
        FTK_UI_API const std::shared_ptr<IWidget>& getWidget() const;
        FTK_UI_API void setWidget(const std::shared_ptr<IWidget>&);
        
        ///@}

        //! \name Scroll Type
        ///@{

        //! Get which ways the widget scrolls.
        FTK_UI_API ScrollType getScrollType() const;
        FTK_UI_API void setScrollType(ScrollType);
        
        ///@}

        //! \name Scroll Size
        ///@{

        //! Get the size of the widget being scrolled, in pixels.
        FTK_UI_API const Size2I& getScrollSize() const;
        //! Set a callback for when the scroll size changes.
        FTK_UI_API void setScrollSizeCallback(const std::function<void(const Size2I&)>&);
        
        ///@}

        //! \name Scroll Position
        ///@{

        //! Get the scroll position, in pixels.
        FTK_UI_API const V2I& getScrollPos() const;
        //! Set the scroll position. With clamp it is kept between zero and the
        //! scroll size less the size of the view.
        FTK_UI_API void setScrollPos(const V2I&, bool clamp = true);
        //! Set a callback for when the scroll position changes.
        FTK_UI_API void setScrollPosCallback(const std::function<void(const V2I&)>&);

        //! Scroll to make the given box visible.
        FTK_UI_API void scrollTo(const Box2I&);

        ///@}

        //! \name Scroll Area
        ///@{

        FTK_UI_API const std::shared_ptr<ScrollArea>& getScrollArea() const;

        //! Get whether the scrolled area is resizable.
        FTK_UI_API bool isAreaResizable() const;

        //! Get whether the scrolled area is resizable.
        FTK_UI_API void setAreaResizable(bool);

        ///@}

        //! \name Scroll Information
        ///@{

        //! Get the scroll information.
        FTK_UI_API ScrollInfo getScrollInfo() const;

        //! Set the scroll information callback.
        FTK_UI_API void setScrollInfoCallback(
            const std::function<void(const ScrollInfo&)>&);

        ///@}

        //! \name Options
        ///@{

        //! Get whether the scroll bars are visible.
        FTK_UI_API bool areScrollBarsVisible() const;

        //! Set whether the scroll bars are visible.
        FTK_UI_API void setScrollBarsVisible(bool);

        //! Get whether the scroll bars auto-hide.
        FTK_UI_API bool getScrollBarsAutoHide() const;

        //! Set whether the scroll bars auto-hide.
        FTK_UI_API void setScrollBarsAutoHide(bool);

        //! Get whether scroll events are enabled.
        FTK_UI_API bool areScrollEventsEnabled() const;

        //! Set whether scroll events are enabled.
        FTK_UI_API void setScrollEventsEnabled(bool);

        //! Get whether the widget scrolls when a drag and drop hovers
        //! near its edges.
        FTK_UI_API bool hasDragScroll() const;

        //! Set whether the widget scrolls when a drag and drop hovers
        //! near its edges. The scrolling carries on while the drag is just
        //! outside the widget, so a drop target that is off the screen can
        //! be reached by holding the drag past the edge of the list.
        FTK_UI_API void setDragScroll(bool);

        //! Get the line step.
        FTK_UI_API int getLineStep() const;

        //! Set the line step.
        FTK_UI_API void setLineStep(int);

        //! Get whether the scroll area has a border.
        FTK_UI_API bool hasBorder() const;

        //! Set whether the scroll area has a border.
        FTK_UI_API void setBorder(bool);

        //! Get the size hint role.
        FTK_UI_API SizeRole getSizeHintRole() const;

        //! Set the size hint role, which is the size the contents are given
        //! to scroll in. "None" asks for the height of the contents
        //! instead, so that the widget takes the room it needs and does not
        //! scroll vertically; the width is unaffected.
        FTK_UI_API void setSizeHintRole(SizeRole);

        //! Get the margin role.
        FTK_UI_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_UI_API void setMarginRole(SizeRole);

        ///@}

        //! \name Viewport
        ///@{

        //! Get the viewport widget.
        FTK_UI_API const std::shared_ptr<IWidget>& getViewportWidget() const;

        //! Set the viewport widget.
        FTK_UI_API void setViewportWidget(const std::shared_ptr<IWidget>&);

        ///@}

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void tickEvent(bool, bool, const TickEvent&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_UI_API void scrollEvent(ScrollEvent&) override;
        FTK_UI_API void gestureEvent(GestureEvent&) override;
        FTK_UI_API void keyPressEvent(KeyEvent&) override;
        FTK_UI_API void keyReleaseEvent(KeyEvent&) override;

    private:
        int _getPageStep() const;

        void _scrollBarsUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
