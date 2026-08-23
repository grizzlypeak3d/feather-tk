// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once


#include <pybind11/pybind11.h>

#define FTK_ENUM_BIND(M, ENUM) \
    M.def("get" #ENUM "Enums", &get##ENUM##Enums); \
    M.def("getLabel", [](ENUM v) { return getLabel(v); }); \
    M.def("to_string", [](ENUM v) { return to_string(v); }); \
    M.def("from_string", [](const std::string& s, ENUM& v) { return from_string(s, v); })
    // \bug ImportError: Internal error while parsing type signature (1)
    //M.def("get" #ENUM "Labels", &get##ENUM##Labels);

namespace ftk
{
    namespace python
    {
        template<typename T>
        void cmdLineOption(pybind11::module_&, const std::string& type);
        template<typename T>
        void cmdLineArg(pybind11::module_&, const std::string& type);
        template<typename T>
        void cmdLineListArg(pybind11::module_&, const std::string& type);

        template<typename T>
        void observable(pybind11::module_&, const std::string& type);
        template<typename T>
        void observableList(pybind11::module_&, const std::string& type);
        template<typename T, typename U>
        void observableMap(pybind11::module_&, const std::string& type);

        void box(pybind11::module_&);
        void cmdLine(pybind11::module_&);
        void color(pybind11::module_&);
        void command(pybind11::module_&);
        void context(pybind11::module_&);
        void diagSystem(pybind11::module_&);
        void fileLogSystem(pybind11::module_&);
        void fontSystem(pybind11::module_&);
        void iApp(pybind11::module_&);
        void image(pybind11::module_&);
        void iRender(pybind11::module_&);
        void iSystem(pybind11::module_&);
        void logSystem(pybind11::module_&);
        void math(pybind11::module_&);
        void matrix(pybind11::module_&);
        void memory(pybind11::module_&);
        void mesh(pybind11::module_&);
        void noise(pybind11::module_&);
        void observable(pybind11::module_&);
        void observableList(pybind11::module_&);
        void observableMap(pybind11::module_&);
        void os(pybind11::module_&);
        void path(pybind11::module_&);
        void random(pybind11::module_&);
        void range(pybind11::module_&);
        void renderOptions(pybind11::module_&);
        void size(pybind11::module_&);
        void string(pybind11::module_&);
        void time(pybind11::module_&);
        void timer(pybind11::module_&);
        void vector(pybind11::module_&);
        void version(pybind11::module_&);

        void coreBind(pybind11::module_&);
    }
}

#include <ftk/CorePy/CmdLineInline.h>
#include <ftk/CorePy/ObservableInline.h>
#include <ftk/CorePy/ObservableListInline.h>
#include <ftk/CorePy/ObservableMapInline.h>
