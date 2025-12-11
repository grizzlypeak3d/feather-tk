// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    class Action;
    class ToolButton;

    //! \name Composite Widgets
    ///@{
        
    //! Tool bar.
    class FTK_API_TYPE ToolBar : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            Orientation,
            const std::shared_ptr<IWidget>& parent);

        ToolBar();

    public:
        virtual ~ToolBar();

        //! Create a new widget.
        static std::shared_ptr<ToolBar> create(
            const std::shared_ptr<Context>&,
            Orientation = Orientation::Horizontal,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Add an action.
        std::shared_ptr<ToolButton> addAction(const std::shared_ptr<Action>&);

        //! Add a widget.
        void addWidget(const std::shared_ptr<IWidget>&);

        //! Clear the tool bar.
        void clear();

        void setGeometry(const Box2I&) override;
        void sizeHintEvent(const SizeHintEvent&) override;

    private:
        FTK_PRIVATE();
    };
        
    ///@}
}
