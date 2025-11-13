// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Vector.h>

#include <ftk/Core/String.h>

#include <sstream>

namespace ftk
{
    std::string to_string(const V2I& value)
    {
        std::stringstream ss;
        ss << value.x << " " << value.y;
        return ss.str();
    }

    std::string to_string(const V2F& value)
    {
        std::stringstream ss;
        ss << value.x << " " << value.y;
        return ss.str();
    }

    std::string to_string(const V3F& value)
    {
        std::stringstream ss;
        ss << value.x << " " << value.y << " " << value.z;
        return ss.str();
    }

    std::string to_string(const V4F& value)
    {
        std::stringstream ss;
        ss << value.x << " " << value.y << " " << value.z << " " << value.w;
        return ss.str();
    }

    bool from_string(const std::string& s, V2I& value)
    {
        bool out = false;
        const auto pieces = split(s, ' ');
        if (2 == pieces.size())
        {
            value.x = std::atoi(pieces[0].c_str());
            value.y = std::atoi(pieces[1].c_str());
            out = true;
        }
        return out;
    }

    bool from_string(const std::string& s, V2F& value)
    {
        bool out = false;
        const auto pieces = split(s, ' ');
        if (2 == pieces.size())
        {
            value.x = std::atof(pieces[0].c_str());
            value.y = std::atof(pieces[1].c_str());
            out = true;
        }
        return out;
    }

    bool from_string(const std::string& s, V3F& value)
    {
        bool out = false;
        const auto pieces = split(s, ' ');
        if (3 == pieces.size())
        {
            value.x = std::atof(pieces[0].c_str());
            value.y = std::atof(pieces[1].c_str());
            value.z = std::atof(pieces[2].c_str());
            out = true;
        }
        return out;
    }

    bool from_string(const std::string& s, V4F& value)
    {
        bool out = false;
        const auto pieces = split(s, ' ');
        if (4 == pieces.size())
        {
            value.x = std::atof(pieces[0].c_str());
            value.y = std::atof(pieces[1].c_str());
            value.z = std::atof(pieces[2].c_str());
            value.w = std::atof(pieces[3].c_str());
            out = true;
        }
        return out;
    }

    void to_json(nlohmann::json& json, const V2I& value)
    {
        json = { value.x, value.y };
    }

    void to_json(nlohmann::json& json, const V2F& value)
    {
        json = { value.x, value.y };
    }

    void to_json(nlohmann::json& json, const V3F& value)
    {
        json = { value.x, value.y, value.z };
    }

    void to_json(nlohmann::json& json, const V4F& value)
    {
        json = { value.x, value.y, value.z, value.w };
    }

    void from_json(const nlohmann::json& json, V2I& value)
    {
        json.at(0).get_to(value.x);
        json.at(1).get_to(value.y);
    }

    void from_json(const nlohmann::json& json, V2F& value)
    {
        json.at(0).get_to(value.x);
        json.at(1).get_to(value.y);
    }

    void from_json(const nlohmann::json& json, V3F& value)
    {
        json.at(0).get_to(value.x);
        json.at(1).get_to(value.y);
        json.at(2).get_to(value.z);
    }

    void from_json(const nlohmann::json& json, V4F& value)
    {
        json.at(0).get_to(value.x);
        json.at(1).get_to(value.y);
        json.at(2).get_to(value.z);
        json.at(3).get_to(value.w);
    }
}
