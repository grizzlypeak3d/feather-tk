# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk
import sys

# Create the context and application.
context = ftk.Context()
app = ftk.App(context, sys.argv, "simple", "Simple example")
if app.exitValue != 0:
    sys.exit(app.exitValue)

# Create a window.
window = ftk.MainWindow(context, app, ftk.Size2I(1280, 960))

# Create a label.
label = ftk.Label(context, "Hello world")
label.fontRole = ftk.FontRole.Title
label.setAlign(ftk.HAlign.Center, ftk.VAlign.Center)
window.widget = label

# Run the application.
app.run()

# Clean up.
window = None
app = None

