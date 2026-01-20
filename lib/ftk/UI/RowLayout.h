// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{
        
    //! Row layout margins.
    struct FTK_API_TYPE RowMargins
    {
        RowMargins() = default;
        FTK_API RowMargins(SizeRole);
        FTK_API RowMargins(SizeRole horizontal, SizeRole vertical);
        FTK_API RowMargins(
            SizeRole left,
            SizeRole top,
            SizeRole right,
            SizeRole bottom);

        SizeRole left   = SizeRole::None;
        SizeRole top    = SizeRole::None;
        SizeRole right  = SizeRole::None;
        SizeRole bottom = SizeRole::None;

        FTK_API bool operator == (const RowMargins&) const;
        FTK_API bool operator != (const RowMargins&) const;
    };

    //! Row layout.
    class FTK_API_TYPE RowLayout : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::string& objectName,
            const std::shared_ptr<IWidget>& parent);

        RowLayout();

    public:
        FTK_API virtual ~RowLayout();

        //! Create a new layout.
        FTK_API static std::shared_ptr<RowLayout> create(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the margins.
        FTK_API const RowMargins& getMargins() const;

        //! Set the margins.
        FTK_API void setMargins(const RowMargins&);

        //! Get the margin role.
        FTK_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_API void setMarginRole(SizeRole);

        //! Get the spacing role.
        FTK_API SizeRole getSpacingRole() const;

        //! Set the spacing role.
        FTK_API void setSpacingRole(SizeRole);

        //! Add a spacer.
        FTK_API void addSpacer(Stretch = Stretch::Fixed);

        //! Add a spacer.
        FTK_API void addSpacer(SizeRole, Stretch = Stretch::Fixed);

        //! Remove all children from the layout.
        FTK_API void clear();

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;
        FTK_API Box2I getChildrenClipRect() const override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void childAddEvent(const ChildAddEvent&) override;
        FTK_API void childRemoveEvent(const ChildRemoveEvent&) override;

    private:
        FTK_PRIVATE();
    };

    //! Horizontal layout.
    class FTK_API_TYPE HorizontalLayout : public RowLayout
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        HorizontalLayout();

    public:
        FTK_API virtual ~HorizontalLayout();

        //! Create a new layout.
        FTK_API static std::shared_ptr<HorizontalLayout> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);
    };

    //! Vertical layout.
    class FTK_API_TYPE VerticalLayout : public RowLayout
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        VerticalLayout();

    public:
        FTK_API virtual ~VerticalLayout();

        //! Create a new layout.
        FTK_API static std::shared_ptr<VerticalLayout> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);
    };
        
    ///@}
}
