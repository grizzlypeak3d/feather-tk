// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IContainer.h>
#include <ftk/UI/ListItemsWidget.h>

namespace ftk
{
    //! \name List Widgets
    ///@{

    //! List widget.
    class FTK_UI_API_TYPE ListWidget : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            ButtonGroupType,
            const std::shared_ptr<IWidget>& parent);

        ListWidget();

    public:
        FTK_UI_API virtual ~ListWidget();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<ListWidget> create(
            const std::shared_ptr<Context>&,
            ButtonGroupType,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the items.
        FTK_UI_API const std::vector<ListItem>& getItems() const;

        //! Set the items.
        FTK_UI_API void setItems(const std::vector<ListItem>&);

        //! Set the items.
        FTK_UI_API void setItems(const std::vector<std::string>&);

        //! Get whether an item is checked.
        FTK_UI_API bool getChecked(int) const;

        //! Set the checked item.
        FTK_UI_API void setChecked(int, bool = true);

        //! Set the callback.
        FTK_UI_API void setCallback(const std::function<void(int, bool)>&);

        //! Get the current item.
        FTK_UI_API int getCurrent() const;

        //! Set the current item.
        FTK_UI_API void setCurrent(int);

        //! Get the search.
        FTK_UI_API const std::string& getSearch() const;

        //! Set the search.
        FTK_UI_API void setSearch(const std::string&);

        //! Clear the search.
        FTK_UI_API void clearSearch();


    private:
        void _scrollUpdate(int);

        FTK_PRIVATE();
    };

    ///@}
}
