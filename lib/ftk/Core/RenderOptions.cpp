// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/RenderOptions.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <algorithm>
#include <array>
#include <sstream>

namespace ftk
{
    FTK_ENUM_IMPL(
        ChannelDisplay,
        "Color",
        "Red",
        "Green",
        "Blue",
        "Alpha");

    FTK_ENUM_IMPL(
        InputVideoLevels,
        "From File",
        "Full Range",
        "Legal Range");

    FTK_ENUM_IMPL(
        AlphaBlend,
        "None",
        "Straight",
        "Premultiplied");

    FTK_ENUM_IMPL(
        ImageFilter,
        "Nearest",
        "Linear");

    void to_json(nlohmann::json& json, const ImageFilters& in)
    {
        json["Minify"] = to_string(in.minify);
        json["Magnify"] = to_string(in.magnify);
    }

    void to_json(nlohmann::json& json, const ImageOptions& in)
    {
        json["ChannelDisplay"] = to_string(in.channelDisplay);
        json["VideoLevels"] = to_string(in.videoLevels);
        json["AlphaBlend"] = to_string(in.alphaBlend);
        json["ImageFilters"] = in.imageFilters;
        json["Cache"] = in.cache;
    }

    void from_json(const nlohmann::json& json, ImageFilters& out)
    {
        from_string(json.at("Minify").get<std::string>(), out.minify);
        from_string(json.at("Magnify").get<std::string>(), out.magnify);
    }

    void from_json(const nlohmann::json& json, ImageOptions& out)
    {
        from_string(json.at("ChannelDisplay").get<std::string>(), out.channelDisplay);
        from_string(json.at("VideoLevels").get<std::string>(), out.videoLevels);
        from_string(json.at("AlphaBlend").get<std::string>(), out.alphaBlend);
        json.at("ImageFilters").get_to(out.imageFilters);
        json.at("Cache").get_to(out.cache);
    }
}
