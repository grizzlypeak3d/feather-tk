# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class RangeTest(unittest.TestCase):

    def test_members(self):
        r = ftk.RangeI()
        self.assertEqual(r.min, 0)
        self.assertEqual(r.max, 0)
        self.assertTrue(r.equal)

        r = ftk.RangeI(1, 10)
        self.assertEqual(r.min, 1)
        self.assertEqual(r.max, 10)
        self.assertFalse(r.equal)

        r = ftk.RangeI64(0, 100)
        self.assertEqual(r.min, 0)
        self.assertEqual(r.max, 100)

        r = ftk.RangeF(0.0, 1.0)
        self.assertAlmostEqual(r.min, 0.0)
        self.assertAlmostEqual(r.max, 1.0)

        r = ftk.RangeD(0.0, 1.0)
        self.assertAlmostEqual(r.min, 0.0)
        self.assertAlmostEqual(r.max, 1.0)

    def test_contains(self):
        r = ftk.RangeI(0, 10)
        self.assertTrue(ftk.contains(r, 5))
        self.assertTrue(ftk.contains(r, 0))
        self.assertTrue(ftk.contains(r, 10))
        self.assertFalse(ftk.contains(r, -1))
        self.assertFalse(ftk.contains(r, 11))

        r = ftk.RangeF(0.0, 1.0)
        self.assertTrue(ftk.contains(r, 0.5))
        self.assertFalse(ftk.contains(r, 1.5))

    def test_intersects(self):
        r = ftk.RangeI(0, 10)
        self.assertTrue(ftk.intersects(r, ftk.RangeI(5, 15)))
        self.assertTrue(ftk.intersects(r, ftk.RangeI(-5, 5)))
        self.assertFalse(ftk.intersects(r, ftk.RangeI(11, 20)))
        self.assertFalse(ftk.intersects(r, ftk.RangeI(-10, -1)))

    def test_expand(self):
        r = ftk.RangeI(2, 8)
        expanded = ftk.expand(r, 15)
        self.assertEqual(expanded.min, 2)
        self.assertEqual(expanded.max, 15)
        expanded = ftk.expand(r, -1)
        self.assertEqual(expanded.min, -1)
        self.assertEqual(expanded.max, 8)

    def test_operators(self):
        self.assertEqual(ftk.RangeI(0, 10), ftk.RangeI(0, 10))
        self.assertNotEqual(ftk.RangeI(0, 10), ftk.RangeI(0, 9))
        self.assertLess(ftk.RangeI(0, 5), ftk.RangeI(6, 10))

    def test_repr(self):
        r = ftk.RangeI(1, 10)
        s = repr(r)
        self.assertIsInstance(s, str)
        self.assertTrue(len(s) > 0)

if __name__ == '__main__':
    unittest.main()
