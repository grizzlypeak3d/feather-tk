// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/CmdLine.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/IApp.h>

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
        void iApp(nb::module_& m)
        {
            nb::class_<IApp>(m, "IApp")
                .def_prop_ro("exeName", &IApp::getExeName)
                .def_prop_ro("context", &IApp::getContext)
                .def_prop_ro("name", &IApp::getName)
                .def_prop_ro("summary", &IApp::getSummary)
                .def_prop_ro("hasCmdLineHelp", &IApp::hasCmdLineHelp);
        }
    }
}
