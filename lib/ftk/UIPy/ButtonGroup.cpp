// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/ButtonGroup.h>

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
        void buttonGroup(nb::module_& m)
        {
            nb::enum_<ButtonGroupType>(m, "ButtonGroupType")
                .value("Click", ButtonGroupType::Click)
                .value("Check", ButtonGroupType::Check)
                .value("Radio", ButtonGroupType::Radio)
                .value("Toggle", ButtonGroupType::Toggle);
            FTK_ENUM_BIND(m, ButtonGroupType);

            nb::class_<ButtonGroup>(m, "ButtonGroup")
                .def(
                    nb::new_(&ButtonGroup::create),
                    nb::arg("context"),
                    nb::arg("type"))
                .def("getButtons", &ButtonGroup::getButtons)
                .def("addButton", &ButtonGroup::addButton)
                .def("clearButtons", &ButtonGroup::clearButtons)
                .def(
                    "setChecked",
                    &ButtonGroup::setChecked,
                    nb::arg("index"),
                    nb::arg("checked"))
                .def("setClickedCallback", &ButtonGroup::setClickedCallback)
                .def("setCheckedCallback", &ButtonGroup::setCheckedCallback);
        }
    }
}
