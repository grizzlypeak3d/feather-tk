// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/ScrollArea.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/ScrollArea.h>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void scrollArea(py::module_& m)
        {
            py::enum_<ScrollType>(m, "ScrollType")
                .value("Horizontal", ScrollType::Horizontal)
                .value("Vertical", ScrollType::Vertical)
                .value("Both", ScrollType::Both)
                .value("Menu", ScrollType::Menu);
            FTK_ENUM_BIND(m, ScrollType);
        }
    }
}
