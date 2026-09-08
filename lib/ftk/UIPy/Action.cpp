// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/Action.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/function.h>
#include <nanobind/stl/string.h>
#include <nanobind/stl/vector.h>
#include <nanobind/stl/list.h>
#include <nanobind/stl/map.h>
#include <nanobind/stl/pair.h>
#include <nanobind/stl/optional.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/filesystem.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void action(nb::module_& m)
        {
            nb::class_<KeyShortcut>(m, "KeyShortcut")
                .def(nb::init<>())
                .def(nb::init<Key, KeyModifier>(), nb::arg("key"), nb::arg("modifier"))
                .def(
                    nb::init<Key, KeyModifier, KeyModifier>(),
                    nb::arg("key"),
                    nb::arg("modifier"),
                    nb::arg("modifier2"))
                .def(
                    nb::init<Key, KeyModifier, KeyModifier, KeyModifier>(),
                    nb::arg("key"),
                    nb::arg("modifier"),
                    nb::arg("modifier2"),
                    nb::arg("modifier3"))
                .def(nb::init<Key, int>(), nb::arg("key"), nb::arg("modifiers") = 0)
                .def_rw("key", &KeyShortcut::key)
                .def_rw("modifiers", &KeyShortcut::modifiers);

            nb::class_<Action>(m, "Action")
                .def(
                    nb::new_(nb::overload_cast<
                        const std::string&,
                        const std::function<void(void)>&>(&Action::create)),
                    nb::arg("text"),
                    nb::arg("callback"))
                .def(
                    nb::new_(nb::overload_cast<
                        const std::string&,
                        const std::string&,
                        const std::function<void(void)>&>(&Action::create)),
                    nb::arg("text"),
                    nb::arg("icon"),
                    nb::arg("callback"))
                .def(
                    nb::new_(nb::overload_cast<
                        const std::string&,
                        const KeyShortcut&,
                        const std::function<void(void)>&>(&Action::create)),
                    nb::arg("text"),
                    nb::arg("shortcut"),
                    nb::arg("callback"))
                .def(
                    nb::new_(nb::overload_cast<
                        const std::string&,
                        const std::string&,
                        const KeyShortcut&,
                        const std::function<void(void)>&>(&Action::create)),
                    nb::arg("text"),
                    nb::arg("icon"),
                    nb::arg("shortcut"),
                    nb::arg("callback"))
                .def(
                    nb::new_(nb::overload_cast<
                        const std::string&,
                        const std::function<void(bool)>&>(&Action::create)),
                    nb::arg("text"),
                    nb::arg("checkedCallback"))
                .def(
                    nb::new_(nb::overload_cast<
                        const std::string&,
                        const std::string&,
                        const std::function<void(bool)>&>(&Action::create)),
                    nb::arg("text"),
                    nb::arg("icon"),
                    nb::arg("checkedCallback"))
                .def(
                    nb::new_(nb::overload_cast<
                        const std::string&,
                        const KeyShortcut&,
                        const std::function<void(bool)>&>(&Action::create)),
                    nb::arg("text"),
                    nb::arg("shortcut"),
                    nb::arg("checkedCallback"))
                .def(
                    nb::new_(nb::overload_cast<
                        const std::string&,
                        const std::string&,
                        const KeyShortcut&,
                        const std::function<void(bool)>&>(&Action::create)),
                    nb::arg("text"),
                    nb::arg("icon"),
                    nb::arg("shortcut"),
                    nb::arg("checkedCallback"))
                .def_prop_rw("text", &Action::getText, &Action::setText)
                .def_prop_ro("observeText", &Action::observeText)
                .def_prop_rw("icon", &Action::getIcon, &Action::setIcon)
                .def_prop_ro("observeIcon", &Action::observeIcon)
                .def_prop_rw("checkedIcon", &Action::getCheckedIcon, &Action::setCheckedIcon)
                .def_prop_ro("observeCheckedIcon", &Action::observeCheckedIcon)
                .def_prop_rw("shortcuts", &Action::getShortcuts, &Action::setShortcuts, nb::rv_policy::copy)
                .def("doCallback", &Action::doCallback)
                .def_prop_rw("checkType", &Action::getCheckType, &Action::setCheckType)
                .def_prop_ro("checkable", &Action::isCheckable)
                .def_prop_rw("checked", &Action::isChecked, &Action::setChecked)
                .def_prop_ro("observeCheckType", &Action::observeCheckType)
                .def_prop_ro("observeChecked", &Action::observeChecked)
                .def("doCheckedCallback", &Action::doCheckedCallback)
                .def_prop_rw("enabled", &Action::isEnabled, &Action::setEnabled)
                .def_prop_ro("observeEnabled", &Action::observeEnabled)
                .def_prop_rw("tooltip", &Action::getTooltip, &Action::setTooltip)
                .def_prop_ro("observeTooltip", &Action::observeTooltip);
        }
    }
}
