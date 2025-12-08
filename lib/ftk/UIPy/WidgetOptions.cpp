// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/WidgetOptions.h>

#include <ftk/CorePy/Util.h>

#include <ftk/UI/IWidget.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void widgetOptions(py::module_& m)
        {
            py::enum_<Orientation>(m, "Orientation")
                .value("Horizontal", Orientation::Horizontal)
                .value("Vertical", Orientation::Vertical);
            FTK_ENUM_BIND(m, Orientation);

            py::enum_<Stretch>(m, "Stretch")
                .value("Fixed", Stretch::Fixed)
                .value("Expanding", Stretch::Expanding);
            FTK_ENUM_BIND(m, Stretch);

            py::enum_<HAlign>(m, "HAlign")
                .value("Fill", HAlign::Fill)
                .value("Left", HAlign::Left)
                .value("Center", HAlign::Center)
                .value("Right", HAlign::Right);
            FTK_ENUM_BIND(m, HAlign);

            py::enum_<VAlign>(m, "VAlign")
                .value("Fill", VAlign::Fill)
                .value("Top", VAlign::Top)
                .value("Center", VAlign::Center)
                .value("Bottom", VAlign::Bottom);
            FTK_ENUM_BIND(m, VAlign);
        }
    }
}
