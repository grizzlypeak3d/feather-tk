// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/SysLogModel.h>

#include <ftk/Core/Context.h>

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
        void sysLogModel(nb::module_& m)
        {
            nb::class_<SysLogModel>(m, "SysLogModel")
                .def(
                    nb::new_(&SysLogModel::create),
                    nb::arg("context"))
                .def("observeMessages", &SysLogModel::observeMessages)
                .def("clearMessages", &SysLogModel::clearMessages)
                .def("observeLog", &SysLogModel::observeLog)
                .def("clearLog", &SysLogModel::clearLog);
        }
    }
}
