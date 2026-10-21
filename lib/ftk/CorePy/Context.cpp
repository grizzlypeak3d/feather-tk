// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Context.h>
#include <ftk/Core/IBaseSystem.h>

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
        void context(nb::module_& m)
        {
            nb::class_<Context>(
                m, "Context",
                "The systems an application runs on: logging, fonts, I/O, "
                "audio and whatever else is added. Everything made from a "
                "context is updated by tick().")
                .def(nb::new_(&Context::create))
                .def("addSystem", &Context::addSystem)
                .def_prop_ro("systems", &Context::getSystems)
                .def("getSystemByName", &Context::getSystemByName)
                .def(
                    "tick",
                    &Context::tick,
                    "Run the systems once: results from other threads are "
                    "taken in, a player's current frame and time move on, and "
                    "observers are called. Nothing changes between calls, so "
                    "a script without an application's event loop calls this "
                    "in its own loop, for as long as it is waiting for "
                    "something.");
        }
    }
}
