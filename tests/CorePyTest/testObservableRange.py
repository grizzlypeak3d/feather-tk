# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class ObservableRangeTest(unittest.TestCase):

    def callbackRangeI(self, value):
        self.rangeI = value

    def test_rangeI(self):
        self.rangeI = None
        ovalue = ftk.ObservableRangeI(ftk.RangeI(0, 10))
        observer = ftk.RangeIObserver(ovalue, self.callbackRangeI)
        self.assertEqual(self.rangeI, ftk.RangeI(0, 10))
        self.assertTrue(ovalue.setIfChanged(ftk.RangeI(0, 20)))
        self.assertFalse(ovalue.setIfChanged(ftk.RangeI(0, 20)))
        self.assertEqual(self.rangeI, ftk.RangeI(0, 20))
        ovalue.setAlways(ftk.RangeI(0, 30))
        self.assertEqual(self.rangeI, ftk.RangeI(0, 30))
        self.assertEqual(ovalue.get(), ftk.RangeI(0, 30))

    def callbackRangeF(self, value):
        self.rangeF = value

    def test_rangeF(self):
        self.rangeF = None
        ovalue = ftk.ObservableRangeF(ftk.RangeF(0.0, 1.0))
        observer = ftk.RangeFObserver(ovalue, self.callbackRangeF)
        self.assertEqual(self.rangeF, ftk.RangeF(0.0, 1.0))
        self.assertTrue(ovalue.setIfChanged(ftk.RangeF(0.0, 2.0)))
        self.assertFalse(ovalue.setIfChanged(ftk.RangeF(0.0, 2.0)))
        self.assertEqual(self.rangeF, ftk.RangeF(0.0, 2.0))

    def callbackRangeD(self, value):
        self.rangeD = value

    def test_rangeD(self):
        self.rangeD = None
        ovalue = ftk.ObservableRangeD(ftk.RangeD(0.0, 1.0))
        observer = ftk.RangeDObserver(ovalue, self.callbackRangeD)
        self.assertEqual(self.rangeD, ftk.RangeD(0.0, 1.0))
        self.assertTrue(ovalue.setIfChanged(ftk.RangeD(0.0, 2.0)))
        self.assertFalse(ovalue.setIfChanged(ftk.RangeD(0.0, 2.0)))
        self.assertEqual(self.rangeD, ftk.RangeD(0.0, 2.0))

if __name__ == '__main__':
    unittest.main()
