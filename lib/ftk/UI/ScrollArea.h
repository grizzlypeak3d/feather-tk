// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Scroll type.
    enum class FTK_API_TYPE ScrollType
    {
        Both,
        Horizontal,
        Vertical,
        Menu,

        Count,
        First = Both
    };
    FTK_ENUM(ScrollType);

    //! Scroll area.
    class FTK_API_TYPE ScrollArea : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            ScrollType,
            const std::shared_ptr<IWidget>& parent);

        ScrollArea();

    public:
        FTK_API virtual ~ScrollArea();

        //! Create a new widget.
        FTK_API static std::shared_ptr<ScrollArea> create(
            const std::shared_ptr<Context>&,
            ScrollType = ScrollType::Both,
            const std::shared_ptr<IWidget>& parent = nullptr);

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

        //! \name Options
        ///@{

        //! Get whether the scrolled area is resizable.
        bool isAreaResizable() const;

        //! Set whether the scrolled area is resizable.
        void setAreaResizable(bool);

        //! Get the size hint role.
        FTK_API SizeRole getSizeHintRole() const;

        //! Set the size hint role.
        FTK_API void setSizeHintRole(SizeRole);
        
        ///@}

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
