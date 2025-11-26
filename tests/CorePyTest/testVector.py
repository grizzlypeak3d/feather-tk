# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class VectorTest(unittest.TestCase):

    def test_members(self):
        v2 = ftk.V2I()
        self.assertEqual(v2[0], 0)
        self.assertEqual(v2[1], 0)
        v3 = ftk.V3F()
        self.assertEqual(v3[0], 0)
        self.assertEqual(v3[1], 0)
        self.assertEqual(v3[2], 0)
        v4 = ftk.V4F()
        self.assertEqual(v4[0], 0)
        self.assertEqual(v4[1], 0)
        self.assertEqual(v4[2], 0)
        self.assertEqual(v4[3], 0)
        
        v2 = ftk.V2I()
        v2[0] = 1
        v2[1] = 2
        self.assertEqual(v2[0], 1)
        self.assertEqual(v2[1], 2)
        v3 = ftk.V3F()
        v3[0] = 1
        v3[1] = 2
        v3[2] = 3
        self.assertEqual(v3[0], 1)
        self.assertEqual(v3[1], 2)
        self.assertEqual(v3[2], 3)
        v4 = ftk.V4F()
        v4[0] = 1
        v4[1] = 2
        v4[2] = 3
        v4[3] = 4
        self.assertEqual(v4[0], 1)
        self.assertEqual(v4[1], 2)
        self.assertEqual(v4[2], 3)
        self.assertEqual(v4[3], 4)
        
        v2 = ftk.V2I(1, 2)
        self.assertEqual(v2.x, 1)
        self.assertEqual(v2.y, 2)
        v3 = ftk.V3F(1, 2, 3)
        self.assertEqual(v3.x, 1)
        self.assertEqual(v3.y, 2)
        self.assertEqual(v3.z, 3)
        v4 = ftk.V4F(1, 2, 3, 4)
        self.assertEqual(v4.x, 1)
        self.assertEqual(v4.y, 2)
        self.assertEqual(v4.z, 3)
        self.assertEqual(v4.w, 4)
        
        v2 = ftk.V2I()
        v2.x = 1;
        v2.y = 2;
        self.assertEqual(v2.x, 1)
        self.assertEqual(v2.y, 2)
        v3 = ftk.V3F()
        v3.x = 1
        v3.y = 2
        v3.z = 3
        self.assertEqual(v3.x, 1)
        self.assertEqual(v3.y, 2)
        self.assertEqual(v3.z, 3)
        v4 = ftk.V4F()
        v4.x = 1
        v4.y = 2
        v4.z = 3
        v4.w = 4
        self.assertEqual(v4.x, 1)
        self.assertEqual(v4.y, 2)
        self.assertEqual(v4.z, 3)
        self.assertEqual(v4.w, 4)

    def test_functions(self):
        l = ftk.length(ftk.V2F(0, 1))
        self.assertEqual(1, l)
        l = ftk.length(ftk.V2F(1, 0))
        self.assertEqual(1, l)
        
        v = ftk.normalize(ftk.V2F(0, 2))
        self.assertEqual(ftk.length(v), 1)
        v = ftk.normalize(ftk.V2F(2, 0))
        self.assertEqual(ftk.length(v), 1)
        
        d = ftk.dot(ftk.V2F(0, 1), ftk.V2F(1, 0))
        self.assertEqual(0, d)
        d = ftk.dot(ftk.V2F(0, 1), ftk.V2F(0, -1))
        self.assertEqual(-1, d)
        
        v = ftk.perpCW(ftk.V2F(1, 0))
        self.assertEqual(ftk.V2F(0, -1), v)
        v = ftk.perpCCW(ftk.V2F(1, 0))
        self.assertEqual(ftk.V2F(0, 1), v)

    def test_operators(self):
        v = ftk.V2F(1, 2) + ftk.V2F(3, 4)
        self.assertEqual(ftk.V2F(4, 6), v)
        v = ftk.V2F(1, 2) + 1
        self.assertEqual(ftk.V2F(2, 3), v)

        v = ftk.V2F(3, 4) - ftk.V2F(1, 2)
        self.assertEqual(ftk.V2F(2, 2), v)
        v = ftk.V2F(3, 4) - 1
        self.assertEqual(ftk.V2F(2, 3), v)
        
        self.assertEqual(ftk.V2F(1, 2), ftk.V2F(1, 2))
        self.assertNotEqual(ftk.V2F(1, 2), ftk.V2F(2, 1))
