// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Settings.h>

#include <ftk/Core/Context.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/filesystem.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void settings(nb::module_& m)
        {
            m.def(
                "getSettingsPath",
                &getSettingsPath,
                nb::arg("directory"),
                nb::arg("fileName"),
                "Get a settings path.");

            nb::class_<Settings>(m, "Settings")
                .def(
                    nb::new_(&Settings::create),
                    nb::arg("context"),
                    nb::arg("path"),
                    nb::arg("reset") = false)
                .def_prop_ro("path", &Settings::getPath)
                .def("save", &Settings::save)
                .def("contains", &Settings::contains, nb::arg("key"))
                .def("getBool",
                    [](Settings& settings, const std::string& key)
                    {
                        bool value = false;
                        return nb::make_tuple(settings.get(key, value), value);
                    })
                .def("getInt",
                    [](Settings& settings, const std::string& key)
                    {
                        int64_t value = 0;
                        return nb::make_tuple(settings.get(key, value), value);
                    })
                .def("getDouble",
                    [](Settings& settings, const std::string& key)
                    {
                        double value = 0.0;
                        return nb::make_tuple(settings.get(key, value), value);
                    })
                .def("getString",
                    [](Settings& settings, const std::string& key)
                    {
                        std::string value;
                        return nb::make_tuple(settings.get(key, value), value);
                    })
                .def("getStringList",
                    [](Settings& settings, const std::string& key)
                    {
                        std::vector<std::string> value;
                        return nb::make_tuple(settings.get(key, value), value);
                    })
                .def("getJSON",
                    [](Settings& settings, const std::string& key)
                    {
                        nlohmann::json json;
                        bool out = settings.get(key, json);
                        return nb::make_tuple(out, json.dump());
                    })
                .def("setBool", nb::overload_cast<const std::string&, bool>(&Settings::set), nb::arg("key"), nb::arg("value"))
                .def("setInt", nb::overload_cast<const std::string&, int64_t>(&Settings::set), nb::arg("key"), nb::arg("value"))
                .def("setDouble", nb::overload_cast<const std::string&, double>(&Settings::set), nb::arg("key"), nb::arg("value"))
                .def("setString", nb::overload_cast<const std::string&, const std::string&>(&Settings::set), nb::arg("key"), nb::arg("value"))
                .def("setStringList", nb::overload_cast<const std::string&, const std::vector<std::string>&>(&Settings::set), nb::arg("key"), nb::arg("value"))
                .def("setJSON",
                    [](Settings& settings, const std::string& key, const std::string& value)
                    {
                        settings.set(key, nlohmann::json().parse(value));
                    });
        }
    }
}
