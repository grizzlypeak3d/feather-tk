// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/MDIMiniMap.h>

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
        void mdiMiniMap(nb::module_& m)
        {
            nb::class_<MDIMiniMap, IWidget>(m, "MDIMiniMap")
                .def(
                    nb::new_(&MDIMiniMap::create),
                    nb::arg("context"),
                    nb::arg("parent") = nullptr)
                .def("setScrollInfo", &MDIMiniMap::setScrollInfo)
                .def("setCallback", &MDIMiniMap::setCallback)
                .def("setChildGeometry", &MDIMiniMap::setChildGeometry)
                .def_prop_rw("childColorRole", &MDIMiniMap::getChildColorRole, &MDIMiniMap::setChildColorRole);
        }
    }
}
