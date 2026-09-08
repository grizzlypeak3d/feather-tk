// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CorePy/Bindings.h>

#include <ftk/Core/CmdLine.h>

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
        void cmdLine(nb::module_& m)
        {
            nb::class_<ICmdLineOption>(m, "ICmdLineOption")
                .def_prop_ro("help", &ICmdLineOption::getHelp)
                .def_prop_ro("group", &ICmdLineOption::getGroup)
                .def_prop_ro("found", &ICmdLineOption::found)
                .def_prop_ro("matchedName", &ICmdLineOption::getMatchedName);

            nb::class_<CmdLineFlag, ICmdLineOption>(m, "CmdLineFlag")
                .def(
                    nanobind::new_(&CmdLineFlag::create),
                    nanobind::arg("names"),
                    nanobind::arg("help"),
                    nanobind::arg("group") = std::string());

            cmdLineOption<int>(m, "I");
            cmdLineOption<float>(m, "F");
            cmdLineOption<double>(m, "D");
            cmdLineOption<bool>(m, "Bool");
            cmdLineOption<std::string>(m, "String");

            cmdLineListOption<std::string>(m, "String");

            nb::class_<ICmdLineArg>(m, "ICmdLineArg");

            cmdLineArg<int>(m, "I");
            cmdLineArg<float>(m, "F");
            cmdLineArg<double>(m, "D");
            cmdLineArg<bool>(m, "Bool");
            cmdLineArg<std::string>(m, "String");

            cmdLineListArg<std::string>(m, "String");
        }
    }
}
