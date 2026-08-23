// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/UI/App.h>

namespace objview
{
    //! Window settings.
    struct WindowSettings
    {
        bool  settings = false;
        float split    = .7F;

        bool operator == (const WindowSettings&) const;
        bool operator != (const WindowSettings&) const;
    };

    //! Render modes.
    enum class RenderMode
    {
        Shaded,
        Flat,
        Texture,
        Normals,

        Count,
        First = Shaded
    };
    // The empty first argument to FTK_ENUM is the export macro, and an
    // example is a program with nothing to export.
    FTK_ENUM(, RenderMode);

    //! Render settigns.
    struct RenderSettings
    {
        RenderMode mode = RenderMode::Shaded;
        bool       grid = true;
        bool       cull = true;

        bool operator == (const RenderSettings&) const;
        bool operator != (const RenderSettings&) const;
    };

    //! Style settings.
    struct StyleSettings
    {
        float displayScale = 2.F;
        ftk::ColorStyle colorStyle = ftk::ColorStyle::Dark;

        bool operator == (const StyleSettings&) const;
        bool operator != (const StyleSettings&) const;
    };

    void to_json(nlohmann::json&, const WindowSettings&);
    void to_json(nlohmann::json&, const RenderSettings&);
    void to_json(nlohmann::json&, const StyleSettings&);

    void from_json(const nlohmann::json&, WindowSettings&);
    void from_json(const nlohmann::json&, RenderSettings&);
    void from_json(const nlohmann::json&, StyleSettings&);
}
