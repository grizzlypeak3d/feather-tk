// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/IContainer.h>
#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{
        
    //! Bellows widget.
    class FTK_UI_API_TYPE Bellows : public IContainer
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        Bellows();

    public:
        FTK_UI_API virtual ~Bellows();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<Bellows> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<Bellows> create(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the text.
        FTK_UI_API const std::string& getText() const;

        //! Set the text.
        FTK_UI_API void setText(const std::string&);

        //! Get the widget.
        FTK_UI_API const std::shared_ptr<IWidget>& getWidget() const;

        //! Set the widget.
        FTK_UI_API void setWidget(const std::shared_ptr<IWidget>&);

        //! Get whether the bellows is open.
        FTK_UI_API bool isOpen() const;

        //! Set whether the bellows is open.
        FTK_UI_API void setOpen(bool);

        //! Set the open callback.
        FTK_UI_API void setOpenCallback(const std::function<void(bool)>&);

        //! Get the tool widget.
        FTK_UI_API const std::shared_ptr<IWidget>& getToolWidget() const;

        //! Set the tool widget.
        FTK_UI_API void setToolWidget(const std::shared_ptr<IWidget>&);


    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
