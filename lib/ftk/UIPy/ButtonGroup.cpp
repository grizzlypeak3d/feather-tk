// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/ButtonGroup.h>

#include <ftk/CorePy/Util.h>

#include <ftk/UI/ButtonGroup.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void buttonGroup(py::module_& m)
        {
            py::enum_<ButtonGroupType>(m, "ButtonGroupType")
                .value("Click", ButtonGroupType::Click)
                .value("Check", ButtonGroupType::Check)
                .value("Radio", ButtonGroupType::Radio)
                .value("Toggle", ButtonGroupType::Toggle);
            FTK_ENUM_BIND(ButtonGroupType);

            py::class_<ButtonGroup, std::shared_ptr<ButtonGroup> >(m, "ButtonGroup")
                .def(
                    py::init(&ButtonGroup::create),
                    py::arg("context"),
                    py::arg("type"))
                .def("getButtons", &ButtonGroup::getButtons)
                .def("addButton", &ButtonGroup::addButton)
                .def("clearButtons", &ButtonGroup::clearButtons)
                .def(
                    "setChecked",
                    &ButtonGroup::setChecked,
                    py::arg("index"),
                    py::arg("checked"))
                .def("setClickedCallback", &ButtonGroup::setClickedCallback)
                .def("setCheckedCallback", &ButtonGroup::setCheckedCallback);
        }
    }
}
