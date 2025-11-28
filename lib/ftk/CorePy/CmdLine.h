// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <pybind11/pybind11.h>

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
        void cmdLine(pybind11::module_&);
    }
}

#include <ftk/CorePy/CmdLineInline.h>
