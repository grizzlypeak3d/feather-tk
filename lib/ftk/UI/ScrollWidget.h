// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ScrollArea.h>

namespace ftk
{
    //! \name Layouts
    ///@{

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
        virtual ~ScrollWidget();

        //! Create a new widget.
        static std::shared_ptr<ScrollWidget> create(
            const std::shared_ptr<Context>&,
            ScrollType = ScrollType::Both,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the widget.
        const std::shared_ptr<IWidget>& getWidget() const;

        //! Set the widget.
        void setWidget(const std::shared_ptr<IWidget>&);

        //! Get the viewport geometry.
        Box2I getViewport() const;

        //! Get the scroll type.
        ScrollType getScrollType() const;

        //! Set the scroll type.
        void setScrollType(ScrollType);

        //! Get the scroll size.
        const Size2I& getScrollSize() const;

        //! Get the scroll position.
        const V2I& getScrollPos() const;

        //! Set the scroll position.
        void setScrollPos(const V2I&, bool clamp = true);

        //! Scroll to make the given box visible.
        void scrollTo(const Box2I&);

        //! Set the scroll position callback.
        void setScrollPosCallback(const std::function<void(const V2I&)>&);

        //! Get whether the scroll bars are visible.
        bool areScrollBarsVisible() const;

        //! Set whether the scroll bars are visible.
        void setScrollBarsVisible(bool);

        //! Get whether the scroll bars auto-hide.
        bool getScrollBarsAutoHide() const;

        //! Set whether the scroll bars auto-hide.
        void setScrollBarsAutoHide(bool);

        //! Get whether scroll events are enabled.
        bool areScrollEventsEnabled() const;

        //! Set whether scroll events are enabled.
        void setScrollEventsEnabled(bool);

        //! Get the line step.
        int getLineStep() const;

        //! Set the line step.
        void setLineStep(int);

        //! Get the margin role.
        SizeRole getMarginRole() const;

        //! Set the margin role.
        void setMarginRole(SizeRole);

        //! Get whether the scroll area has a border.
        bool hasBorder() const;

        //! Set whether the scroll area has a border.
        void setBorder(bool);

        //! Get the size hint role.
        SizeRole getSizeHintRole() const;

        //! Set the size hint role.
        void setSizeHintRole(SizeRole);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void drawEvent(const Box2I&, const DrawEvent&) override;
        void scrollEvent(ScrollEvent&) override;
        void keyPressEvent(KeyEvent&) override;
        void keyReleaseEvent(KeyEvent&) override;

    private:
        int _getPageStep() const;

        void _scrollBarsUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
