// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{

    //! Stack layout.
    class FTK_UI_API_TYPE StackLayout : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        StackLayout();

    public:
        FTK_UI_API virtual ~StackLayout();

        //! Create a new layout.
        FTK_UI_API static std::shared_ptr<StackLayout> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the current index.
        FTK_UI_API int getCurrentIndex() const;

        //! Set the current index.
        FTK_UI_API void setCurrentIndex(int);

        //! Go to the next index.
        FTK_UI_API void nextIndex();

        //! Observe whether there is a next index.
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeHasNextIndex() const;

        //! Go to the previous index.
        FTK_UI_API void prevIndex();

        //! Observe whether there is a previous index.
        FTK_UI_API std::shared_ptr<IObservable<bool> > observeHasPrevIndex() const;

        //! Go to the first index.
        FTK_UI_API void firstIndex();

        //! Go to the last index.
        FTK_UI_API void lastIndex();

        //! Get the current widget.
        FTK_UI_API std::shared_ptr<IWidget> getCurrentWidget() const;

        //! Set the current widget.
        FTK_UI_API void setCurrentWidget(const std::shared_ptr<IWidget>&);

        //! Remove all children from the layout.
        FTK_UI_API void clear();

        //! Get whether the layout size fits all the children or just the
        //! current one.
        FTK_UI_API bool hasFitAll() const;

        //! Set whether the layout size fits all the children or just the
        //! current one.
        FTK_UI_API void setFitAll(bool);

        //! Get the margin role.
        FTK_UI_API SizeRole getMarginRole() const;

        //! Set the margin role.
        FTK_UI_API void setMarginRole(SizeRole);

        FTK_UI_API Size2I getSizeHint() const override;
        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API Box2I getChildrenClipRect() const override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void childAddEvent(const ChildAddEvent&) override;
        FTK_UI_API void childRemoveEvent(const ChildRemoveEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
