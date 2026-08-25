# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

# An application that tests itself: feather-tk applications run
# headless, drive their own interface with synthetic input, and inspect
# the result -- the same machinery the toolkit's own test suite uses.
# This script builds a small application, then operates it the way a
# user would and checks what happened. Run it like any test; it exits
# non-zero on failure.
#
# The same tools are available from the command line in any feather-tk
# application: -screenshot writes an image of the window, -widgetDump
# writes the widget tree as JSON, and the FTK_TRACE_EVENTS environment
# variable logs input dispatch.

import ftkPy as ftk

import json
import sys

# Windows are offscreen for the whole process: everything works the
# same -- layout, input, drawing -- there is just no window on screen.
ftk.App.setOffscreenDefault(True)

context = ftk.Context()
app = ftk.App(context, sys.argv, "testing", "Self testing example")
if app.hasCmdLineHelp:
    sys.exit(0)

# A small application: a counter, a button that increments it, and a
# name field.
window = ftk.MainWindow(context, app, ftk.Size2I(640, 480))
layout = ftk.VerticalLayout(context)
layout.marginRole = ftk.SizeRole.Margin
countLabel = ftk.Label(context, "Count: 0", layout)
count = [0]
def increment():
    count[0] += 1
    countLabel.text = "Count: {0}".format(count[0])
button = ftk.PushButton(context, "Increment", layout)
button.setClickedCallback(increment)
nameEdit = ftk.LineEdit(context, layout)
window.widget = layout

# Lay the window out and run the event loop by hand. Without a layout
# pass every widget has an empty geometry, and a click aimed at one is
# aiming at nothing; the tick is what delivers events and settles
# updates.
window.show()
window.layout(ftk.Size2I(640, 480))
app.tick()

failures = []
def check(name, value):
    print(("PASS" if value else "FAIL"), name)
    if not value:
        failures.append(name)

# Click the button by its geometry, the way a user would: the press and
# release go through the window's dispatch, so focus, hover, and accept
# semantics are all the real ones.
g = button.geometry
center = ftk.V2I(g.min.x + g.w // 2, g.min.y + g.h // 2)
window.click(center)
app.tick()
check("click increments", countLabel.text == "Count: 1")
window.click(center)
app.tick()
check("second click", countLabel.text == "Count: 2")

# Type into the line edit: a click gives it focus, and text arrives as
# text input -- letters are not key presses.
g = nameEdit.geometry
window.click(ftk.V2I(g.min.x + g.w // 2, g.min.y + g.h // 2))
window.text("hello")
window.keyPress(ftk.Key.Return)
app.tick()
check("typed text", nameEdit.text == "hello")

# The widget tree serializes to JSON: what everything is, where it is,
# and what it shows. This is how a test asserts on structure without
# depending on pixels.
dump = json.loads(ftk.widgetDump(window))
def find(node, text):
    if node.get("text") == text:
        return node
    for child in node.get("children", []):
        result = find(child, text)
        if result:
            return result
    return None
check("label in dump", find(dump, "Count: 2") is not None)
check("button in dump", find(dump, "Increment") is not None)

sys.exit(1 if failures else 0)
