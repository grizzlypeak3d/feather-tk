# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class ObservableValueTest(unittest.TestCase):

    def callbackI(self, value):
        self.valueI = value

    def test_value(self):
        self.valueI = 0
        ovalueI = ftk.ObservableValueI(0)
        observerI = ftk.ValueObserverI(ovalueI, self.callbackI)
        self.assertTrue(ovalueI.setIfChanged(1))
        self.assertFalse(ovalueI.setIfChanged(1))
        self.assertEqual(self.valueI, 1)
        ovalueI.setAlways(2)
        self.assertEqual(self.valueI, 2)

    def callbackListI(self, value):
        self.listI = value

    def test_list(self):
        self.listI = []
        olistI = ftk.ObservableListI([])
        observerI = ftk.ListObserverI(olistI, self.callbackListI)
        self.assertTrue(olistI.setIfChanged([ 1 ]))
        self.assertFalse(olistI.setIfChanged([ 1 ]))
        self.assertEqual(self.listI, [ 1 ])
        olistI.setAlways([ 1, 2 ])
        self.assertEqual(self.listI, [ 1, 2 ])

    def callbackMapStringI(self, value):
        self.mapStringI = value

    def test_map(self):
        self.mapStringI = {}
        omapStringI = ftk.ObservableMapStringI({})
        observerI = ftk.MapObserverStringI(omapStringI, self.callbackMapStringI)
        self.assertTrue(omapStringI.setIfChanged({ "a": 1 }))
        self.assertFalse(omapStringI.setIfChanged({ "a": 1 }))
        self.assertEqual(self.mapStringI, { "a": 1 })
        omapStringI.setAlways({ "a": 2 })
        self.assertEqual(self.mapStringI, { "a": 2 })
