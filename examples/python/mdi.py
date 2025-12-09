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
size = ftk.Size2I(1280, 960)
window = ftk.MainWindow(context, app, size)

# Create a scroll widget.
scrollWidget = ftk.ScrollWidget(context, ftk.ScrollType.Both)
scrollWidget.border = False
scrollWidget.vStretch = ftk.Stretch.Expanding
window.widget = scrollWidget

# Create a MDI canvas.
canvas = ftk.MDICanvas(context)
scrollWidget.widget = canvas
        
# Create MDI widgets.
random = ftk.Random()
for i in range(0, 10):
    colorWidget = ftk.ColorWidget(context)
    colorWidget.color = ftk.Color4F(
        random.getF(),
        random.getF(),
        random.getF())
    canvas.addWidget(
        "Color {0}".format(i),
        ftk.V2I(random.getI(0, size[0]), random.getI(0, size[1])),
        colorWidget)

# Run the application.
app.run()

# Clean up.
window = None
app = None

