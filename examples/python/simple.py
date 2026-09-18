# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import feather_tk as ftk
import sys

# Create the context and application.
context = ftk.Context()
app = ftk.App(context, sys.argv, "simple", "Simple example")
if app.hasCmdLineHelp:
    sys.exit(0)

# Create a window.
window = ftk.MainWindow(context, app, ftk.Size2I(1280, 960))

# Create a label, centered in the window: the layout centers it across,
# and the spacers center it along.
layout = ftk.VerticalLayout(context)
spacer = ftk.Spacer(context, ftk.Orientation.Vertical, layout)
spacer.setStretch(ftk.Stretch.Expanding)
label = ftk.Label(context, "Hello world", layout)
label.fontSize = 32
label.hAlign = ftk.HAlign.Center
spacer = ftk.Spacer(context, ftk.Orientation.Vertical, layout)
spacer.setStretch(ftk.Stretch.Expanding)
window.widget = layout

# Run the application.
app.run()

# Clean up.
window = None
app = None

