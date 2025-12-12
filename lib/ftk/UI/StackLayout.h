// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Stack layout.
    class FTK_API_TYPE StackLayout : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        StackLayout();

    public:
        FTK_API virtual ~StackLayout();

        //! Create a new layout.
        FTK_API static std::shared_ptr<StackLayout> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the current index.
        FTK_API int getCurrentIndex() const;

        //! Set the current index.
        FTK_API void setCurrentIndex(int);

        //! Go to the next index.
        FTK_API void nextIndex();

        //! Observe whether there is a next index.
        FTK_API std::shared_ptr<IObservable<bool> > observeHasNextIndex() const;

        //! Go to the previous index.
        FTK_API void prevIndex();

        //! Observe whether there is a previous index.
        FTK_API std::shared_ptr<IObservable<bool> > observeHasPrevIndex() const;

        //! Go to the first index.
        FTK_API void firstIndex();

        //! Go to the last index.
        FTK_API void lastIndex();

        //! Set the current widget.
        FTK_API void setCurrentWidget(const std::shared_ptr<IWidget>&);

        //! Get the margin role.
        FTK_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_API void setMarginRole(SizeRole);

        //! Remove all children from the group box.
        FTK_API void clear();

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API Box2I getChildrenClipRect() const override;
        FTK_API void childAddEvent(const ChildAddEvent&) override;
        FTK_API void childRemoveEvent(const ChildRemoveEvent&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
