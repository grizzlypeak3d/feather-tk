// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IButton.h>

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
        void iButton(nb::module_& m)
        {
            nb::class_<IButton, IWidget>(m, "IButton")
                .def_prop_rw("checkable", &IButton::isCheckable, &IButton::setCheckable)
                .def_prop_rw("checked", &IButton::isChecked, &IButton::setChecked)
                .def_prop_rw("text", &IButton::getText, &IButton::setText)
                .def_prop_rw("font", &IButton::getFont, &IButton::setFont)
                .def_prop_rw("icon", &IButton::getIcon, &IButton::setIcon)
                .def_prop_rw("checkedIcon", &IButton::getCheckedIcon, &IButton::setCheckedIcon)
                .def_prop_rw("buttonRole", &IButton::getButtonRole, &IButton::setButtonRole)
                .def_prop_rw("checkedRole", &IButton::getCheckedRole, &IButton::setCheckedRole)
                .def("setHoveredCallback", &IButton::setHoveredCallback)
                .def("setPressedCallback", &IButton::setPressedCallback)
                .def_prop_rw("repeatClick", &IButton::hasRepeatClick, &IButton::setRepeatClick)
                .def("setClickedCallback", &IButton::setClickedCallback)
                .def("setCheckedCallback", &IButton::setCheckedCallback);
        }
    }
}
