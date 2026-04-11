# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class RandomTest(unittest.TestCase):

    def test_getF(self):
        r = ftk.Random()
        # Default: [0, 1)
        for _ in range(100):
            v = r.getF()
            self.assertGreaterEqual(v, 0.0)
            self.assertLess(v, 1.0)

        # With max
        for _ in range(100):
            v = r.getF(2.0)
            self.assertGreaterEqual(v, 0.0)
            self.assertLess(v, 2.0)

        # With min and max
        for _ in range(100):
            v = r.getF(-1.0, 1.0)
            self.assertGreaterEqual(v, -1.0)
            self.assertLess(v, 1.0)

    def test_getI(self):
        r = ftk.Random()
        # With max
        for _ in range(100):
            v = r.getI(10)
            self.assertGreaterEqual(v, 0)
            self.assertLessEqual(v, 10)

        # With min and max
        for _ in range(100):
            v = r.getI(5, 10)
            self.assertGreaterEqual(v, 5)
            self.assertLessEqual(v, 10)

    def test_seed(self):
        r = ftk.Random()
        r.setSeed(42)
        v1 = r.getF()
        r.setSeed(42)
        v2 = r.getF()
        self.assertEqual(v1, v2)

        # setSeed() with no arg should not raise
        r.setSeed()

    def test_determinism(self):
        r1 = ftk.Random()
        r2 = ftk.Random()
        r1.setSeed(123)
        r2.setSeed(123)
        for _ in range(20):
            self.assertEqual(r1.getF(), r2.getF())

if __name__ == '__main__':
    unittest.main()
