// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/OS.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void os(py::module_& m)
        {
            py::class_<SysInfo>(m, "SystInfo")
                .def_readwrite("name", &SysInfo::name)
                .def_readwrite("cores", &SysInfo::cores)
                .def_readwrite("ram", &SysInfo::ram)
                .def_readwrite("ramGB", &SysInfo::ramGB);

            m.def("getSystInfo", &getSysInfo, "Get operating system information.");
        }
    }
}
