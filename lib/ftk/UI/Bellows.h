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
        virtual ~Bellows();

        //! Create a new widget.
        static std::shared_ptr<Bellows> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Create a new widget.
        static std::shared_ptr<Bellows> create(
            const std::shared_ptr<Context>&,
            const std::string& text,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Get the text.
        const std::string& getText() const;

        //! Set the text.
        void setText(const std::string&);

        //! Get the widget.
        const std::shared_ptr<IWidget>& getWidget() const;

        //! Set the widget.
        void setWidget(const std::shared_ptr<IWidget>&);

        //! Get whether the bellows is open.
        bool isOpen() const;

        //! Set whether the bellows is open.
        void setOpen(bool);

        //! Set the open callback.
        void setOpenCallback(const std::function<void(bool)>&);

        //! Get the tool widget.
        const std::shared_ptr<IWidget>& getToolWidget() const;

        //! Set the tool widget.
        void setToolWidget(const std::shared_ptr<IWidget>&);

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
