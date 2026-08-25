# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

# A tool panel from JSON, wired up: the Python counterpart of the C++
# panel example. The structure and the properties live in panel.json,
# and the behavior lives here, attached to the widgets by their ids --
# the way markup and script divide a web page.
#
# Where the C++ example embeds the JSON in the binary with
# ftk_embed_text(), a script just reads the file beside it. Python also
# needs no casts: findWidget() returns the widget as its concrete type.

import ftkPy as ftk

import os
import sys

context = ftk.Context()
app = ftk.App(context, sys.argv, "panel", "Tool panel from JSON example")
if app.hasCmdLineHelp:
    sys.exit(0)

window = ftk.MainWindow(context, app, ftk.Size2I(640, 480))

# Build the panel from the JSON. The application's own data is trusted,
# so errors here are programming mistakes and fail loudly instead of
# showing a broken panel.
path = os.path.join(os.path.dirname(os.path.abspath(__file__)), "panel.json")
widget, errors = ftk.widgetLoad(context, open(path).read())
if errors:
    sys.exit("\n".join(errors))
window.widget = widget

# Find the widgets by id and attach the behavior.
widthSlider = ftk.findWidget(widget, "width")
samplesSlider = ftk.findWidget(widget, "samples")
outputEdit = ftk.findWidget(widget, "output")
statusLabel = ftk.findWidget(widget, "status")
renderButton = ftk.findWidget(widget, "render")

def status(value = None):
    statusLabel.text = "{0} pixels wide, {1} samples".format(
        widthSlider.value, samplesSlider.value)
widthSlider.setCallback(status)
samplesSlider.setCallback(status)

def render():
    print("Render {0} wide, {1} samples, to {2}".format(
        widthSlider.value, samplesSlider.value, outputEdit.text))
    statusLabel.text = "Rendered " + outputEdit.text
renderButton.setClickedCallback(render)

window.show()
app.run()
app = None
