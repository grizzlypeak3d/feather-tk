// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Settings.h>

#include <ftk/UI/Settings.h>

#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/stl/filesystem.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void settings(py::module_& m)
        {
            m.def(
                "getSettingsPath",
                &getSettingsPath,
                py::arg("directory"),
                py::arg("fileName"),
                "Get a settings path.");

            py::class_<Settings, std::shared_ptr<Settings> >(m, "Settings")
                .def(
                    py::init(&Settings::create),
                    py::arg("context"),
                    py::arg("path"),
                    py::arg("reset") = false)
                .def_property_readonly("path", &Settings::getPath)
                .def("save", &Settings::save)
                .def("contains", &Settings::contains, py::arg("key"))
                .def_property("bool",
                    [](Settings& settings, const std::string& key)
                    {
                        bool value = false;
                        return py::make_tuple(settings.get(key, value), value);
                    },
                    [](Settings& settings, const std::string& key, bool value)
                    {
                        settings.set(key, value);
                    })
                .def_property("int",
                    [](Settings& settings, const std::string& key)
                    {
                        int64_t value = 0;
                        return py::make_tuple(settings.get(key, value), value);
                    },
                    [](Settings& settings, const std::string& key, int value)
                    {
                        settings.set(key, value);
                    })
                .def_property("double",
                    [](Settings& settings, const std::string& key)
                    {
                        double value = 0.0;
                        return py::make_tuple(settings.get(key, value), value);
                    },
                    [](Settings& settings, const std::string& key, double value)
                    {
                        settings.set(key, value);
                    })
                .def_property("string",
                    [](Settings& settings, const std::string& key)
                    {
                        std::string value;
                        return py::make_tuple(settings.get(key, value), value);
                    },
                    [](Settings& settings, const std::string& key, const std::string& value)
                    {
                        settings.set(key, value);
                    })
                .def_property("stringList",
                    [](Settings& settings, const std::string& key)
                    {
                        std::vector<std::string> value;
                        return py::make_tuple(settings.get(key, value), value);
                    },
                    [](Settings& settings, const std::string& key, const std::vector<std::string>& value)
                    {
                        settings.set(key, value);
                    })
                .def_property("json",
                    [](Settings& settings, const std::string& key)
                    {
                        nlohmann::json json;
                        return py::make_tuple(settings.get(key, json), json.dump());
                    },
                    [](Settings& settings, const std::string& key, const std::string& value)
                    {
                        settings.set(key, nlohmann::json().parse(value));
                    });
        }
    }
}
