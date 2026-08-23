// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IContainer.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Text Widgets
    ///@{

    //! Search box widget.
    class FTK_UI_API_TYPE SearchBox : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        SearchBox();

    public:
        FTK_UI_API virtual ~SearchBox();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<SearchBox> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the text.
        FTK_UI_API const std::string& getText() const;

        //! Set the text.
        FTK_UI_API void setText(const std::string&);

        //! Set the callback.
        FTK_UI_API void setCallback(const std::function<void(const std::string&)>&);


    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
