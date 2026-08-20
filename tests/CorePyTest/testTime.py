# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class TimeTest(unittest.TestCase):

    def test_toRational(self):
        self.assertEqual(ftk.toRational(23.976), (24000, 1001))
        self.assertEqual(ftk.toRational(24.0), (24, 1))
        self.assertEqual(ftk.toRational(25.0), (25, 1))
        self.assertEqual(ftk.toRational(29.97), (30000, 1001))
        self.assertEqual(ftk.toRational(30.0), (30, 1))
        self.assertEqual(ftk.toRational(59.94), (60000, 1001))
        self.assertEqual(ftk.toRational(60.0), (60, 1))
        self.assertEqual(ftk.toRational(120.0), (120, 1))

if __name__ == '__main__':
    unittest.main()
