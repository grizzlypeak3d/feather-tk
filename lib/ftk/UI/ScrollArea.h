// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Scroll type.
    enum class FTK_UI_API_TYPE ScrollType
    {
        Both,
        Horizontal,
        Vertical,
        Menu,

        Count,
        First = Both
    };
    FTK_ENUM(FTK_UI_API, ScrollType);

    //! Scroll area.
    class FTK_UI_API_TYPE ScrollArea : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            ScrollType,
            const std::shared_ptr<IWidget>& parent);

        ScrollArea();

    public:
        FTK_UI_API virtual ~ScrollArea();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ScrollArea> create(
            const std::shared_ptr<Context>&,
            ScrollType = ScrollType::Both,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! \name Scroll Type
        ///@{

        //! Get the directions the contents scroll in.
        FTK_UI_API ScrollType getScrollType() const;
        //! Set the directions the contents scroll in.
        FTK_UI_API void setScrollType(ScrollType);
        
        ///@}

        //! \name Scroll Size
        ///@{

        //! Get the size of the scrolled contents.
        FTK_UI_API const Size2I& getScrollSize() const;
        //! Set a callback for when the size of the scrolled contents changes.
        FTK_UI_API void setScrollSizeCallback(const std::function<void(const Size2I&)>&);

        ///@}

        //! \name Scroll Position
        ///@{

        //! Get the scroll position: how far the contents are scrolled, in
        //! pixels.
        FTK_UI_API const V2I& getScrollPos() const;
        //! Set the scroll position. With clamp, it is kept within the
        //! scrollable range.
        FTK_UI_API void setScrollPos(const V2I&, bool clamp = true);
        //! Set a callback for when the scroll position changes, including from
        //! setScrollPos() and scrollTo().
        FTK_UI_API void setScrollPosCallback(const std::function<void(const V2I&)>&);

        //! Scroll to make the given box visible.
        FTK_UI_API void scrollTo(const Box2I&);
        
        ///@}

        //! \name Options
        ///@{

        //! Get whether the scrolled area is resizable.
        FTK_UI_API bool isAreaResizable() const;

        //! Set whether the scrolled area is resizable.
        FTK_UI_API void setAreaResizable(bool);

        //! Get the size hint role.
        FTK_UI_API SizeRole getSizeHintRole() const;

        //! Set the size hint role.
        FTK_UI_API void setSizeHintRole(SizeRole);
        
        ///@}

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
