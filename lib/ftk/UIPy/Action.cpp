// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Action.h>

#include <ftk/UI/Action.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void action(py::module_& m)
        {
            py::class_<Action, std::shared_ptr<Action> >(m, "Action")
                .def(
                    py::init(py::overload_cast<
                        const std::string&,
                        const std::function<void(void)>&>(&Action::create)),
                    py::arg("text"),
                    py::arg("callback"))
                .def(
                    py::init(py::overload_cast<
                        const std::string&,
                        const std::string&,
                        const std::function<void(void)>&>(&Action::create)),
                    py::arg("text"),
                    py::arg("icon"),
                    py::arg("callback"))
                .def(
                    py::init(py::overload_cast<
                        const std::string&,
                        Key,
                        int,
                        const std::function<void(void)>&>(&Action::create)),
                    py::arg("text"),
                    py::arg("shortcut"),
                    py::arg("shortcutModifiers"),
                    py::arg("callback"))
                .def(
                    py::init(py::overload_cast<
                        const std::string&,
                        const std::string&,
                        Key,
                        int,
                        const std::function<void(void)>&>(&Action::create)),
                    py::arg("text"),
                    py::arg("icon"),
                    py::arg("shortcut"),
                    py::arg("shortcutModifiers"),
                    py::arg("callback"))
                .def(
                    py::init(py::overload_cast<
                        const std::string&,
                        const std::function<void(bool)>&>(&Action::create)),
                    py::arg("text"),
                    py::arg("checkedCallback"))
                .def(
                    py::init(py::overload_cast<
                        const std::string&,
                        const std::string&,
                        const std::function<void(bool)>&>(&Action::create)),
                    py::arg("text"),
                    py::arg("icon"),
                    py::arg("checkedCallback"))
                .def(
                    py::init(py::overload_cast<
                        const std::string&,
                        Key,
                        int,
                        const std::function<void(bool)>&>(&Action::create)),
                    py::arg("text"),
                    py::arg("shortcut"),
                    py::arg("shortcutModifiers"),
                    py::arg("checkedCallback"))
                .def(
                    py::init(py::overload_cast<
                        const std::string&,
                        const std::string&,
                        Key,
                        int,
                        const std::function<void(bool)>&>(&Action::create)),
                    py::arg("text"),
                    py::arg("icon"),
                    py::arg("shortcut"),
                    py::arg("shortcutModifiers"),
                    py::arg("checkedCallback"))
                .def_property("text", &Action::getText, &Action::setText)
                .def_property_readonly("observeText", &Action::observeText)
                .def_property("icon", &Action::getIcon, &Action::setIcon)
                .def_property_readonly("observeIcon", &Action::observeIcon)
                .def_property("checkedIcon", &Action::getCheckedIcon, &Action::setCheckedIcon)
                .def_property_readonly("observeCheckedIcon", &Action::observeCheckedIcon)
                .def_property("shortcut", &Action::getShortcut, &Action::setShortcut)
                .def_property("shortcutModifiers", &Action::getShortcutModifiers, &Action::setShortcutModifiers)
                .def("doCallback", &Action::doCallback)
                .def_property("checkable", &Action::isCheckable, &Action::setCheckable)
                .def_property("checked", &Action::isChecked, &Action::setChecked)
                .def_property_readonly("observeCheckable", &Action::observeCheckable)
                .def_property_readonly("observeChecked", &Action::observeChecked)
                .def("doCheckedCallback", &Action::doCheckedCallback)
                .def_property("enabled", &Action::isEnabled, &Action::setEnabled)
                .def_property_readonly("observeEnabled", &Action::observeEnabled)
                .def_property("tooltip", &Action::getTooltip, &Action::setTooltip)
                .def_property_readonly("observeTooltip", &Action::observeTooltip);
        }
    }
}
