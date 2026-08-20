// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/ActionGroup.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void actionGroup(py::module_& m)
        {
            py::enum_<ActionGroupType>(m, "ActionGroupType")
                .value("Click", ActionGroupType::Click)
                .value("Check", ActionGroupType::Check)
                .value("Radio", ActionGroupType::Radio)
                .value("Toggle", ActionGroupType::Toggle);
            FTK_ENUM_BIND(m, ActionGroupType);

            py::class_<ActionGroup, std::shared_ptr<ActionGroup> >(m, "ActionGroup")
                .def(
                    py::init(&ActionGroup::create),
                    py::arg("type"))
                .def_property_readonly("actions", &ActionGroup::getActions)
                .def("addAction", &ActionGroup::addAction)
                .def("clear", &ActionGroup::clear)
                .def_property("checked", &ActionGroup::getChecked, &ActionGroup::setChecked)
                .def("observeChecked", &ActionGroup::observeChecked)
                .def("setCheckedCallback", &ActionGroup::setCheckedCallback);
        }
    }
}
