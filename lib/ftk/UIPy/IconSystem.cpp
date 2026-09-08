// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/IconSystem.h>

#include <nanobind/nanobind.h>
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
        void iconSystem(nb::module_& m)
        {
            nb::class_<IconSystem, ISystem>(m, "IconSystem")
                .def(
                    nb::new_(&IconSystem::create),
                    nb::arg("context"))
                .def_prop_ro("names", &IconSystem::getNames)
                .def(
                    "add",
                    &IconSystem::add,
                    nb::arg("name"),
                    nb::arg("svg"))
                .def(
                    "get",
                    &IconSystem::get,
                    nb::arg("name"),
                    nb::arg("displayScale"));
        }
    }
}
