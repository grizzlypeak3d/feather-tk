// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Stack layout.
    class StackLayout : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        StackLayout();

    public:
        virtual ~StackLayout();

        //! Create a new layout.
        static std::shared_ptr<StackLayout> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the current index.
        int getCurrentIndex() const;

        //! Set the current index.
        void setCurrentIndex(int);

        //! Go to the next index.
        void nextIndex();

        //! Observe whether there is a next index.
        std::shared_ptr<IObservable<bool> > observeHasNextIndex() const;

        //! Go to the previous index.
        void prevIndex();

        //! Observe whether there is a previous index.
        std::shared_ptr<IObservable<bool> > observeHasPrevIndex() const;

        //! Go to the first index.
        void firstIndex();

        //! Go to the last index.
        void lastIndex();

        //! Set the current widget.
        void setCurrentWidget(const std::shared_ptr<IWidget>&);

        //! Get the margin role.
        SizeRole getMarginRole() const;

        //! Set the margin role.
        void setMarginRole(SizeRole);

        //! Remove all children from the group box.
        void clear();

        void setGeometry(const Box2I&) override;
        Box2I getChildrenClipRect() const override;
        void childAddEvent(const ChildAddEvent&) override;
        void childRemoveEvent(const ChildRemoveEvent&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    ///@}
}