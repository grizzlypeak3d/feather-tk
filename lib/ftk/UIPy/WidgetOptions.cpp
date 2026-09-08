// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Bindings.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/IWidget.h>

#include <nanobind/nanobind.h>
#include <nanobind/stl/shared_ptr.h>
#include <nanobind/stl/string.h>

namespace nb = nanobind;

namespace ftk
{
    namespace python
    {
        void widgetOptions(nb::module_& m)
        {
            FTK_ENUM_PY(m, Orientation);
            FTK_ENUM_BIND(m, Orientation);

            FTK_ENUM_PY(m, Stretch);
            FTK_ENUM_BIND(m, Stretch);

            FTK_ENUM_PY(m, HAlign);
            FTK_ENUM_BIND(m, HAlign);

            FTK_ENUM_PY(m, VAlign);
            FTK_ENUM_BIND(m, VAlign);
        }
    }
}
