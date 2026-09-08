// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/ActionGroup.h>

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
        void actionGroup(nb::module_& m)
        {
            FTK_ENUM_PY(m, ActionGroupType);
            FTK_ENUM_BIND(m, ActionGroupType);

            nb::class_<ActionGroup>(m, "ActionGroup")
                .def(
                    nb::new_(&ActionGroup::create),
                    nb::arg("type"))
                .def_prop_ro("actions", &ActionGroup::getActions)
                .def("addAction", &ActionGroup::addAction)
                .def("clear", &ActionGroup::clear)
                .def_prop_rw("checked", &ActionGroup::getChecked, &ActionGroup::setChecked)
                .def("observeChecked", &ActionGroup::observeChecked)
                .def("setCheckedCallback", &ActionGroup::setCheckedCallback);
        }
    }
}
