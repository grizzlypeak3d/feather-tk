// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

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
            
            py::class_<CmdLineFlagOption, ICmdLineOption, std::shared_ptr<CmdLineFlagOption> >(m, "CmdLineFlagOption")
                .def(
                    pybind11::init(&CmdLineFlagOption::create),
                    pybind11::arg("names"),
                    pybind11::arg("help"),
                    pybind11::arg("group") = std::string())
                .def_property_readonly("found", &CmdLineFlagOption::found);

            cmdLineValueOption<int>(m, "I");
            cmdLineValueOption<float>(m, "F");
            cmdLineValueOption<double>(m, "D");
            cmdLineValueOption<bool>(m, "Bool");
            cmdLineValueOption<std::string>(m, "String");

            py::class_<ICmdLineArg, std::shared_ptr<ICmdLineArg> >(m, "ICmdLineArg");

            cmdLineValueArg<int>(m, "I");
            cmdLineValueArg<float>(m, "F");
            cmdLineValueArg<double>(m, "D");
            cmdLineValueArg<bool>(m, "Bool");
            cmdLineValueArg<std::string>(m, "String");

            cmdLineListArg<std::string>(m, "String");
        }
    }
}
