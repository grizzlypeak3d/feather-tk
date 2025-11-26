// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/CmdLine.h>

#include <ftk/Core/CmdLine.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void cmdLine(py::module_& m)
        {
            py::class_<ICmdLineOption, std::shared_ptr<ICmdLineOption> >(m, "ICmdLineOption");
            py::class_<ICmdLineArg, std::shared_ptr<ICmdLineArg> >(m, "ICmdLineArg");
        }
    }
}
