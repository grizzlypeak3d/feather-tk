// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Color.h>

#include <ftk/Core/String.h>

#include <sstream>

namespace ftk
{
    M44F brightness(const V3F& value)
    {
        return M44F(
            value.x, 0.F, 0.F, 0.F,
            0.F, value.y, 0.F, 0.F,
            0.F, 0.F, value.z, 0.F,
            0.F, 0.F, 0.F, 1.F);
    }

    M44F contrast(const V3F& value)
    {
        return
            M44F(
                1.F, 0.F, 0.F, .5F,
                0.F, 1.F, 0.F, .5F,
                0.F, 0.F, 1.F, .5F,
                0.F, 0.F, 0.F, 1.F) *
            M44F(
                value.x, 0.F, 0.F, 0.F,
                0.F, value.y, 0.F, 0.F,
                0.F, 0.F, value.z, 0.F,
                0.F, 0.F, 0.F, 1.F) *
            M44F(
                1.F, 0.F, 0.F, -.5F,
                0.F, 1.F, 0.F, -.5F,
                0.F, 0.F, 1.F, -.5F,
                0.F, 0.F, 0.F, 1.F);
    }

    M44F saturation(const V3F& value)
    {
        const V3F s(
            (1.F - value.x) * .3086F,
            (1.F - value.y) * .6094F,
            (1.F - value.z) * .0820F);
        return M44F(
            s.x + value.x, s.x, s.x, 0.F,
            s.y, s.y + value.y, s.y, 0.F,
            s.z, s.z, s.z + value.z, 0.F,
            0.F, 0.F, 0.F, 1.F);
    }

    M44F tint(float v)
    {
        const float c = cos(v * pi * 2.F);
        const float c2 = 1.F - c;
        const float c3 = 1.F / 3.F * c2;
        const float s = sin(v * pi * 2.F);
        const float sq = sqrtf(1.F / 3.F);
        return M44F(
            c + c2 / 3.F, c3 - sq * s, c3 + sq * s, 0.F,
            c3 + sq * s, c + c3, c3 - sq * s, 0.F,
            c3 - sq * s, c3 + sq * s, c + c3, 0.F,
            0.F, 0.F, 0.F, 1.F);
    }

    void rgbToHSV(const float in[3], float out[3])
    {
        const float& min = std::min(in[0], std::min(in[1], in[2]));
        const float& max = std::max(in[0], std::max(in[1], in[2]));
        out[2] = max;
        out[1] = max != 0.F ? (1.F - min / max) : 0.F;
        const float v = (max - min) * 6.F;
        if (max == min)
        {
            out[0] = 0.F;
        }
        else if (in[0] == max)
        {
            if (in[1] >= in[2])
            {
                out[0] = (in[1] - in[2]) / v;
            }
            else
            {
                out[0] = 1.F + (in[1] - in[2]) / v;
            }
        }
        else if (in[1] == max)
        {
            out[0] = 1.F / 3.F + (in[2] - in[0]) / v;
        }
        else if (in[2] == max)
        {
            out[0] = 2.F / 3.F + (in[0] - in[1]) / v;
        }
    }

    namespace
    {
        float mod(float value, float mod)
        {
            float tmp = value;
            if (mod != 0.F)
            {
                tmp = value - (static_cast<float>(static_cast<int>(value / mod)) * mod);
                if (tmp < 0.F)
                {
                    tmp += mod;
                }
            }
            return tmp;
        }

    } // namespace

    void hsvToRGB(const float in[3], float out[3])
    {
        const float h = mod(in[0] * 6.F, 6.F);
        const int   i = static_cast<int>(floorf(h));
        const float f = h - static_cast<float>(i);
        const float p = in[2] * (1.F - in[1]);
        const float q = in[2] * (1.F - (in[1] * f));
        const float t = in[2] * (1.F - (in[1] * (1.F - f)));
        switch (i)
        {
        case 0:
            out[0] = in[2];
            out[1] = t;
            out[2] = p;
            break;
        case 1:
            out[0] = q;
            out[1] = in[2];
            out[2] = p;
            break;
        case 2:
            out[0] = p;
            out[1] = in[2];
            out[2] = t;
            break;
        case 3:
            out[0] = p;
            out[1] = q;
            out[2] = in[2];
            break;
        case 4:
            out[0] = t;
            out[1] = p;
            out[2] = in[2];
            break;
        case 5:
            out[0] = in[2];
            out[1] = p;
            out[2] = q;
            break;
        default: break;
        }
    }

    std::string to_string(const Color1F& value)
    {
        std::stringstream ss;
        ss << value.l;
        return ss.str();
    }

    std::string to_string(const Color2F& value)
    {
        std::stringstream ss;
        ss << value.l << " " << value.a;
        return ss.str();
    }

    std::string to_string(const Color3F& value)
    {
        std::stringstream ss;
        ss << value.r << " " << value.g << " " << value.b;
        return ss.str();
    }

    std::string to_string(const Color4F& value)
    {
        std::stringstream ss;
        ss << value.r << " " << value.g << " " << value.b << " " << value.a;
        return ss.str();
    }

    bool from_string(const std::string& s, Color1F& value)
    {
        bool out = false;
        if (!s.empty())
        {
            value.l = std::atof(s.c_str());
            out = true;
        }
        return out;
    }

    bool from_string(const std::string& s, Color2F& value)
    {
        bool out = false;
        const auto pieces = split(s, ' ');
        if (2 == pieces.size())
        {
            value.l = std::atof(pieces[0].c_str());
            value.a = std::atof(pieces[1].c_str());
            out = true;
        }
        return out;
    }

    bool from_string(const std::string& s, Color3F& value)
    {
        bool out = false;
        const auto pieces = split(s, ' ');
        if (3 == pieces.size())
        {
            value.r = std::atof(pieces[0].c_str());
            value.g = std::atof(pieces[1].c_str());
            value.b = std::atof(pieces[2].c_str());
            out = true;
        }
        return out;
    }

    bool from_string(const std::string& s, Color4F& value)
    {
        bool out = false;
        const auto pieces = split(s, ' ');
        if (4 == pieces.size())
        {
            value.r = std::atof(pieces[0].c_str());
            value.g = std::atof(pieces[1].c_str());
            value.b = std::atof(pieces[2].c_str());
            value.a = std::atof(pieces[3].c_str());
            out = true;
        }
        return out;
    }

    void to_json(nlohmann::json& json, const Color1F& value)
    {
        json = { value.l };
    }

    void to_json(nlohmann::json& json, const Color2F& value)
    {
        json = { value.l, value.a };
    }

    void to_json(nlohmann::json& json, const Color3F& value)
    {
        json = { value.r, value.g, value.b };
    }

    void to_json(nlohmann::json& json, const Color4F& value)
    {
        json = { value.r, value.g, value.b, value.a };
    }

    void from_json(const nlohmann::json& json, Color1F& value)
    {
        json.at(0).get_to(value.l);
    }

    void from_json(const nlohmann::json& json, Color2F& value)
    {
        json.at(0).get_to(value.l);
        json.at(1).get_to(value.a);
    }

    void from_json(const nlohmann::json& json, Color3F& value)
    {
        json.at(0).get_to(value.r);
        json.at(1).get_to(value.g);
        json.at(2).get_to(value.b);
    }

    void from_json(const nlohmann::json& json, Color4F& value)
    {
        json.at(0).get_to(value.r);
        json.at(1).get_to(value.g);
        json.at(2).get_to(value.b);
        json.at(3).get_to(value.a);
    }
}
