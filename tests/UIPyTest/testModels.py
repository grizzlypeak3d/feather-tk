# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class IntModelTest(unittest.TestCase):

    def test_value(self):
        m = ftk.IntModel()
        self.assertEqual(m.value, 0)
        self.assertEqual(m.range, ftk.RangeI(0, 100))
        m.value = 42
        self.assertEqual(m.value, 42)
        m.value = 200
        self.assertEqual(m.value, 100)
        m.value = -1
        self.assertEqual(m.value, 0)

    def test_range(self):
        m = ftk.IntModel()
        m.range = ftk.RangeI(0, 10)
        self.assertEqual(m.range, ftk.RangeI(0, 10))
        m.value = 100
        self.assertEqual(m.value, 10)

        # Reading the range returns a detached copy, not a reference into
        # the model.
        r = m.range
        self.assertIsNot(r, m.range)
        m.range = ftk.RangeI(0, 50)
        self.assertEqual(r, ftk.RangeI(0, 10))

    def test_step(self):
        m = ftk.IntModel()
        self.assertEqual(m.step, 1)
        self.assertEqual(m.largeStep, 10)
        m.step = 2
        m.largeStep = 20
        self.assertEqual(m.step, 2)
        self.assertEqual(m.largeStep, 20)

        m.range = ftk.RangeI(0, 10)
        m.step = 1
        m.largeStep = 10
        m.value = 5
        m.stepInc()
        self.assertEqual(m.value, 6)
        m.stepDec()
        self.assertEqual(m.value, 5)
        m.largeStepInc()
        self.assertEqual(m.value, 10)
        m.largeStepDec()
        self.assertEqual(m.value, 0)
        m.stepDec()
        self.assertEqual(m.value, 0)
        m.value = 10
        m.stepInc()
        self.assertEqual(m.value, 10)
        m.largeStepInc()
        self.assertEqual(m.value, 10)

    def test_default(self):
        m = ftk.IntModel()
        self.assertFalse(m.hasDefault)
        m.defaultValue = 3
        self.assertTrue(m.hasDefault)
        self.assertEqual(m.defaultValue, 3)
        m.value = 7
        m.setDefault()
        self.assertEqual(m.value, 3)
        m.clearDefault()
        self.assertFalse(m.hasDefault)

    def callbackValue(self, value):
        self.values.append(value)

    def test_observeValue(self):
        self.values = []
        m = ftk.IntModel()
        m.value = 1
        observer = ftk.IntObserver(m.observeValue(), self.callbackValue)
        self.assertEqual(self.values, [1])
        m.value = 2
        self.assertEqual(self.values, [1, 2])
        m.value = 2
        self.assertEqual(self.values, [1, 2])

    def callbackRange(self, value):
        self.ranges.append(value)

    def test_observeRange(self):
        self.ranges = []
        m = ftk.IntModel()
        observer = ftk.RangeIObserver(m.observeRange(), self.callbackRange)
        self.assertEqual(self.ranges, [ftk.RangeI(0, 100)])
        m.range = ftk.RangeI(0, 10)
        self.assertEqual(self.ranges, [ftk.RangeI(0, 100), ftk.RangeI(0, 10)])

class FloatModelTest(unittest.TestCase):

    def test_value(self):
        m = ftk.FloatModel()
        self.assertAlmostEqual(m.value, 0.0)
        self.assertEqual(m.range, ftk.RangeF(0.0, 1.0))
        m.value = 0.5
        self.assertAlmostEqual(m.value, 0.5)
        m.value = 2.0
        self.assertAlmostEqual(m.value, 1.0)

    def test_range(self):
        m = ftk.FloatModel()
        m.range = ftk.RangeF(0.0, 10.0)
        self.assertEqual(m.range, ftk.RangeF(0.0, 10.0))
        r = m.range
        self.assertIsNot(r, m.range)
        m.range = ftk.RangeF(0.0, 5.0)
        self.assertEqual(r, ftk.RangeF(0.0, 10.0))

    def test_step(self):
        m = ftk.FloatModel()
        m.range = ftk.RangeF(0.0, 10.0)
        m.step = 1.0
        m.largeStep = 5.0
        m.value = 5.0
        m.stepInc()
        self.assertAlmostEqual(m.value, 6.0)
        m.stepDec()
        self.assertAlmostEqual(m.value, 5.0)
        m.largeStepInc()
        self.assertAlmostEqual(m.value, 10.0)
        m.largeStepInc()
        self.assertAlmostEqual(m.value, 10.0)
        m.value = 0.0
        m.largeStepDec()
        self.assertAlmostEqual(m.value, 0.0)

    def test_default(self):
        m = ftk.FloatModel()
        self.assertFalse(m.hasDefault)
        m.defaultValue = 0.25
        self.assertTrue(m.hasDefault)
        self.assertAlmostEqual(m.defaultValue, 0.25)
        m.value = 0.75
        m.setDefault()
        self.assertAlmostEqual(m.value, 0.25)
        m.clearDefault()
        self.assertFalse(m.hasDefault)

    def callbackValue(self, value):
        self.values.append(value)

    def test_observeValue(self):
        self.values = []
        m = ftk.FloatModel()
        observer = ftk.FloatObserver(m.observeValue(), self.callbackValue)
        self.assertEqual(self.values, [0.0])
        m.value = 0.5
        self.assertEqual(self.values, [0.0, 0.5])

    def callbackRange(self, value):
        self.ranges.append(value)

    def test_observeRange(self):
        self.ranges = []
        m = ftk.FloatModel()
        observer = ftk.RangeFObserver(m.observeRange(), self.callbackRange)
        self.assertEqual(self.ranges, [ftk.RangeF(0.0, 1.0)])
        m.range = ftk.RangeF(0.0, 2.0)
        self.assertEqual(self.ranges, [ftk.RangeF(0.0, 1.0), ftk.RangeF(0.0, 2.0)])

