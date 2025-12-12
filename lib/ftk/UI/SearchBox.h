// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Text Widgets
    ///@{

    //! Search box widget.
    class FTK_API_TYPE SearchBox : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        SearchBox();

    public:
        FTK_API virtual ~SearchBox();

        //! Create a new widget.
        FTK_API static std::shared_ptr<SearchBox> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the text.
        FTK_API const std::string& getText() const;

        //! Set the text.
        FTK_API void setText(const std::string&);

        //! Set the callback.
        FTK_API void setCallback(const std::function<void(const std::string&)>&);

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;

    private:
        void _widgetUpdate();

        FTK_PRIVATE();
    };

    ///@}
}
