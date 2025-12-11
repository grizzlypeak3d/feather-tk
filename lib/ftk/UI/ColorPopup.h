// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IWidgetPopup.h>

namespace ftk
{
    //! \name Color Widgets
    ///@{

    //! Color popup.
    class FTK_API_TYPE ColorPopup : public IWidgetPopup
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const Color4F&,
            const std::shared_ptr<IWidget>& parent);

        ColorPopup();

    public:
        virtual ~ColorPopup();

        //! Create a new popup.
        static std::shared_ptr<ColorPopup> create(
            const std::shared_ptr<Context>&,
            const Color4F&,
            const std::shared_ptr<IWidget>& parent = nullptr);

        //! Set the callback.
        void setCallback(const std::function<void(const Color4F&)>&);

    private:
        FTK_PRIVATE();
    };

    ///@}
}
