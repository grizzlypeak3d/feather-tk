# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import math
import unittest

class MathTest(unittest.TestCase):

    def test_constants(self):
        self.assertAlmostEqual(ftk.pi, math.pi, places=6)
        self.assertAlmostEqual(ftk.pi2, math.pi * 2.0, places=6)

    def test_deg2rad(self):
        self.assertAlmostEqual(ftk.deg2rad(180.0), math.pi, places=6)
        for i in [0.0, 90.0, 180.0, 270.0]:
            self.assertAlmostEqual(ftk.rad2deg(ftk.deg2rad(i)), i, places=4)

    def test_clamp(self):
        self.assertEqual(ftk.clamp(-1, 0, 1), 0)
        self.assertEqual(ftk.clamp(0, 0, 1), 0)
        self.assertEqual(ftk.clamp(1, 0, 1), 1)
        self.assertEqual(ftk.clamp(2, 0, 1), 1)
        self.assertAlmostEqual(ftk.clamp(2.5, 0.0, 1.0), 1.0)
        self.assertAlmostEqual(ftk.clamp(0.5, 0.0, 1.0), 0.5)

    def test_lerp(self):
        self.assertAlmostEqual(ftk.lerp(0.0, 0.0, 9.0), 0.0)
        self.assertAlmostEqual(ftk.lerp(1.0, 0.0, 9.0), 9.0)
        self.assertAlmostEqual(ftk.lerp(0.5, 0.0, 9.0), 4.5)
        self.assertEqual(ftk.lerp(0.5, 0, 10), 5)

    def test_smoothStep(self):
        self.assertAlmostEqual(ftk.smoothStep(0.0, 0.0, 1.0), 0.0)
        self.assertAlmostEqual(ftk.smoothStep(1.0, 0.0, 1.0), 1.0)
        self.assertAlmostEqual(ftk.smoothStep(0.5, 0.0, 1.0), 0.5)
        self.assertLess(ftk.smoothStep(0.25, 0.0, 1.0), 0.25)
        self.assertGreater(ftk.smoothStep(0.75, 0.0, 1.0), 0.75)

    def test_digits(self):
        self.assertEqual(ftk.digits(0), 1)
        self.assertEqual(ftk.digits(10), 2)
        self.assertEqual(ftk.digits(100), 3)
        self.assertEqual(ftk.digits(1000), 4)
        self.assertEqual(ftk.digits(-1000), 4)

    def test_fuzzyCompare(self):
        self.assertTrue(ftk.fuzzyCompare(1.0, 1.00000000001))
        self.assertFalse(ftk.fuzzyCompare(1.0, 1.1))
        self.assertTrue(ftk.fuzzyCompare(1.0, 1.05, 0.1))
        self.assertFalse(ftk.fuzzyCompare(1.0, 1.05, 0.01))

if __name__ == '__main__':
    unittest.main()
