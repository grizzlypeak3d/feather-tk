// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/ScrollWidget.h>

namespace ftk
{
    //! \name MDI Widgets
    ///@{

    //! MDI mini-map widget.
    class FTK_UI_API_TYPE MDIMiniMap : public IWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent);

        MDIMiniMap();

    public:
        FTK_UI_API virtual ~MDIMiniMap();

        //! Create a new widget.
        FTK_UI_API static std::shared_ptr<MDIMiniMap> create(
            const std::shared_ptr<Context>&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the scroll information.
        FTK_UI_API void setScrollInfo(const ScrollInfo&);

        //! Set the callback.
        FTK_UI_API void setCallback(const std::function<void(const V2I&)>&);

        //! Set the child geometry.
        FTK_UI_API void setChildGeometry(const std::vector<Box2I>&);
        
        //! Get the child geometry color role.
        FTK_UI_API ColorRole getChildColorRole() const;
        
        //! Set the child geometry color role.
        FTK_UI_API void setChildColorRole(ColorRole);

        FTK_UI_API void setGeometry(const Box2I&) override;
        FTK_UI_API void styleEvent(const StyleEvent&) override;
        FTK_UI_API void sizeHintEvent(const SizeHintEvent&) override;
        FTK_UI_API void drawEvent(const Box2I&, const DrawEvent&) override;

    private:
        FTK_PRIVATE();
    };
    
    ///@}
}
