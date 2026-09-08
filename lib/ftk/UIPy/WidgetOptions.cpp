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
            nb::enum_<Orientation>(m, "Orientation")
                .value("Horizontal", Orientation::Horizontal)
                .value("Vertical", Orientation::Vertical);
            FTK_ENUM_BIND(m, Orientation);

            nb::enum_<Stretch>(m, "Stretch")
                .value("Fixed", Stretch::Fixed)
                .value("Expanding", Stretch::Expanding);
            FTK_ENUM_BIND(m, Stretch);

            nb::enum_<HAlign>(m, "HAlign")
                .value("Fill", HAlign::Fill)
                .value("Left", HAlign::Left)
                .value("Center", HAlign::Center)
                .value("Right", HAlign::Right);
            FTK_ENUM_BIND(m, HAlign);

            nb::enum_<VAlign>(m, "VAlign")
                .value("Fill", VAlign::Fill)
                .value("Top", VAlign::Top)
                .value("Center", VAlign::Center)
                .value("Bottom", VAlign::Bottom);
            FTK_ENUM_BIND(m, VAlign);
        }
    }
}
