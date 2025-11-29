# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import sys

# Create the context and application.
context = ftk.Context()
app = ftk.App(context, sys.argv, "buttons", "Buttons example")
if app.exitValue!= 0:
    sys.exit(app.exitValue)

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
        
# Create push buttons.
groupBox = ftk.GroupBox(context, "Push Buttons", layout)
hLayout = ftk.HorizontalLayout(context, groupBox)
hLayout.spacingRole = ftk.SizeRole.SpacingSmall
pushButton = ftk.PushButton(context, "Click", hLayout)
pushButton.setClickedCallback(lambda: print("Click"))
pushButton = ftk.PushButton(context, "Text", hLayout)
pushButton.icon = "Settings"
pushButton = ftk.PushButton(context, "Disabled", hLayout)
pushButton.enabled = False
        
# Create tool buttons.
groupBox = ftk.GroupBox(context, "Tool Buttons", layout)
hLayout = ftk.HorizontalLayout(context, groupBox)
hLayout.spacingRole = ftk.SizeRole.SpacingSmall
toolButtonGroup = ftk.ButtonGroup(context, ftk.ButtonGroupType.Radio)
toolButtonGroup.setCheckedCallback(lambda index, checked: print("Tool:", index))
hLayout = ftk.HorizontalLayout(context, groupBox)
hLayout.spacingRole = ftk.SizeRole.SpacingSmall
toolIcons = [
    "PlaybackReverse",
    "PlaybackStop",
    "PlaybackForward"
]
hLayout2 = ftk.HorizontalLayout(context, hLayout)
hLayout2.spacingRole = ftk.SizeRole.SpacingTool
for i in range(0, 3):
    toolButton = ftk.ToolButton(context, hLayout2)
    toolButton.checkable = True
    toolButton.checked = 0 == i
    toolButton.icon = toolIcons[i]
    toolButtonGroup.addButton(toolButton)
playStopButton = ftk.ToolButton(context, hLayout)
playStopButton.icon = "PlaybackStop"
playStopButton.checkedIcon = "PlaybackForward"
playStopButton.checkable = True
toolButton = ftk.ToolButton(context, "Text", hLayout)
toolButton = ftk.ToolButton(context, "Text", hLayout)
toolButton.icon = "Settings"
toolButton = ftk.ToolButton(context, "Disabled", hLayout)
toolButton.enabled = False
toolButtonGroup.addButton(toolButton)

# Create check boxes.
groupBox = ftk.GroupBox(context, "Check Boxes", layout)
vLayout = ftk.VerticalLayout(context, groupBox)
vLayout.spacingRole = ftk.SizeRole.SpacingSmall
checkBox = ftk.CheckBox(context, "Check", vLayout)
checkBox.setCheckedCallback(lambda checked: print("Checked:", checked))
checkBox = ftk.CheckBox(context, "Disabled", vLayout)
checkBox.enabled = False

# Create radio buttons.
groupBox = ftk.GroupBox(context, "Radio Buttons", layout)
vLayout = ftk.VerticalLayout(context, groupBox)
vLayout.spacingRole = ftk.SizeRole.SpacingSmall
radioButtonGroup = ftk.ButtonGroup(context, ftk.ButtonGroupType.Radio)
radioButtonGroup.setCheckedCallback(lambda index, checked: print("Radio:", index))
for i in range(0, 3):
    radioButton = ftk.RadioButton(context, "Radio {}".format(i), vLayout)
    radioButton.setCheckedCallback(lambda checked: print("Radio:", checked))
    radioButton.checked = 0 == i
    radioButtonGroup.addButton(radioButton)
radioButton = ftk.RadioButton(context, "Disabled", vLayout)
radioButton.enabled = False

# Run the application.
app.run()

# Clean up.
window = None
app = None
