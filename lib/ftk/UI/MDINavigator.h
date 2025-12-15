// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidget.h>

namespace ftk
{
    //! \name MDI Widgets
    ///@{

    //! MDI navigator widget.
    class FTK_API_TYPE MDINavigator : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        MDINavigator();

    public:
        FTK_API virtual ~MDINavigator();

        //! Create a new widget.
        FTK_API static std::shared_ptr<MDINavigator> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the widget geometry.
        FTK_API void setWidgetGeometry(const std::vector<Box2I>&);

        //! Set the scroll size.
        FTK_API void setScrollSize(const Size2I&);

        //! Set the scroll position.
        FTK_API void setScrollPos(const V2I&);

        //! Set the scroll viewport size.
        FTK_API void setViewportSize(const Size2I&);
        
        //! Set the callback.
        FTK_API void setCallback(const std::function<void(const V2I&)>&);

        FTK_API void setGeometry(const Box2I&) override;
        FTK_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };
    
    ///@}
}
