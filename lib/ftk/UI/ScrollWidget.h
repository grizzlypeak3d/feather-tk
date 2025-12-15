// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ScrollArea.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Scroll widget viewport information.
    struct FTK_API_TYPE ScrollViewport
    {
        Size2I size;
        Size2I scrollSize;
        V2I    scrollPos;
        
        bool operator == (const ScrollViewport&) const;
        bool operator != (const ScrollViewport&) const;
    };

    //! Scroll widget.
    class FTK_API_TYPE ScrollWidget : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            ScrollType,
            const std::shared_ptr<IWidget>& parent);

        ScrollWidget();

    public:
        FTK_API virtual ~ScrollWidget();

        //! Create a new widget.
        FTK_API static std::shared_ptr<ScrollWidget> create(
            const std::shared_ptr<Context>&,
            ScrollType = ScrollType::Both,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Scroll Widget
        ///@{

        FTK_API const std::shared_ptr<IWidget>& getWidget() const;
        FTK_API void setWidget(const std::shared_ptr<IWidget>&);
        
        ///@}

        //! \name Scroll Type
        ///@{

        FTK_API ScrollType getScrollType() const;
        FTK_API void setScrollType(ScrollType);
        
        ///@}

        //! \name Scroll Size
        ///@{

        FTK_API const Size2I& getScrollSize() const;
        FTK_API void setScrollSizeCallback(const std::function<void(const Size2I&)>&);
        
        ///@}

        //! \name Scroll Position
        ///@{

        FTK_API const V2I& getScrollPos() const;
        FTK_API void setScrollPos(const V2I&, bool clamp = true);
        FTK_API void setScrollPosCallback(const std::function<void(const V2I&)>&);

        //! Scroll to make the given box visible.
        FTK_API void scrollTo(const Box2I&);

        ///@}

        //! \name Scroll Area
        ///@{

        //! Get whether the scrolled area is resizable.
        FTK_API bool isAreaResizable() const;

        //! Get whether the scrolled area is resizable.
        FTK_API void setAreaResizable(bool);

        ///@}

        //! \name Options
        ///@{

        //! Get whether the scroll bars are visible.
        FTK_API bool areScrollBarsVisible() const;

        //! Set whether the scroll bars are visible.
        FTK_API void setScrollBarsVisible(bool);

        //! Get whether the scroll bars auto-hide.
        FTK_API bool getScrollBarsAutoHide() const;

        //! Set whether the scroll bars auto-hide.
        FTK_API void setScrollBarsAutoHide(bool);

        //! Get whether scroll events are enabled.
        FTK_API bool areScrollEventsEnabled() const;

        //! Set whether scroll events are enabled.
        FTK_API void setScrollEventsEnabled(bool);

        //! Get the line step.
        FTK_API int getLineStep() const;

        //! Set the line step.
        FTK_API void setLineStep(int);

        //! Get whether the scroll area has a border.
        FTK_API bool hasBorder() const;

        //! Set whether the scroll area has a border.
        FTK_API void setBorder(bool);

        //! Get the size hint role.
        FTK_API SizeRole getSizeHintRole() const;

        //! Set the size hint role.
        FTK_API void setSizeHintRole(SizeRole);

        //! Get the margin role.
        FTK_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_API void setMarginRole(SizeRole);

        ///@}

        //! \name Viewport
        ///@{

        //! Get the viewport information.
        FTK_API ScrollViewport getViewport() const;

        //! Set the viewport callback.
        FTK_API void setViewportCallback(
            const std::function<void(const ScrollViewport&)>&);

        //! Get the viewport widget.
        FTK_API const std::shared_ptr<IWidget>& getViewportWidget() const;

        //! Set the viewport widget.
        FTK_API void setViewportWidget(const std::shared_ptr<IWidget>&);

        ///@}

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;
        FTK_API void scrollEvent(ScrollEvent&) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        int _getPageStep() const;

        void _scrollBarsUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
