# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class PathTest(unittest.TestCase):

    def test_util(self):
        self.assertTrue(ftk.isDotFile(".profile"))
        
        self.assertEqual(ftk.split("/tmp/render"), [ "/", "tmp", "render" ])
        
        print("Drives:", ftk.getDrives())
        
        print("Home:", ftk.getUserPath(ftk.UserPath.Home))
        print("Desktop:", ftk.getUserPath(ftk.UserPath.Desktop))
        print("Documents:", ftk.getUserPath(ftk.UserPath.Documents))
        print("Downloads:", ftk.getUserPath(ftk.UserPath.Downloads))
        
        self.assertEqual(ftk.toString(100), "100")
        self.assertEqual(ftk.toString(100, 6), "000100")

    def test_path(self):
        path = ftk.Path("file:///tmp/render.1.exr?user=foo;password=bar")
        self.assertEqual("file://", path.protocol)
        self.assertEqual("/tmp/", path.dir)
        self.assertEqual("render.", path.base)
        self.assertEqual("1", path.num)
        self.assertEqual(".exr", path.ext)
        self.assertEqual("file://", path.protocol)
        self.assertEqual("?user=foo;password=bar", path.request)
        self.assertEqual("render.1.exr", path.fileName)
        path.protocol = "http://"
        self.assertEqual(path.get(), "http:///tmp/render.1.exr?user=foo;password=bar")
        path.dir = "/media/"
        self.assertEqual(path.get(), "http:///media/render.1.exr?user=foo;password=bar")
        path.base = "lighting"
        self.assertEqual(path.get(), "http:///media/lighting1.exr?user=foo;password=bar")
        path.num = "100"
        self.assertEqual(path.get(), "http:///media/lighting100.exr?user=foo;password=bar")
        path.pad = 6
        self.assertEqual(path.get(), "http:///media/lighting000100.exr?user=foo;password=bar")
        path.ext = ".tiff"
        self.assertEqual(path.get(), "http:///media/lighting000100.tiff?user=foo;password=bar")
        path.request = ""
        self.assertEqual(path.get(), "http:///media/lighting000100.tiff")

