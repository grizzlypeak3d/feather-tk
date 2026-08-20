# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class MatrixTest(unittest.TestCase):

    def assertMatrixAlmostEqual(self, a, b, dim):
        for row in range(dim):
            for column in range(dim):
                self.assertAlmostEqual(a.get(row, column), b.get(row, column), places=3)

    def test_members(self):
        m = ftk.M33F()
        self.assertEqual(m.get(0, 0), 1.0)
        self.assertEqual(m.get(0, 1), 0.0)
        m.set(0, 0, 0.0)
        self.assertEqual(m.get(0, 0), 0.0)
        m = ftk.M33F(
            1.0, 2.0, 3.0,
            4.0, 5.0, 6.0,
            7.0, 8.0, 9.0)
        self.assertEqual(m.get(2, 2), 9.0)
        self.assertEqual(m[8], 9.0)
        m[0] = 10.0
        self.assertEqual(m.get(0, 0), 10.0)

        m = ftk.M44F()
        self.assertEqual(m.get(0, 0), 1.0)
        self.assertEqual(m.get(0, 1), 0.0)
        m.set(0, 0, 0.0)
        self.assertEqual(m.get(0, 0), 0.0)
        m = ftk.M44F(
            1.0, 2.0, 3.0, 4.0,
            5.0, 6.0, 7.0, 8.0,
            9.0, 10.0, 11.0, 12.0,
            13.0, 14.0, 15.0, 16.0)
        self.assertEqual(m.get(3, 3), 16.0)
        self.assertEqual(m[15], 16.0)
        m[0] = 10.0
        self.assertEqual(m.get(0, 0), 10.0)

    def test_functions(self):
        m = ftk.translate(ftk.V2F(1.0, 2.0))
        self.assertIsInstance(m, ftk.M33F)
        m = ftk.translate(ftk.V3F(1.0, 2.0, 3.0))
        self.assertIsInstance(m, ftk.M44F)

        self.assertMatrixAlmostEqual(
            ftk.rotate(90.0, ftk.V3F(1.0, 0.0, 0.0)), ftk.rotateX(90.0), 4)
        self.assertMatrixAlmostEqual(
            ftk.rotate(90.0, ftk.V3F(0.0, 1.0, 0.0)), ftk.rotateY(90.0), 4)
        self.assertMatrixAlmostEqual(
            ftk.rotate(90.0, ftk.V3F(0.0, 0.0, 1.0)), ftk.rotateZ(90.0), 4)
        self.assertMatrixAlmostEqual(
            ftk.rotateXYZ(ftk.V3F(0.0, 90.0, 0.0)), ftk.rotateY(90.0), 4)

        angles = ftk.V3F(30.0, 40.0, 50.0)
        m = ftk.rotateXYZ(angles)
        back = ftk.getRotateXYZ(m, angles)
        self.assertAlmostEqual(back.x, angles.x, places=3)
        self.assertAlmostEqual(back.y, angles.y, places=3)
        self.assertAlmostEqual(back.z, angles.z, places=3)

        s = ftk.scale(ftk.V3F(2.0, 3.0, 4.0)) * ftk.V3F(1.0, 1.0, 1.0)
        self.assertAlmostEqual(s.x, 2.0)
        self.assertAlmostEqual(s.y, 3.0)
        self.assertAlmostEqual(s.z, 4.0)

        m = ftk.ortho(0.0, 1.0, 0.0, 1.0, 0.1, 10000.0)
        self.assertAlmostEqual(m.get(0, 0), 2.0)

        m = ftk.perspective(60.0, 1.0, 0.1, 10000.0)
        self.assertAlmostEqual(m.get(0, 0), 1.7320508, places=4)
        self.assertAlmostEqual(m.get(3, 2), -1.0)

        m = ftk.M44F()
        self.assertTrue(ftk.invert(ftk.translate(ftk.V3F(1.0, 2.0, 3.0)), m))
        v = m * ftk.V3F(1.0, 2.0, 3.0)
        self.assertAlmostEqual(v.x, 0.0)
        self.assertAlmostEqual(v.y, 0.0)
        self.assertAlmostEqual(v.z, 0.0)

    def test_operators(self):
        self.assertEqual(ftk.M33F() * ftk.M33F(), ftk.M33F())
        self.assertEqual(ftk.M44F() * ftk.M44F(), ftk.M44F())
        self.assertEqual(
            ftk.translate(ftk.V3F(1.0, 2.0, 3.0)) * ftk.translate(ftk.V3F(-1.0, -2.0, -3.0)),
            ftk.M44F())

        v2 = ftk.translate(ftk.V2F(-1.0, -2.0)) * ftk.V2F(1.0, 2.0)
        self.assertEqual(v2, ftk.V2F(0.0, 0.0))

        v3 = ftk.translate(ftk.V3F(-1.0, -2.0, -3.0)) * ftk.V3F(1.0, 2.0, 3.0)
        self.assertEqual(v3, ftk.V3F(0.0, 0.0, 0.0))

        v4 = ftk.rotateZ(90.0) * ftk.V4F(1.0, 0.0, 0.0, 1.0)
        self.assertAlmostEqual(v4.x, 0.0, places=5)
        self.assertAlmostEqual(v4.y, 1.0, places=5)
        self.assertAlmostEqual(v4.z, 0.0, places=5)
        self.assertAlmostEqual(v4.w, 1.0, places=5)

        a = ftk.M44F()
        b = ftk.M44F()
        self.assertEqual(a, b)
        b.set(0, 0, 0.0)
        self.assertNotEqual(a, b)

    def test_repr(self):
        self.assertTrue(len(repr(ftk.M33F())) > 0)
        self.assertTrue(len(repr(ftk.M44F())) > 0)

if __name__ == '__main__':
    unittest.main()
