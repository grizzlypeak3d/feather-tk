# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import sys

# Create the context and application.
context = ftk.Context()
app = ftk.App(context, sys.argv, "MDI", "MDI example")
if app.exitValue != 0:
    sys.exit(app.exitValue)

# Create a window.
window = ftk.MainWindow(context, app, ftk.Size2I(1280, 960))

# Create a scroll widget.
scrollWidget = ftk.ScrollWidget(context, ftk.ScrollType.Both)
scrollWidget.areaResizable = False
scrollWidget.border = False
scrollWidget.vStretch = ftk.Stretch.Expanding
window.widget = scrollWidget

# Create a MDI canvas.
canvas = ftk.MDICanvas(context)
canvasSize = ftk.Size2I(8192, 8192)
canvas.setSize(canvasSize)
scrollWidget.widget = canvas

# Create MDI widgets.
random = ftk.Random()
for i in range(0, 25):
    colorWidget = ftk.ColorWidget(context)
    colorWidget.color = ftk.Color4F(
        random.getF(),
        random.getF(),
        random.getF())
    canvas.addWidget(
        "Color {0}".format(i),
        ftk.V2I(random.getI(0, canvasSize[0]), random.getI(0, canvasSize[1])),
        colorWidget)

# Create a MDI mini-map.
miniMap = ftk.MDIMiniMap(context)
scrollWidget.viewportWidget = miniMap

# Setup callbacks.
scrollWidget.setScrollInfoCallback(
    lambda value: miniMap.setScrollInfo(value))
canvas.setChildGeometryCallback(
    lambda value: miniMap.setChildGeometry(value))
miniMap.setCallback(
    lambda value: scrollWidget.setScrollPos(value))

# Run the application.
app.run()

# Clean up.
window = None
app = None

