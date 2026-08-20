# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class NoiseTest(unittest.TestCase):

    def test_get(self):
        noise = ftk.Noise()
        for i in range(50):
            v = noise.get(i * 0.13, i * 0.29, i * 0.41)
            self.assertIsInstance(v, float)
            self.assertGreaterEqual(v, -1.0)
            self.assertLessEqual(v, 1.0)

    def test_deterministic(self):
        a = ftk.Noise()
        b = ftk.Noise()
        for i in range(10):
            x, y, z = i * 0.7, i * 0.3, i * 0.5
            self.assertEqual(a.get(x, y, z), a.get(x, y, z))
            self.assertEqual(a.get(x, y, z), b.get(x, y, z))

if __name__ == '__main__':
    unittest.main()