class DoubleModelTest(unittest.TestCase):

    def test_value(self):
        m = ftk.DoubleModel()
        self.assertAlmostEqual(m.value, 0.0)
        self.assertEqual(m.range, ftk.RangeD(0.0, 1.0))
        m.value = 0.5
        self.assertAlmostEqual(m.value, 0.5)
        m.value = 2.0
        self.assertAlmostEqual(m.value, 1.0)

    def test_range(self):
        m = ftk.DoubleModel()
        m.range = ftk.RangeD(0.0, 10.0)
        self.assertEqual(m.range, ftk.RangeD(0.0, 10.0))
        r = m.range
        self.assertIsNot(r, m.range)
        m.range = ftk.RangeD(0.0, 5.0)
        self.assertEqual(r, ftk.RangeD(0.0, 10.0))

    def test_step(self):
        m = ftk.DoubleModel()
        m.range = ftk.RangeD(0.0, 10.0)
        m.step = 1.0
        m.largeStep = 5.0
        m.value = 5.0
        m.stepInc()
        self.assertAlmostEqual(m.value, 6.0)
        m.stepDec()
        self.assertAlmostEqual(m.value, 5.0)
        m.largeStepInc()
        self.assertAlmostEqual(m.value, 10.0)
        m.largeStepInc()
        self.assertAlmostEqual(m.value, 10.0)
        m.value = 0.0
        m.largeStepDec()
        self.assertAlmostEqual(m.value, 0.0)

    def test_default(self):
        m = ftk.DoubleModel()
        self.assertFalse(m.hasDefault)
        m.defaultValue = 0.25
        self.assertTrue(m.hasDefault)
        self.assertAlmostEqual(m.defaultValue, 0.25)
        m.value = 0.75
        m.setDefault()
        self.assertAlmostEqual(m.value, 0.25)
        m.clearDefault()
        self.assertFalse(m.hasDefault)

    def callbackValue(self, value):
        self.values.append(value)

    def test_observeValue(self):
        self.values = []
        m = ftk.DoubleModel()
        observer = ftk.DoubleObserver(m.observeValue(), self.callbackValue)
        self.assertEqual(self.values, [0.0])
        m.value = 0.5
        self.assertEqual(self.values, [0.0, 0.5])

    def callbackRange(self, value):
        self.ranges.append(value)

    def test_observeRange(self):
        self.ranges = []
        m = ftk.DoubleModel()
        observer = ftk.RangeDObserver(m.observeRange(), self.callbackRange)
        self.assertEqual(self.ranges, [ftk.RangeD(0.0, 1.0)])
        m.range = ftk.RangeD(0.0, 2.0)
        self.assertEqual(self.ranges, [ftk.RangeD(0.0, 1.0), ftk.RangeD(0.0, 2.0)])

class ActionGroupTest(unittest.TestCase):

    def test_enum(self):
        self.assertEqual(
            [i.name for i in ftk.ActionGroupType.__members__.values()],
            ["Click", "Check", "Radio", "Toggle"])

    def callbackChecked(self, value):
        self.checked.append(value)

    def test_radio(self):
        group = ftk.ActionGroup(ftk.ActionGroupType.Radio)
        a0 = ftk.Action("one", lambda: None)
        a1 = ftk.Action("two", lambda: None)
        group.addAction(a0)
        group.addAction(a1)
        self.assertEqual(len(group.actions), 2)
        self.assertEqual(group.checked, -1)

        self.checked = []
        observer = ftk.IntObserver(group.observeChecked(), self.callbackChecked)
        self.assertEqual(self.checked, [-1])

        self.callbacks = []
        group.setCheckedCallback(
            lambda index, value: self.callbacks.append((index, value)))
        group.checked = 1
        self.assertEqual(group.checked, 1)
        self.assertEqual(self.checked, [-1, 1])
        self.assertTrue(a1.checked)
        self.assertFalse(a0.checked)

        group.clear()
        self.assertEqual(len(group.actions), 0)

class ListItemTest(unittest.TestCase):

    def test_members(self):
        item = ftk.ListItem()
        self.assertEqual(item.text, "")
        self.assertEqual(item.tooltip, "")
        item = ftk.ListItem("text", "tooltip")
        self.assertEqual(item.text, "text")
        self.assertEqual(item.tooltip, "tooltip")

    def test_operators(self):
        self.assertEqual(ftk.ListItem("a", "b"), ftk.ListItem("a", "b"))
        self.assertNotEqual(ftk.ListItem("a", "b"), ftk.ListItem("c", "b"))
        self.assertNotEqual(ftk.ListItem("a", "b"), ftk.ListItem("a", "c"))

if __name__ == '__main__':
    unittest.main()
