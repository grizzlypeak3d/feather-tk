// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/ISystem.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/chrono.h>
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
        void iSystem(nb::module_& m)
        {
            nb::class_<IBaseSystem>(m, "IBaseSystem")
                .def_prop_ro("context", &ISystem::getContext)
                .def_prop_ro("name", &ISystem::getName)
                .def("tick", &ISystem::tick)
                .def_prop_ro("tickTime", &ISystem::getTickTime);

            nb::class_<ISystem, IBaseSystem>(m, "ISystem");
        }
    }
}
