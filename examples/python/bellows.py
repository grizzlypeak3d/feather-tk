# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import sys

# Create the context and application.
context = ftk.Context()
app = ftk.App(context, sys.argv, "bellows", "Bellows example")
if app.exitValue != 0:
    sys.exit(app.exitValue)

# Create a window.
window = ftk.MainWindow(context, app, ftk.Size2I(1280, 960))

# Create a layout.
layout = ftk.VerticalLayout(context)
layout.spacingRole = ftk.SizeRole._None
scrollWidget = ftk.ScrollWidget(context, ftk.ScrollType.Both)
scrollWidget.border = False
scrollWidget.vStretch = ftk.Stretch.Expanding
scrollWidget.widget = layout;
window.setWidget(scrollWidget)

# Add a bellows.
vLayout = ftk.VerticalLayout(context)
vLayout.marginRole = ftk.SizeRole.Margin
vLayout.spacingRole = ftk.SizeRole.SpacingSmall
for i in range(0, 10):
    ftk.PushButton(context, "Push Button {}".format(i), vLayout)
bellows = ftk.Bellows(context, "Push Buttons", layout)
bellows.widget = vLayout

# Add a bellows.
vLayout = ftk.VerticalLayout(context)
vLayout.marginRole = ftk.SizeRole.Margin
vLayout.spacingRole = ftk.SizeRole.SpacingSmall
icons = [
    "Audio",
    "Close",
    "Copy",
    "Directory",
    "File",
    "FrameNext",
    "FramePrev",
    "Mute",
    "Search",
    "Volume"
]
for i in range(0, 10):
    button = ftk.ToolButton(context, "Tool Button {}".format(i), vLayout)
    if i < len(icons):
        button.icon = icons[i]
bellows = ftk.Bellows(context, "Tool Buttons", layout)
bellows.widget = vLayout

# Add a bellows.
vLayout = ftk.VerticalLayout(context)
vLayout.marginRole = ftk.SizeRole.Margin
vLayout.spacingRole = ftk.SizeRole.SpacingSmall
for i in range(0, 10):
    ftk.CheckBox(context, "Check Box {}".format(i), vLayout)
bellows = ftk.Bellows(context, "Check Boxes", layout)
bellows.widget = vLayout

# Run the application.
app.run()
