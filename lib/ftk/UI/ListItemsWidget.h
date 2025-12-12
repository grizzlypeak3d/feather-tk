// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ButtonGroup.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name List Widgets
    ///@{

    //! List item.
    struct FTK_API_TYPE ListItem
    {
        ListItem() = default;
        FTK_API explicit ListItem(
            const std::string& text,
            const std::string& tooltip = std::string());

        std::string text;
        std::string tooltip;

        FTK_API bool operator == (const ListItem&) const;
        FTK_API bool operator != (const ListItem&) const;
    };

    //! List items widget.
    class FTK_API_TYPE ListItemsWidget : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            ButtonGroupType,
            const std::shared_ptr<IWidget>& parent);

        ListItemsWidget();

    public:
        FTK_API virtual ~ListItemsWidget();

        //! Create a new widget.
        FTK_API static std::shared_ptr<ListItemsWidget> create(
            const std::shared_ptr<Context>&,
            ButtonGroupType,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the items.
        FTK_API const std::vector<ListItem>& getItems() const;

        //! Set the items.
        FTK_API void setItems(const std::vector<ListItem>&);

        //! Set the items.
        FTK_API void setItems(const std::vector<std::string>&);

        //! Get whether an item is checked.
        FTK_API bool getChecked(int) const;

        //! Set whether an item is checked.
        FTK_API void setChecked(int, bool);

        //! Set the callback.
        FTK_API void setCallback(const std::function<void(int, bool)>&);

        //! Get the current item.
        FTK_API int getCurrent() const;

        //! Observe the current item.
        FTK_API std::shared_ptr<IObservable<int> > observeCurrent() const;

        //! Set the current item.
        FTK_API void setCurrent(int);

        //! Observe the item to scroll to.
        FTK_API std::shared_ptr<IObservable<int> > observeScrollTo() const;

        //! Get the search.
        FTK_API const std::string& getSearch() const;

        //! Set the search.
        FTK_API void setSearch(const std::string&);

        //! Clear the search.
        FTK_API void clearSearch();

        //! Get an item rectangle.
        FTK_API Box2I getRect(int) const;

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void keyFocusEvent(bool) override;
        FTK_API void keyPressEvent(KeyEvent&) override;
        FTK_API void keyReleaseEvent(KeyEvent&) override;

    private:
        void _itemsUpdate();
        void _currentUpdate();

        FTK_PRIVATE();
    };
}
