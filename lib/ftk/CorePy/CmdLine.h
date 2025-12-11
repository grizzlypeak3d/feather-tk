// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

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
        
        FTK_API void cmdLine(pybind11::module_&);
    }
}

#include <ftk/CorePy/CmdLineInline.h>
