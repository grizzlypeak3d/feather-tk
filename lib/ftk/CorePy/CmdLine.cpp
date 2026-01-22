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
            py::class_<ICmdLineOption, std::shared_ptr<ICmdLineOption> >(m, "ICmdLineOption")
                .def_property_readonly("help", &ICmdLineOption::getHelp)
                .def_property_readonly("group", &ICmdLineOption::getGroup)
                .def_property_readonly("found", &ICmdLineOption::found)
                .def_property_readonly("matchedName", &ICmdLineOption::getMatchedName);

            py::class_<CmdLineFlag, ICmdLineOption, std::shared_ptr<CmdLineFlag> >(m, "CmdLineFlag")
                .def(
                    pybind11::init(&CmdLineFlag::create),
                    pybind11::arg("names"),
                    pybind11::arg("help"),
                    pybind11::arg("group") = std::string());

            cmdLineOption<int>(m, "I");
            cmdLineOption<float>(m, "F");
            cmdLineOption<double>(m, "D");
            cmdLineOption<bool>(m, "Bool");
            cmdLineOption<std::string>(m, "String");

            py::class_<ICmdLineArg, std::shared_ptr<ICmdLineArg> >(m, "ICmdLineArg");

            cmdLineArg<int>(m, "I");
            cmdLineArg<float>(m, "F");
            cmdLineArg<double>(m, "D");
            cmdLineArg<bool>(m, "Bool");
            cmdLineArg<std::string>(m, "String");

            cmdLineListArg<std::string>(m, "String");
        }
    }
}
