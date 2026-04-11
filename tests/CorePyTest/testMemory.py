# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class MemoryTest(unittest.TestCase):

    def test_constants(self):
        self.assertEqual(ftk.kilobyte, 1024)
        self.assertEqual(ftk.megabyte, 1024 * 1024)
        self.assertEqual(ftk.gigabyte, 1024 * 1024 * 1024)
        self.assertEqual(ftk.terabyte, 1024 * 1024 * 1024 * 1024)

    def test_endian(self):
        endian = ftk.getEndian()
        self.assertIn(endian, [ftk.Endian.MSB, ftk.Endian.LSB])

        opposite = ftk.oppositeEndian(ftk.Endian.MSB)
        self.assertEqual(opposite, ftk.Endian.LSB)
        opposite = ftk.oppositeEndian(ftk.Endian.LSB)
        self.assertEqual(opposite, ftk.Endian.MSB)

    def test_endian_enum(self):
        self.assertEqual(ftk.getLabel(ftk.Endian.MSB), "MSB")
        self.assertEqual(ftk.getLabel(ftk.Endian.LSB), "LSB")

        endian = ftk.Endian.MSB
        s = ftk.to_string(endian)
        self.assertIsInstance(s, str)

if __name__ == '__main__':
    unittest.main()
