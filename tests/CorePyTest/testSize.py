# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class SizeTest(unittest.TestCase):

    def test_members(self):
        s2 = ftk.Size2I()
        self.assertEqual(s2[0], 0)
        self.assertEqual(s2[1], 0)
        self.assertFalse(s2.valid)
        s3 = ftk.Size3F()
        self.assertEqual(s3[0], 0)
        self.assertEqual(s3[1], 0)
        self.assertEqual(s3[2], 0)
        self.assertFalse(s3.valid)
        
        s2 = ftk.Size2I(1, 2)
        self.assertEqual(s2.w, 1)
        self.assertEqual(s2.h, 2)
        s3 = ftk.Size3F(1, 2, 3)
        self.assertEqual(s3.w, 1)
        self.assertEqual(s3.h, 2)
        self.assertEqual(s3.d, 3)
        
        s2 = ftk.Size2I()
        s2[0] = 1
        s2[1] = 2
        self.assertEqual(s2[0], 1)
        self.assertEqual(s2[1], 2)
        s3 = ftk.Size3F()
        s3[0] = 1
        s3[1] = 2
        s3[2] = 3
        self.assertEqual(s3[0], 1)
        self.assertEqual(s3[1], 2)
        self.assertEqual(s3[2], 3)

    def test_functions(self):
        s2 = ftk.Size2I(2, 1)
        self.assertEqual(2, ftk.aspectRatio(s2))
        s2 = ftk.Size2F(2, 1)
        self.assertEqual(2, ftk.aspectRatio(s2))
        
        s2 = ftk.Size2F(1, 2)
        self.assertEqual(2, ftk.area(s2))
        
        s3 = ftk.Size3F(1, 2, 3)
        self.assertEqual(6, ftk.volume(s3))

    def test_operators(self):
        s2 = ftk.Size2I(1, 2)
        s2a = s2 + 1
        s2b = s2 - 1
        self.assertEqual(ftk.Size2I(2, 3), s2a)
        self.assertEqual(ftk.Size2I(0, 1), s2b)

        self.assertEqual(ftk.Size2I(1, 2), ftk.Size2I(1, 2))
        self.assertNotEqual(ftk.Size2I(1, 2), ftk.Size2I(2, 1))
