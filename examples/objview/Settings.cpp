// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include "SettingsModel.h"

#include <algorithm>
#include <array>
#include <sstream>

using namespace ftk;

namespace objview
{
    bool WindowSettings::operator == (const WindowSettings& other) const
    {
        return
            settings == other.settings &&
            split == other.split;
    }

    bool WindowSettings::operator != (const WindowSettings& other) const
    {
        return !(*this == other);
    }

    FTK_ENUM_IMPL(
        RenderMode,
        "Shaded",
        "Flat",
        "Texture",
        "Normals");

    bool RenderSettings::operator == (const RenderSettings& other) const
    {
        return
            mode == other.mode &&
            grid == other.grid &&
            cull == other.cull;
    }

    bool RenderSettings::operator != (const RenderSettings& other) const
    {
        return !(*this == other);
    }

    bool StyleSettings::operator == (const StyleSettings& other) const
    {
        return
            displayScale == other.displayScale &&
            colorStyle == other.colorStyle;
    }

    bool StyleSettings::operator != (const StyleSettings& other) const
    {
        return !(*this == other);
    }

    void to_json(nlohmann::json& json, const WindowSettings& value)
    {
        json["Settings"] = value.settings;
        json["Split"] = value.split;
    }

    void to_json(nlohmann::json& json, const RenderSettings& value)
    {
        json["Mode"] = to_string(value.mode);
        json["Grid"] = value.grid;
        json["Cull"] = value.cull;
    }

    void to_json(nlohmann::json& json, const StyleSettings& value)
    {
        json["DisplayScale"] = value.displayScale;
        json["ColorStyle"] = to_string(value.colorStyle);
    }

    void from_json(const nlohmann::json& json, WindowSettings& value)
    {
        json.at("Settings").get_to(value.settings);
        json.at("Split").get_to(value.split);
    }

    void from_json(const nlohmann::json& json, RenderSettings& value)
    {
        from_string(json.at("Mode").get<std::string>(), value.mode);
        json.at("Grid").get_to(value.grid);
        json.at("Cull").get_to(value.cull);
    }

    void from_json(const nlohmann::json& json, StyleSettings& value)
    {
        json.at("DisplayScale").get_to(value.displayScale);
        from_string(json.at("ColorStyle").get<std::string>(), value.colorStyle);
    }
}
