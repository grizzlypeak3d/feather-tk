// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Size.h>

#include <ftk/Core/String.h>

#include <sstream>

namespace ftk
{
    std::string to_string(const Size2I& value)
    {
        std::stringstream ss;
        ss << value.w << " " << value.h;
        return ss.str();
    }

    std::string to_string(const Size2F& value)
    {
        std::stringstream ss;
        ss << value.w << " " << value.h;
        return ss.str();
    }

    std::string to_string(const Size3F& value)
    {
        std::stringstream ss;
        ss << value.w << " " << value.h << " " << value.d;
        return ss.str();
    }

    bool from_string(const std::string& s, Size2I& value)
    {
        bool out = false;
        const auto pieces = split(s, ' ');
        if (2 == pieces.size())
        {
            value.w = std::atoi(pieces[0].c_str());
            value.h = std::atoi(pieces[1].c_str());
            out = true;
        }
        return out;
    }

    bool from_string(const std::string& s, Size2F& value)
    {
        bool out = false;
        const auto pieces = split(s, ' ');
        if (2 == pieces.size())
        {
            value.w = std::atof(pieces[0].c_str());
            value.h = std::atof(pieces[1].c_str());
            out = true;
        }
        return out;
    }

    bool from_string(const std::string& s, Size3F& value)
    {
        bool out = false;
        const auto pieces = split(s, ' ');
        if (3 == pieces.size())
        {
            value.w = std::atof(pieces[0].c_str());
            value.h = std::atof(pieces[1].c_str());
            value.d = std::atof(pieces[2].c_str());
            out = true;
        }
        return out;
    }

    void to_json(nlohmann::json& json, const Size2I& value)
    {
        json = { value.w, value.h };
    }

    void to_json(nlohmann::json& json, const Size2F& value)
    {
        json = { value.w, value.h };
    }

    void to_json(nlohmann::json& json, const Size3F& value)
    {
        json = { value.w, value.h , value.d };
    }

    void from_json(const nlohmann::json& json, Size2I& value)
    {
        json.at(0).get_to(value.w);
        json.at(1).get_to(value.h);
    }

    void from_json(const nlohmann::json& json, Size2F& value)
    {
        json.at(0).get_to(value.w);
        json.at(1).get_to(value.h);
    }

    void from_json(const nlohmann::json& json, Size3F& value)
    {
        json.at(0).get_to(value.w);
        json.at(1).get_to(value.h);
        json.at(2).get_to(value.d);
    }
}
