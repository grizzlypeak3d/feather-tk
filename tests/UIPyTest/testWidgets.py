# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import os
import unittest

@unittest.skipIf(os.environ.get("FTK_TESTS_NO_GL"), "OpenGL is not available")
class WidgetsTest(unittest.TestCase):

    def setUp(self):
        self.context = ftk.Context()
        ftk.uiInit(self.context)

    def test_colorSlider(self):
        model = ftk.IntModel()
        slider = ftk.ColorIntSlider(self.context, model)
        slider.setColors([ftk.V4F(0, 0, 0, 1), ftk.V4F(1, 1, 1, 1)])

        editSlider = ftk.ColorIntEditSlider(self.context)
        editSlider.range = ftk.RangeI(0, 50)
        editSlider.value = 25
        self.assertEqual(editSlider.value, 25)
        self.assertEqual(editSlider.range, ftk.RangeI(0, 50))
        editSlider.setCallback(lambda value: None)
        self.assertIsNotNone(editSlider.getModel())

        model = ftk.FloatModel()
        slider = ftk.ColorFloatSlider(self.context, model)

        editSlider = ftk.ColorFloatEditSlider(self.context)
        editSlider.range = ftk.RangeF(0.0, 2.0)
        editSlider.value = 1.0
        self.assertAlmostEqual(editSlider.value, 1.0)
        self.assertEqual(editSlider.range, ftk.RangeF(0.0, 2.0))

    def test_diagWidget(self):
        widget = ftk.DiagWidget(self.context)
        widget.marginRole = ftk.SizeRole.Margin
        self.assertEqual(widget.marginRole, ftk.SizeRole.Margin)

    def test_flowLayout(self):
        layout = ftk.FlowLayout(self.context)
        layout.marginRole = ftk.SizeRole.Margin
        self.assertEqual(layout.marginRole, ftk.SizeRole.Margin)
        layout.spacingRole = ftk.SizeRole.Spacing
        self.assertEqual(layout.spacingRole, ftk.SizeRole.Spacing)
        label = ftk.Label(self.context, "Label", layout)

    def test_splitter2D(self):
        splitter = ftk.Splitter2D(self.context)
        splitter.setWidgets([
            ftk.Label(self.context, "One"),
            ftk.Label(self.context, "Two")])
        splitter.split = ftk.V2F(0.3, 0.5)
        self.assertEqual(splitter.split, ftk.V2F(0.3, 0.5))
        splitter.setSplitCallback(lambda value: None)
        splitter.border = False
        self.assertFalse(splitter.border)

    def test_graphWidget(self):
        label = ftk.GraphLabel()
        label.colorRole = ftk.ColorRole.Red
        label.text = "FPS"
        label.format = ftk.DiagFormat()
        self.assertEqual(label.colorRole, ftk.ColorRole.Red)
        self.assertEqual(label.text, "FPS")
        widget = ftk.GraphWidget(self.context, "Title", [label])
        widget.setSamples(ftk.ColorRole.Red, [1, 2])
        widget.addSample(ftk.ColorRole.Red, 3)

    def test_pieChart(self):
        data = ftk.PieChartData(50.0, ftk.Color4F(1, 0, 0))
        self.assertAlmostEqual(data.percentage, 50.0)
        self.assertEqual(data, ftk.PieChartData(50.0, ftk.Color4F(1, 0, 0)))
        self.assertNotEqual(data, ftk.PieChartData(25.0, ftk.Color4F(1, 0, 0)))
        widget = ftk.PieChart(self.context)
        widget.data = [data, ftk.PieChartData(50.0, ftk.Color4F(0, 1, 0))]
        self.assertEqual(len(widget.data), 2)
        widget.sizeMult = 10
        self.assertEqual(widget.sizeMult, 10)

    def test_incButtons(self):
        button = ftk.IncButton(self.context)
        buttons = ftk.IncButtons(self.context)
        buttons.setIncCallback(lambda: None)
        buttons.setDecCallback(lambda: None)

    def test_shuttleWidget(self):
        widget = ftk.ShuttleWidget(self.context)
        self.assertEqual(widget.modifiers, 0)
        widget.setCallback(lambda value: None)
        widget.setDeltaCallback(lambda value: None)
        widget.setActiveCallback(lambda value: None)

    def test_editShuttle(self):
        widget = ftk.IntEditShuttle(self.context)
        widget.range = ftk.RangeI(0, 100)
        widget.value = 42
        self.assertEqual(widget.value, 42)
        self.assertEqual(widget.range, ftk.RangeI(0, 100))
        widget.setCallback(lambda value: None)

        widget = ftk.FloatEditShuttle(self.context)
        widget.range = ftk.RangeF(0.0, 2.0)
        widget.value = 1.0
        self.assertAlmostEqual(widget.value, 1.0)
        self.assertEqual(widget.range, ftk.RangeF(0.0, 2.0))
        widget.setCallback(lambda value: None)

        widget = ftk.DoubleEditShuttle(self.context)
        widget.range = ftk.RangeD(0.0, 2.0)
        widget.value = 1.0
        self.assertAlmostEqual(widget.value, 1.0)
        self.assertEqual(widget.range, ftk.RangeD(0.0, 2.0))
        widget.setCallback(lambda value: None)

    def test_levels(self):
        model = ftk.LevelsModel()
        self.assertEqual(model.value, ftk.RangeF(0.0, 1.0))
        self.assertEqual(model.range, ftk.RangeF(0.0, 1.0))
        self.assertFalse(model.hasDefault)

        slider = ftk.LevelsSlider(self.context, model)
        slider.value = ftk.RangeF(0.1, 0.9)
        self.assertEqual(slider.value, model.value)
        self.assertIs(slider.getModel(), model)
        slider.setCallback(lambda value: None)

        editSlider = ftk.LevelsEditSlider(self.context)
        editSlider.range = ftk.RangeF(0.0, 2.0)
        self.assertEqual(editSlider.range, ftk.RangeF(0.0, 2.0))
        editSlider.value = ftk.RangeF(0.5, 1.5)
        self.assertEqual(editSlider.value, ftk.RangeF(0.5, 1.5))
        editSlider.precision = 3
        self.assertEqual(editSlider.precision, 3)
        editSlider.setCallback(lambda value: None)

    def test_listItemsWidget(self):
        widget = ftk.ListItemsWidget(self.context, ftk.ButtonGroupType.Check)
        widget.setItems([ftk.ListItem("one"), ftk.ListItem("two")])
        self.assertEqual(len(widget.getItems()), 2)
        widget.setChecked(0, True)
        self.assertTrue(widget.getChecked(0))
        self.assertFalse(widget.getChecked(1))
        widget.setCallback(lambda index, value: None)
        widget.current = 1
        self.assertEqual(widget.current, 1)
        widget.search = "one"
        self.assertEqual(widget.search, "one")
        widget.clearSearch()
        self.assertEqual(widget.search, "")

    def test_iContainerSubclass(self):
        class TestContainer(ftk.IContainer):
            def __init__(self, context):
                ftk.IContainer.__init__(self, context, "Test", None)
                self.label = ftk.Label(context, "x")
                self._setWidget(self.label)
        container = TestContainer(self.context)
        self.assertIs(container.widget, container.label)
        self.assertEqual(container.getSizeHint(), container.label.getSizeHint())

    def test_iContainerSubclasses(self):
        self.assertTrue(issubclass(ftk.Bellows, ftk.IContainer))

if __name__ == '__main__':
    unittest.main()
