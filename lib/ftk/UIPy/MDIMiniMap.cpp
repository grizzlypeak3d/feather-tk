// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/MDIMiniMap.h>

#include <pybind11/pybind11.h>
#include <pybind11/functional.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void mdiMiniMap(py::module_& m)
        {
            py::class_<MDIMiniMap, IWidget, std::shared_ptr<MDIMiniMap> >(m, "MDIMiniMap")
                .def(
                    py::init(&MDIMiniMap::create),
                    py::arg("context"),
                    py::arg("parent") = nullptr)
                .def("setScrollInfo", &MDIMiniMap::setScrollInfo)
                .def("setCallback", &MDIMiniMap::setCallback)
                .def("setChildGeometry", &MDIMiniMap::setChildGeometry)
                .def_property("childColorRole", &MDIMiniMap::getChildColorRole, &MDIMiniMap::setChildColorRole);
        }
    }
}
