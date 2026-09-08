// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once


#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

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
        void cmdLineOption(nanobind::module_&, const std::string& type);
        template<typename T>
        void cmdLineArg(nanobind::module_&, const std::string& type);
        template<typename T>
        void cmdLineListArg(nanobind::module_&, const std::string& type);

        template<typename T>
        void observable(nanobind::module_&, const std::string& type);
        template<typename T>
        void observableList(nanobind::module_&, const std::string& type);
        template<typename T, typename U>
        void observableMap(nanobind::module_&, const std::string& type);

        void box(nanobind::module_&);
        void cmdLine(nanobind::module_&);
        void color(nanobind::module_&);
        void command(nanobind::module_&);
        void context(nanobind::module_&);
        void diagSystem(nanobind::module_&);
        void fileLogSystem(nanobind::module_&);
        void fontSystem(nanobind::module_&);
        void iApp(nanobind::module_&);
        void image(nanobind::module_&);
        void iRender(nanobind::module_&);
        void iSystem(nanobind::module_&);
        void logSystem(nanobind::module_&);
        void math(nanobind::module_&);
        void matrix(nanobind::module_&);
        void memory(nanobind::module_&);
        void mesh(nanobind::module_&);
        void noise(nanobind::module_&);
        void observable(nanobind::module_&);
        void observableList(nanobind::module_&);
        void observableMap(nanobind::module_&);
        void os(nanobind::module_&);
        void path(nanobind::module_&);
        void random(nanobind::module_&);
        void range(nanobind::module_&);
        void renderOptions(nanobind::module_&);
        void size(nanobind::module_&);
        void string(nanobind::module_&);
        void time(nanobind::module_&);
        void timer(nanobind::module_&);
        void vector(nanobind::module_&);
        void version(nanobind::module_&);

        void coreBind(nanobind::module_&);
    }
}

#include <ftk/CorePy/CmdLineInline.h>
#include <ftk/CorePy/ObservableInline.h>
#include <ftk/CorePy/ObservableListInline.h>
#include <ftk/CorePy/ObservableMapInline.h>
