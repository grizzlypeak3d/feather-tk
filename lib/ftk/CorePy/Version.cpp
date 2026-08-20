// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/Version.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void version(py::module_& m)
        {
            m.attr("VERSION_MAJOR") = FTK_VERSION_MAJOR;
            m.attr("VERSION_MINOR") = FTK_VERSION_MINOR;
            m.attr("VERSION_PATCH") = FTK_VERSION_PATCH;
            m.attr("VERSION_DEV") = FTK_VERSION_DEV;
            m.attr("VERSION_FULL") = FTK_VERSION_FULL;
        }
    }
}
