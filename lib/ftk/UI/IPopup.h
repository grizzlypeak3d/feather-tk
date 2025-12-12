// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/IMouseWidget.h>

namespace ftk
{
    //! \name Popup Widgets
    ///@{
        
    //! Base class for popups.
    class FTK_API_TYPE IPopup : public IMouseWidget
    {
    protected:
        void _init(
            const std::shared_ptr<Context>&,
            const std::string& objectName,
            const std::shared_ptr<IWidget>& parent = nullptr);

        IPopup();

    public:
        FTK_API virtual ~IPopup() = 0;

        //! Close the popup.
        FTK_API virtual void close() = 0;
    };
        
    ///@}
}
