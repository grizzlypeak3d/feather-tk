// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Matrix.h>

#include <nlohmann/json.hpp>

namespace ftk
{
    void to_json(nlohmann::json& json, const M33F& value)
    {
        json =
        {
            value.get(0, 0), value.get(0, 1), value.get(0, 2),
            value.get(1, 0), value.get(1, 1), value.get(1, 2),
            value.get(2, 0), value.get(2, 1), value.get(2, 2)
        };
    }

    void to_json(nlohmann::json& json, const M44F& value)
    {
        json =
        {
            value.get(0, 0), value.get(0, 1), value.get(0, 2), value.get(0, 3),
            value.get(1, 0), value.get(1, 1), value.get(1, 2), value.get(1, 3),
            value.get(2, 0), value.get(2, 1), value.get(2, 2), value.get(2, 3),
            value.get(3, 0), value.get(3, 1), value.get(3, 2), value.get(3, 3)
        };
    }

    void from_json(const nlohmann::json& json, M33F& value)
    {
        for (int i = 0; i < 9; ++i)
        {
            value[i] = json.at(i).get<float>();
        }
    }

    void from_json(const nlohmann::json& json, M44F& value)
    {
        for (int i = 0; i < 16; ++i)
        {
            value[i] = json.at(i).get<float>();
        }
    }
}
