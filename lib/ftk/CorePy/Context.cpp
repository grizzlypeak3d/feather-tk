// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Context.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/ISystem.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void context(py::module_& m)
        {
            py::class_<Context, std::shared_ptr<Context> >(m, "Context")
                .def(py::init(&Context::create))
                .def("addSystem", &Context::addSystem)
                .def_property_readonly("systems", &Context::getSystems)
                .def("getSystemByName", &Context::getSystemByName)
                .def("tick", &Context::tick);
        }
    }
}
