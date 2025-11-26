# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import sys

# Create the context and application.
context = ftk.Context()
app = ftk.App(context, sys.argv, "buttons", "Buttons example")
if app.getExit() != 0:
    sys.exit(app.getExit())

# Create a window.
window = ftk.MainWindow(context, app, ftk.Size2I(1280, 960))

# Create a layout.
layout = ftk.VerticalLayout(context)
layout.marginRole = ftk.SizeRole.Margin
scrollWidget = ftk.ScrollWidget(context, ftk.ScrollType.Both)
scrollWidget.border = False
scrollWidget.vStretch = ftk.Stretch.Expanding
scrollWidget.widget = layout
window.setWidget(scrollWidget)

# Create integer sliders.
groupBox = ftk.GroupBox(context, "Integer sliders", layout)
vLayout = ftk.VerticalLayout(context, groupBox)
intEditSlider = ftk.IntEditSlider(context, vLayout)
intEditSlider.setCallback(lambda value: print("Int:", value))
intEditSlider = ftk.IntEditSlider(context, vLayout)
intEditSlider.range = ftk.RangeI(100, 200)
intEditSlider.defaultValue = 100
intEditSlider.setCallback(lambda value: print("Int:", value))
intEditSlider = ftk.IntEditSlider(context, vLayout)
intEditSlider.range = ftk.RangeI(-100, 100)
intEditSlider.defaultValue = -100
intEditSlider.setCallback(lambda value: print("Int:", value))
        
# Create float sliders.
groupBox = ftk.GroupBox(context, "Float sliders", layout)
vLayout = ftk.VerticalLayout(context, groupBox)
floatEditSlider = ftk.FloatEditSlider(context, vLayout)
floatEditSlider.setCallback(lambda value: print("Float:", value))
floatEditSlider = ftk.FloatEditSlider(context, vLayout)
floatEditSlider.range = ftk.RangeF(100, 200)
floatEditSlider.defaultValue = 100
floatEditSlider.step = 1
floatEditSlider.largeStep = 10
floatEditSlider.setCallback(lambda value: print("Float:", value))
floatEditSlider = ftk.FloatEditSlider(context, vLayout)
floatEditSlider.range = ftk.RangeF(-100, 100)
floatEditSlider.defaultValue = -100
floatEditSlider.step = 1
floatEditSlider.largeStep = 10
floatEditSlider.setCallback(lambda value: print("Float:", value))
        
# Create double sliders.
groupBox = ftk.GroupBox(context, "Double sliders", layout)
vLayout = ftk.VerticalLayout(context, groupBox)
doubleEditSlider = ftk.DoubleEditSlider(context, vLayout)
doubleEditSlider.setCallback(lambda value: print("Double:", value))
doubleEditSlider = ftk.DoubleEditSlider(context, vLayout)
doubleEditSlider.range = ftk.RangeD(100, 200)
doubleEditSlider.defaultValue = 100
doubleEditSlider.step = 1
doubleEditSlider.largeStep = 10
doubleEditSlider.setCallback(lambda value: print("Double:", value))
doubleEditSlider = ftk.DoubleEditSlider(context, vLayout)
doubleEditSlider.range = ftk.RangeD(-100, 100)
doubleEditSlider.defaultValue = -100
doubleEditSlider.step = 1
doubleEditSlider.largeStep = 10
doubleEditSlider.setCallback(lambda value: print("Double:", value))

# Run the application.
app.run()
