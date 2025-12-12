// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/ListItemsWidget.h>

namespace ftk
{
    //! \name List Widgets
    ///@{

    //! List widget.
    class FTK_API_TYPE ListWidget : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            ButtonGroupType,
            const std::shared_ptr<IWidget>& parent);

        ListWidget();

    public:
        FTK_API virtual ~ListWidget();

        //! Create a new widget.
        FTK_API static std::shared_ptr<ListWidget> create(
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

        //! Set the checked item.
        FTK_API void setChecked(int, bool = true);

        //! Set the callback.
        FTK_API void setCallback(const std::function<void(int, bool)>&);

        //! Get the current item.
        FTK_API int getCurrent() const;

        //! Set the current item.
        FTK_API void setCurrent(int);

        //! Get the search.
        FTK_API const std::string& getSearch() const;

        //! Set the search.
        FTK_API void setSearch(const std::string&);

        //! Clear the search.
        FTK_API void clearSearch();

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _scrollUpdate(int);

        FTK_PRIVATE();
    };

    ///@}
}
