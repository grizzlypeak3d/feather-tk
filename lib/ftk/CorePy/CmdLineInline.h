// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/CmdLine.h>

namespace ftk
{
    namespace python
    {
        template<typename T>
        inline void cmdLineOption(pybind11::module_& m, const std::string& type)
        {
            std::string name = "CmdLineOption" + type;
            pybind11::class_<CmdLineOption<T>, ICmdLineOption, std::shared_ptr<CmdLineOption<T> > >(m, name.c_str())
                .def(
                    pybind11::init(&CmdLineOption<T>::create),
                    pybind11::arg("names"),
                    pybind11::arg("help"),
                    pybind11::arg("group") = std::string(),
                    pybind11::arg("defaultValue") = std::optional<T>(),
                    pybind11::arg("possibleValues") = std::string())
                .def_property_readonly("hasValue", &CmdLineOption<T>::hasValue)
                .def_property_readonly("value", &CmdLineOption<T>::getValue);
        }

        template<typename T>
        inline void cmdLineArg(pybind11::module_& m, const std::string& type)
        {
            std::string name = "CmdLineArg" + type;
            pybind11::class_<CmdLineArg<T>, ICmdLineArg, std::shared_ptr<CmdLineArg<T> > >(m, name.c_str())
                .def(
                    pybind11::init(&CmdLineArg<T>::create),
                    pybind11::arg("name"),
                    pybind11::arg("help"),
                    pybind11::arg("optional") = false)
                .def_property_readonly("hasValue", &CmdLineArg<T>::hasValue)
                .def_property_readonly("value", &CmdLineArg<T>::getValue);
        }

        template<typename T>
        inline void cmdLineListArg(pybind11::module_& m, const std::string& type)
        {
            std::string name = "CmdLineListArg" + type;
            pybind11::class_<CmdLineListArg<T>, ICmdLineArg, std::shared_ptr<CmdLineListArg<T> > >(m, name.c_str())
                .def(
                    pybind11::init(&CmdLineListArg<T>::create),
                    pybind11::arg("name"),
                    pybind11::arg("help"),
                    pybind11::arg("optional") = false)
                .def_property_readonly("list", &CmdLineListArg<T>::getList);
        }
    }
}
