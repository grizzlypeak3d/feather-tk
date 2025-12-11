// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>

#include <pybind11/pybind11.h>

namespace ftk
{
    namespace python
    {
        FTK_API void formLayout(pybind11::module_&);
    }
}
