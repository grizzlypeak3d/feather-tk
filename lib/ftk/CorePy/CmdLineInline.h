// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/CmdLine.h>

namespace ftk
{
    namespace python
    {
        template<typename T>
        inline void cmdLineValueOption(pybind11::module_& m, const std::string& type)
        {
            std::string name = "CmdLineValueOption" + type;
            pybind11::class_<CmdLineValueOption<T>, ICmdLineOption, std::shared_ptr<CmdLineValueOption<T> > >(m, name.c_str())
                .def(
                    pybind11::init(&CmdLineValueOption<T>::create),
                    pybind11::arg("names"),
                    pybind11::arg("help"),
                    pybind11::arg("group") = std::string(),
                    pybind11::arg("defaultValue") = std::optional<T>(),
                    pybind11::arg("possibleValues") = std::string())
                .def_property_readonly("hasValue", &CmdLineValueOption<T>::hasValue)
                .def_property_readonly("value", &CmdLineValueOption<T>::getValue);
        }

        template<typename T>
        inline void cmdLineValueArg(pybind11::module_& m, const std::string& type)
        {
            std::string name = "CmdLineValueArg" + type;
            pybind11::class_<CmdLineValueArg<T>, ICmdLineArg, std::shared_ptr<CmdLineValueArg<T> > >(m, name.c_str())
                .def(
                    pybind11::init(&CmdLineValueArg<T>::create),
                    pybind11::arg("name"),
                    pybind11::arg("help"),
                    pybind11::arg("optional") = false)
                .def_property_readonly("hasValue", &CmdLineValueArg<T>::hasValue)
                .def_property_readonly("value", &CmdLineValueArg<T>::getValue);
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
