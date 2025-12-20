// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/ISystem.h>

#include <pybind11/pybind11.h>
#include <pybind11/chrono.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void iSystem(py::module_& m)
        {
            py::class_<IBaseSystem, std::shared_ptr<IBaseSystem> >(m, "IBaseSystem")
                .def_property_readonly("context", &ISystem::getContext)
                .def_property_readonly("name", &ISystem::getName)
                .def("tick", &ISystem::tick)
                .def_property_readonly("tickTime", &ISystem::getTickTime);

            py::class_<ISystem, IBaseSystem, std::shared_ptr<ISystem> >(m, "ISystem");
        }
    }
}
