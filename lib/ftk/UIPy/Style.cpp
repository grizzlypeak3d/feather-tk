// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/Style.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void style(nb::module_& m)
        {
            FTK_ENUM_PY(m, SizeRole);
            FTK_ENUM_BIND(m, SizeRole);

            FTK_ENUM_PY(m, ColorRole);
            FTK_ENUM_BIND(m, ColorRole);
        }
    }
}
