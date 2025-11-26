// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/IApp.h>

#include <ftk/Core/CmdLine.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/IApp.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void iApp(py::module_& m)
        {
            py::class_<IApp, std::shared_ptr<IApp> >(m, "IApp")
                .def("getExeName", &IApp::getExeName)
                .def("getExit", &IApp::getExit)
                .def("getContext", &IApp::getContext)
                .def("getName", &IApp::getName)
                .def("getSummary", &IApp::getSummary)
                .def("run", &IApp::run);
        }
    }
}
