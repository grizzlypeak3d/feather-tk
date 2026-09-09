// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/App.h>

namespace imageview
{
    //! Window settings.
    struct WindowSettings
    {
        bool  settings = false;
        float split    = .7F;

        bool operator == (const WindowSettings&) const;
        bool operator != (const WindowSettings&) const;
    };

    //! Style settings.
    struct StyleSettings
    {
        //! Zero follows the display.
        float displayScale = 0.F;
        ftk::ColorStyle colorStyle = ftk::ColorStyle::Dark;

        bool operator == (const StyleSettings&) const;
        bool operator != (const StyleSettings&) const;
    };

    void to_json(nlohmann::json&, const WindowSettings&);
    void to_json(nlohmann::json&, const StyleSettings&);

    void from_json(const nlohmann::json&, WindowSettings&);
    void from_json(const nlohmann::json&, StyleSettings&);
}
