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
        virtual ~ScrollArea();

        //! Create a new widget.
        static std::shared_ptr<ScrollArea> create(
            const std::shared_ptr<Context>&,
            ScrollType = ScrollType::Both,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the scroll type.
        ScrollType getScrollType() const;

        //! Set the scroll type.
        void setScrollType(ScrollType);

        //! Get the scroll size.
        const Size2I& getScrollSize() const;

        //! Set the scroll size callback.
        void setScrollSizeCallback(const std::function<void(const Size2I&)>&);

        //! Get the scroll position.
        const V2I& getScrollPos() const;

        //! Set the scroll position.
        void setScrollPos(const V2I&, bool clamp = true);

        //! Scroll to make the given box visible.
        void scrollTo(const Box2I&);

        //! Set the scroll position callback.
        void setScrollPosCallback(const std::function<void(const V2I&)>&);

        //! Get the size hint role.
        SizeRole getSizeHintRole() const;

        //! Set the size hint role.
        void setSizeHintRole(SizeRole);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        FTK_PRIVATE();
    };

    ///@}
}
