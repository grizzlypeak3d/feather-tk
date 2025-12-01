# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class ObservableValueTest(unittest.TestCase):

    def callbackInt(self, value):
        self.valueI = value

    def test_value(self):
        self.valueI = 0
        ovalueI = ftk.ObservableInt(0)
        observerI = ftk.IntObserver(ovalueI, self.callbackInt)
        self.assertTrue(ovalueI.setIfChanged(1))
        self.assertFalse(ovalueI.setIfChanged(1))
        self.assertEqual(self.valueI, 1)
        ovalueI.setAlways(2)
        self.assertEqual(self.valueI, 2)

    def callbackIntList(self, value):
        self.listI = value

    def test_list(self):
        self.listI = []
        olistI = ftk.ObservableIntList([])
        observerI = ftk.IntListObserver(olistI, self.callbackIntList)
        self.assertTrue(olistI.setIfChanged([ 1 ]))
        self.assertFalse(olistI.setIfChanged([ 1 ]))
        self.assertEqual(self.listI, [ 1 ])
        olistI.setAlways([ 1, 2 ])
        self.assertEqual(self.listI, [ 1, 2 ])

    def callbackStringIntMap(self, value):
        self.mapStringI = value

    def test_map(self):
        self.mapStringI = {}
        omapStringI = ftk.ObservableStringIntMap({})
        observerI = ftk.StringIntMapObserver(omapStringI, self.callbackStringIntMap)
        self.assertTrue(omapStringI.setIfChanged({ "a": 1 }))
        self.assertFalse(omapStringI.setIfChanged({ "a": 1 }))
        self.assertEqual(self.mapStringI, { "a": 1 })
        omapStringI.setAlways({ "a": 2 })
        self.assertEqual(self.mapStringI, { "a": 2 })
