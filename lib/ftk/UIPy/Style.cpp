// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/UIPy/Style.h>

#include <ftk/CorePy/Bindings.h>

#include <ftk/UI/Style.h>

#include <pybind11/pybind11.h>

namespace py = pybind11;

namespace ftk
{
    namespace python
    {
        void style(py::module_& m)
        {
            py::enum_<SizeRole>(m, "SizeRole")
                .value("_None", SizeRole::None)
                .value("Margin", SizeRole::Margin)
                .value("MarginSmall", SizeRole::MarginSmall)
                .value("MarginLarge", SizeRole::MarginLarge)
                .value("MarginInside", SizeRole::MarginInside)
                .value("MarginDialog", SizeRole::MarginDialog)
                .value("Spacing", SizeRole::Spacing)
                .value("SpacingSmall", SizeRole::SpacingSmall)
                .value("SpacingLarge", SizeRole::SpacingLarge)
                .value("SpacingTool", SizeRole::SpacingTool)
                .value("SizeRole_Icon", SizeRole::Icon)
                .value("Border", SizeRole::Border)
                .value("KeyFocus", SizeRole::KeyFocus)
                .value("LabelPad", SizeRole::LabelPad)
                .value("ScrollArea", SizeRole::ScrollArea)
                .value("ScrollAreaSmall", SizeRole::ScrollAreaSmall)
                .value("Slider", SizeRole::Slider)
                .value("Handle", SizeRole::Handle)
                .value("Swatch", SizeRole::Swatch)
                .value("SwatchLarge", SizeRole::SwatchLarge)
                .value("Shadow", SizeRole::Shadow)
                .value("DragLength", SizeRole::DragLength);
            //FTK_ENUM_BIND(m, SizeRole);

            py::enum_<ColorRole>(m, "ColorRole")
                .value("_None", ColorRole::None)
                .value("Window", ColorRole::Window)
                .value("Base", ColorRole::Base)
                .value("Button", ColorRole::Button)
                .value("Text", ColorRole::Text)
                .value("TextDisabled", ColorRole::TextDisabled)
                .value("Border", ColorRole::Border)
                .value("Hover", ColorRole::Hover)
                .value("Pressed", ColorRole::Pressed)
                .value("Checked", ColorRole::Checked)
                .value("KeyFocus", ColorRole::KeyFocus)
                .value("Overlay", ColorRole::Overlay)
                .value("TooltipWindow", ColorRole::TooltipWindow)
                .value("TooltipText", ColorRole::TooltipText)
                .value("Red", ColorRole::Red)
                .value("Green", ColorRole::Green)
                .value("Blue", ColorRole::Blue)
                .value("Cyan", ColorRole::Cyan)
                .value("Magenta", ColorRole::Magenta)
                .value("Yellow", ColorRole::Yellow);
            //FTK_ENUM_BIND(m, ColorRole);

            py::enum_<FontRole>(m, "FontRole")
                .value("_None", FontRole::None)
                .value("Label", FontRole::Label)
                .value("Mono", FontRole::Mono)
                .value("Title", FontRole::Title);
            FTK_ENUM_BIND(m, FontRole);
        }
    }
}
