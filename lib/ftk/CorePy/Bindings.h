// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once


#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

#include <cctype>
#include <vector>

#define FTK_ENUM_BIND(M, ENUM) \
    M.def("get" #ENUM "Enums", &get##ENUM##Enums); \
    M.def("getLabel", [](ENUM v) { return getLabel(v); }); \
    M.def("to_string", [](ENUM v) { return to_string(v); }); \
    M.def("from_string", [](const std::string& s, ENUM& v) { return from_string(s, v); })
    // \bug ImportError: Internal error while parsing type signature (1)
    //M.def("get" #ENUM "Labels", &get##ENUM##Labels);

//! Bind an FTK_ENUM'd enum, taking every value and its Python name from
//! the labels -- a value added to the enum is bound the moment it exists,
//! where a hand-written list falls behind silently (Key was missing
//! F13-F24 for years). See ftk::python::bindEnum().
#define FTK_ENUM_PY(M, ENUM) \
    ftk::python::bindEnum<ENUM>(M, #ENUM, get##ENUM##Labels())

namespace ftk
{
    namespace python
    {
        //! A Python name for an enum label: the label with the
        //! non-identifier characters removed, prefixed with an underscore
        //! when it would start with a digit or shadow a Python keyword --
        //! "Margin Small" is MarginSmall, "Extra 1" is Extra1, "None" is
        //! _None.
        inline std::string enumValueName(const std::string& label)
        {
            std::string out;
            for (char c : label)
            {
                if (std::isalnum(static_cast<unsigned char>(c)) || '_' == c)
                {
                    out.push_back(c);
                }
            }
            if (out.empty() ||
                std::isdigit(static_cast<unsigned char>(out[0])) ||
                "None" == out || "True" == out || "False" == out)
            {
                out.insert(out.begin(), '_');
            }
            return out;
        }

        //! Bind an enum from its FTK_ENUM labels; see FTK_ENUM_PY. Only
        //! for enums whose values run 0..Count-1 -- a bitmask binds its
        //! values by hand.
        template<typename T>
        inline nanobind::enum_<T> bindEnum(
            nanobind::module_& m,
            const char* name,
            const std::vector<std::string>& labels)
        {
            nanobind::enum_<T> out(m, name);
            for (std::size_t i = 0; i < labels.size(); ++i)
            {
                out.value(
                    enumValueName(labels[i]).c_str(),
                    static_cast<T>(i));
            }
            return out;
        }
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
