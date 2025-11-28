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
                .def("get", py::overload_cast<const std::string&, bool&>(&Settings::get), py::arg("key"), py::arg("value"))
                .def("get", py::overload_cast<const std::string&, int64_t&>(&Settings::get), py::arg("key"), py::arg("value"))
                .def("get", py::overload_cast<const std::string&, double&>(&Settings::get), py::arg("key"), py::arg("value"))
                .def("get", py::overload_cast<const std::string&, std::string&>(&Settings::get), py::arg("key"), py::arg("value"))
                .def("get", py::overload_cast<const std::string&, std::vector<std::string>&>(&Settings::get), py::arg("key"), py::arg("value"))
                .def("set", py::overload_cast<const std::string&, bool>(&Settings::set), py::arg("key"), py::arg("value"))
                .def("set", py::overload_cast<const std::string&, int64_t>(&Settings::set), py::arg("key"), py::arg("value"))
                .def("set", py::overload_cast<const std::string&, double>(&Settings::set), py::arg("key"), py::arg("value"))
                .def("set", py::overload_cast<const std::string&, const std::string&>(&Settings::set), py::arg("key"), py::arg("value"))
                .def("set", py::overload_cast<const std::string&, const std::vector<std::string>&>(&Settings::set), py::arg("key"), py::arg("value"));
        }
    }
}
