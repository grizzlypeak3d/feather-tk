// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/UI/SysLogModel.h>

#include <ftk/Core/Context.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void sysLogModel(py::module_& m)
        {
            py::class_<SysLogModel, std::shared_ptr<SysLogModel> >(m, "SysLogModel")
                .def(
                    py::init(&SysLogModel::create),
                    py::arg("context"))
                .def("observeMessages", &SysLogModel::observeMessages)
                .def("clearMessages", &SysLogModel::clearMessages)
                .def("observeLog", &SysLogModel::observeLog)
                .def("clearLog", &SysLogModel::clearLog);
        }
    }
}
