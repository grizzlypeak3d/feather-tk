// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

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
        class PyIApp : public IApp
        {
        public:
            virtual void run() override
            {
                PYBIND11_OVERRIDE(void, IApp, run);
            }
        };
        
        void iApp(py::module_& m)
        {
            py::class_<IApp, std::shared_ptr<IApp>, PyIApp>(m, "IApp")
                .def_property_readonly("exeName", &IApp::getExeName)
                .def_property_readonly("exitValue", &IApp::getExit)
                .def_property_readonly("context", &IApp::getContext)
                .def_property_readonly("name", &IApp::getName)
                .def_property_readonly("summary", &IApp::getSummary)
                .def("run", &IApp::run);
        }
    }
}
