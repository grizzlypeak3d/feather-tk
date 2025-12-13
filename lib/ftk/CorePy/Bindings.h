// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

#include <pybind11/pybind11.h>

#define FTK_ENUM_BIND(M, ENUM) \
    M.def("get" #ENUM "Enums", &get##ENUM##Enums); \
    M.def("getLabel", [](ENUM v) { return getLabel(v); }); \
    M.def("to_string", [](ENUM v) { return to_string(v); }); \
    M.def("from_string", [](const std::string& s, ENUM& v) { return from_string(s, v); })

    // \bug ImportError: Internal error while parsing type signature (1)
    //M.def("get" #ENUM "Labels", &get##ENUM##Labels); \

namespace ftk
{
    namespace python
    {
        template<typename T>
        void cmdLineValueOption(pybind11::module_&, const std::string& type);
        template<typename T>
        void cmdLineValueArg(pybind11::module_&, const std::string& type);
        template<typename T>
        void cmdLineListArg(pybind11::module_&, const std::string& type);

        template<typename T>
        void observable(pybind11::module_&, const std::string& type);
        template<typename T>
        void observableList(pybind11::module_&, const std::string& type);
        template<typename T, typename U>
        void observableMap(pybind11::module_&, const std::string& type);

        FTK_API void box(pybind11::module_&);
        FTK_API void cmdLine(pybind11::module_&);
        FTK_API void color(pybind11::module_&);
        FTK_API void context(pybind11::module_&);
        FTK_API void fontSystem(pybind11::module_&);
        FTK_API void iApp(pybind11::module_&);
        FTK_API void image(pybind11::module_&);
        FTK_API void iRender(pybind11::module_&);
        FTK_API void iSystem(pybind11::module_&);
        FTK_API void logSystem(pybind11::module_&);
        FTK_API void memory(pybind11::module_&);
        FTK_API void observable(pybind11::module_&);
        FTK_API void observableList(pybind11::module_&);
        FTK_API void observableMap(pybind11::module_&);
        FTK_API void os(pybind11::module_&);
        FTK_API void path(pybind11::module_&);
        FTK_API void random(pybind11::module_&);
        FTK_API void range(pybind11::module_&);
        FTK_API void size(pybind11::module_&);
        FTK_API void string(pybind11::module_&);
        FTK_API void timer(pybind11::module_&);
        FTK_API void vector(pybind11::module_&);

        FTK_API void coreBind(pybind11::module_&);
    }
}

#include <ftk/CorePy/CmdLineInline.h>
#include <ftk/CorePy/ObservableInline.h>
#include <ftk/CorePy/ObservableListInline.h>
#include <ftk/CorePy/ObservableMapInline.h>
