// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/CmdLine.h>

namespace ftk
{
    namespace python
    {
        template<typename T>
        inline void cmdLineOption(nanobind::module_& m, const std::string& type)
        {
            std::string name = "CmdLineOption" + type;
            nanobind::class_<CmdLineOption<T>, ICmdLineOption>(m, name.c_str())
                .def(
                    nanobind::new_(&CmdLineOption<T>::create),
                    nanobind::arg("names"),
                    nanobind::arg("help"),
                    nanobind::arg("group") = std::string(),
                    nanobind::arg("defaultValue") = std::optional<T>(),
                    nanobind::arg("possibleValues") = std::string())
                .def_prop_ro("hasValue", &CmdLineOption<T>::hasValue)
                .def_prop_ro("value", &CmdLineOption<T>::getValue);
        }

        template<typename T>
        inline void cmdLineListOption(nanobind::module_& m, const std::string& type)
        {
            std::string name = "CmdLineListOption" + type;
            nanobind::class_<CmdLineListOption<T>, ICmdLineOption>(m, name.c_str())
                .def(
                    nanobind::new_(&CmdLineListOption<T>::create),
                    nanobind::arg("names"),
                    nanobind::arg("help"),
                    nanobind::arg("group") = std::string())
                .def_prop_ro("list", &CmdLineListOption<T>::getList);
        }

        template<typename T>
        inline void cmdLineArg(nanobind::module_& m, const std::string& type)
        {
            std::string name = "CmdLineArg" + type;
            nanobind::class_<CmdLineArg<T>, ICmdLineArg>(m, name.c_str())
                .def(
                    nanobind::new_(&CmdLineArg<T>::create),
                    nanobind::arg("name"),
                    nanobind::arg("help"),
                    nanobind::arg("optional") = false)
                .def_prop_ro("hasValue", &CmdLineArg<T>::hasValue)
                .def_prop_ro("value", &CmdLineArg<T>::getValue);
        }

        template<typename T>
        inline void cmdLineListArg(nanobind::module_& m, const std::string& type)
        {
            std::string name = "CmdLineListArg" + type;
            nanobind::class_<CmdLineListArg<T>, ICmdLineArg>(m, name.c_str())
                .def(
                    nanobind::new_(&CmdLineListArg<T>::create),
                    nanobind::arg("name"),
                    nanobind::arg("help"),
                    nanobind::arg("optional") = false)
                .def_prop_ro("list", &CmdLineListArg<T>::getList);
        }
    }
}
