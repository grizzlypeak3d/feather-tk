// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/WidgetOptions.h>

#include <ftk/Core/Box.h>

namespace ftk
{
    //! \name Layouts
    ///@{
        
    //! Align within the given box.
    FTK_API Box2I align(
        const Box2I&  box,
        const Size2I& sizeHint,
        HAlign        hAlign,
        VAlign        vAlign);

    //! Get a format string for the given number.
    FTK_API std::string format(int);

    //! Get a format string for the given number.
    FTK_API std::string format(float, int precision);
        
    ///@}
}
