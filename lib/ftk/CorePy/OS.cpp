// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/OS.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void os(nb::module_& m)
        {
            m.def(
                "openURL",
                &openURL,
                nb::arg("url"),
                "Open a URL in an external application.");
            nb::class_<SysInfo>(m, "SystInfo")
                .def_rw("name", &SysInfo::name)
                .def_rw("cores", &SysInfo::cores)
                .def_rw("ram", &SysInfo::ram)
                .def_rw("ramGB", &SysInfo::ramGB);

            m.def("getSystInfo", &getSysInfo, "Get operating system information.");
        }
    }
}
