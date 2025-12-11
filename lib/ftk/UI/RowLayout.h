// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{
        
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
        virtual ~RowLayout();

        //! Create a new layout.
        static std::shared_ptr<RowLayout> create(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the margin role.
        SizeRole getMarginRole() const;

        //! Set the margin role.
        void setMarginRole(SizeRole);

        //! Get the spacing role.
        SizeRole getSpacingRole() const;

        //! Set the spacing role.
        void setSpacingRole(SizeRole);

        //! Add a spacer.
        void addSpacer(Stretch = Stretch::Fixed);

        //! Add a spacer.
        void addSpacer(SizeRole, Stretch = Stretch::Fixed);

        //! Remove all children from the layout.
        void clear();

        void setGeometry(const Box2I&) override;
        Box2I getChildrenClipRect() const override;
        void sizeHintEvent(const SizeHintEvent&) override;
        void childAddEvent(const ChildAddEvent&) override;
        void childRemoveEvent(const ChildRemoveEvent&) override;

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
        virtual ~HorizontalLayout();

        //! Create a new layout.
        static std::shared_ptr<HorizontalLayout> create(
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
        virtual ~VerticalLayout();

        //! Create a new layout.
        static std::shared_ptr<VerticalLayout> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);
    };
        
    ///@}
}
