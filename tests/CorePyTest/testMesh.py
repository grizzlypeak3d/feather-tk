# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class MeshTest(unittest.TestCase):

    def test_members(self):
        v = ftk.Vertex2()
        self.assertEqual(v.v, 0)
        self.assertEqual(v.t, 0)
        self.assertEqual(v.c, 0)
        v = ftk.Vertex2(1, 2, 3)
        self.assertEqual(v.v, 1)
        self.assertEqual(v.t, 2)
        self.assertEqual(v.c, 3)

        v = ftk.Vertex3()
        self.assertEqual(v.v, 0)
        self.assertEqual(v.t, 0)
        self.assertEqual(v.n, 0)
        self.assertEqual(v.c, 0)
        v = ftk.Vertex3(1, 2, 3, 4)
        self.assertEqual(v.v, 1)
        self.assertEqual(v.t, 2)
        self.assertEqual(v.n, 3)
        self.assertEqual(v.c, 4)

        t = ftk.Triangle2(ftk.Vertex2(1), ftk.Vertex2(2), ftk.Vertex2(3))
        self.assertEqual([i.v for i in t.v], [1, 2, 3])
        t = ftk.Triangle3(ftk.Vertex3(1), ftk.Vertex3(2), ftk.Vertex3(3))
        self.assertEqual([i.v for i in t.v], [1, 2, 3])

        m = ftk.TriMesh2F()
        self.assertEqual(len(m.v), 0)
        self.assertEqual(len(m.triangles), 0)
        m.v = [ftk.V2F(0, 0), ftk.V2F(1, 0), ftk.V2F(0, 1)]
        m.triangles = [ftk.Triangle2(ftk.Vertex2(1), ftk.Vertex2(2), ftk.Vertex2(3))]
        self.assertEqual(len(m.v), 3)
        self.assertEqual(len(m.triangles), 1)
        self.assertTrue(m.getByteCount() > 0)

        m = ftk.TriMesh3F()
        self.assertEqual(len(m.v), 0)
        self.assertEqual(len(m.n), 0)
        self.assertEqual(len(m.triangles), 0)

    def test_functions(self):
        e = ftk.edge(ftk.V2F(0, 1), ftk.V2F(0, 0), ftk.V2F(2, 0))
        self.assertAlmostEqual(e, -2.0)

        m = ftk.mesh(ftk.Box2I(0, 0, 2, 2))
        self.assertEqual(len(m.v), 4)
        self.assertEqual(len(m.triangles), 2)
        m = ftk.mesh(ftk.Box2I(0, 0, 2, 2), True)
        self.assertEqual(len(m.v), 4)
        m = ftk.mesh(ftk.Box2F(0, 0, 2, 2))
        self.assertEqual(len(m.v), 4)
        self.assertEqual(len(m.triangles), 2)

        m = ftk.checkers(
            ftk.Box2I(0, 0, 100, 100),
            ftk.Color4F(0, 0, 0),
            ftk.Color4F(1, 1, 1),
            ftk.Size2I(10, 10))
        self.assertEqual(len(m.triangles), 200)

        m = ftk.sphere(1.0, 8, 8)
        self.assertTrue(len(m.v) > 0)
        self.assertTrue(len(m.triangles) > 0)
        b = ftk.bbox(m)
        self.assertAlmostEqual(b.min.x, -1.0, places=5)
        self.assertAlmostEqual(b.min.y, -1.0, places=5)
        self.assertAlmostEqual(b.min.z, -1.0, places=5)
        self.assertAlmostEqual(b.max.x, 1.0, places=5)
        self.assertAlmostEqual(b.max.y, 1.0, places=5)
        self.assertAlmostEqual(b.max.z, 1.0, places=5)

if __name__ == '__main__':
    unittest.main()
