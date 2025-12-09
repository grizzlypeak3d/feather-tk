# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import sys

# Create the context and application.
context = ftk.Context()
app = ftk.App(context, sys.argv, "tabs", "Tabs example")
if app.exitValue != 0:
    sys.exit(app.exitValue)

# Create a window.
window = ftk.MainWindow(context, app, ftk.Size2I(1280, 960))

# Create a tab widget.
tabWidget = ftk.TabWidget(context)
tabWidget.vStretch = ftk.Stretch.Expanding
window.widget = tabWidget

# Add tabs.
for i in range(0, 10):
    label = ftk.Label(context, ftk.getLoremIpsum(100))
    label.marginRole = ftk.SizeRole.MarginSmall
    scrollWidget = ftk.ScrollWidget(context)
    scrollWidget.border = False
    scrollWidget.widget = label
    tabWidget.addTab("Tab {}".format(i), scrollWidget)

# Run the application.
app.run()

# Clean up.
window = None
app = None

