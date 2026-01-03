// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name Layouts
    ///@{
        
    //! Bellows widget.
    class FTK_API_TYPE Bellows : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        Bellows();

    public:
        FTK_API virtual ~Bellows();

        //! Create a new widget.
        FTK_API static std::shared_ptr<Bellows> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        FTK_API static std::shared_ptr<Bellows> create(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the text.
        FTK_API const std::string& getText() const;

        //! Set the text.
        void setText(const std::string&);

        //! Get the widget.
        FTK_API const std::shared_ptr<IWidget>& getWidget() const;

        //! Set the widget.
        FTK_API void setWidget(const std::shared_ptr<IWidget>&);

        //! Get whether the bellows is open.
        FTK_API bool isOpen() const;

        //! Set whether the bellows is open.
        FTK_API void setOpen(bool);

        //! Set the open callback.
        FTK_API void setOpenCallback(const std::function<void(bool)>&);

        //! Get the tool widget.
        FTK_API const std::shared_ptr<IWidget>& getToolWidget() const;

        //! Set the tool widget.
        FTK_API void setToolWidget(const std::shared_ptr<IWidget>&);

        FTK_API Size2I getSizeHint() const override;
        FTK_API void setGeometry(const Box2I&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
