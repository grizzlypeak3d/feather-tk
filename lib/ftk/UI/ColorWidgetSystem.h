// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/Export.h>
#include <ftk/UI/ColorWidget.h>

#include <ftk/Core/ISystem.h>

#include <vector>

namespace ftk
{
    //! Shared state for the color widgets.
    //!
    //! Every color popup is created fresh, so without somewhere shared the
    //! widget mode reset to the first tab on every open. The system holds
    //! the mode the user last chose and the palette colors, so each picker
    //! in an application opens the way the last one was left. Applications
    //! that persist the state across sessions read and write it here.
    class FTK_UI_API_TYPE ColorWidgetSystem : public ISystem
    {
    protected:
        ColorWidgetSystem(const std::shared_ptr<Context>&);

    public:
        FTK_UI_API virtual ~ColorWidgetSystem();

        //! Create a new system.
        FTK_UI_API static std::shared_ptr<ColorWidgetSystem> create(
            const std::shared_ptr<Context>&);

        //! Get the widget mode.
        FTK_UI_API ColorWidgetMode getMode() const;

        //! Set the widget mode.
        FTK_UI_API void setMode(ColorWidgetMode);

        //! Get the palette colors.
        FTK_UI_API const std::vector<Color4F>& getPalette() const;

        //! Set the palette colors.
        FTK_UI_API void setPalette(const std::vector<Color4F>&);

    private:
        FTK_PRIVATE();
    };
}
