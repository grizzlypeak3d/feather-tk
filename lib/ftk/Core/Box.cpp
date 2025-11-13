// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Box.h>

#include <ftk/Core/String.h>

#include <sstream>

namespace ftk
{
    std::string to_string(const Box2I& value)
    {
        std::stringstream ss;
        ss << value.min.x << " " <<
            value.min.y << " " <<
            value.max.x << " " <<
            value.max.y;
        return ss.str();
    }

    std::string to_string(const Box2F& value)
    {
        std::stringstream ss;
        ss << value.min.x << " " << 
            value.min.y << " " <<
            value.max.x << " " <<
            value.max.y;
        return ss.str();
    }

    std::string to_string(const Box3F& value)
    {
        std::stringstream ss;
        ss << value.min.x << " " <<
            value.min.y << " " <<
            value.min.z << " " <<
            value.max.x << " " <<
            value.max.y << " " <<
            value.max.z;
        return ss.str();
    }

    bool from_string(const std::string& s, Box2I& value)
    {
        bool out = false;
        const auto pieces = split(s, ' ');
        if (4 == pieces.size())
        {
            value.min.x = std::atoi(pieces[0].c_str());
            value.min.y = std::atoi(pieces[1].c_str());
            value.max.x = std::atoi(pieces[2].c_str());
            value.max.y = std::atoi(pieces[3].c_str());
            out = true;
        }
        return out;
    }

    bool from_string(const std::string& s, Box2F& value)
    {
        bool out = false;
        const auto pieces = split(s, ' ');
        if (4 == pieces.size())
        {
            value.min.x = std::atof(pieces[0].c_str());
            value.min.y = std::atof(pieces[1].c_str());
            value.max.x = std::atof(pieces[2].c_str());
            value.max.y = std::atof(pieces[3].c_str());
            out = true;
        }
        return out;
    }

    bool from_string(const std::string& s, Box3F& value)
    {
        bool out = false;
        const auto pieces = split(s, ' ');
        if (6 == pieces.size())
        {
            value.min.x = std::atof(pieces[0].c_str());
            value.min.y = std::atof(pieces[1].c_str());
            value.min.z = std::atof(pieces[2].c_str());
            value.max.x = std::atof(pieces[3].c_str());
            value.max.y = std::atof(pieces[4].c_str());
            value.max.z = std::atof(pieces[5].c_str());
            out = true;
        }
        return out;
    }

    void to_json(nlohmann::json& json, const Box2I& value)
    {
        json = { value.min, value.max };
    }

    void to_json(nlohmann::json& json, const Box2F& value)
    {
        json = { value.min, value.max };
    }

    void to_json(nlohmann::json& json, const Box3F& value)
    {
        json = { value.min, value.max };
    }

    void from_json(const nlohmann::json& json, Box2I& value)
    {
        json.at(0).get_to(value.min);
        json.at(1).get_to(value.max);
    }

    void from_json(const nlohmann::json& json, Box2F& value)
    {
        json.at(0).get_to(value.min);
        json.at(1).get_to(value.max);
    }

    void from_json(const nlohmann::json& json, Box3F& value)
    {
        json.at(0).get_to(value.min);
        json.at(1).get_to(value.max);
    }
}
