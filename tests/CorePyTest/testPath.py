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

    def test_frameSeq(self):
        seq = ftk.toFrameSeq([ 1, 2, 3, 10, 11, 20 ])
        self.assertEqual(
            [ (s.range.min, s.range.max, s.inc) for s in seq ],
            [ (1, 3, 1), (10, 11, 1), (20, 20, 1) ])
        self.assertEqual(ftk.getFrameCount(seq), 6)
        self.assertEqual(ftk.toFrames(seq), [ 1, 2, 3, 10, 11, 20 ])
        self.assertEqual(ftk.getLabel(seq), "1-3,10-11,20")
        self.assertEqual(ftk.getLabel(seq, 4), "0001-0003,0010-0011,0020")

        bounds = ftk.getRange(seq)
        self.assertEqual(bounds.min, 1)
        self.assertEqual(bounds.max, 20)
        self.assertIsNone(ftk.getRange([]))

        # addFrame() returns a new list rather than modifying in place.
        self.assertEqual(ftk.getLabel(ftk.addFrame(seq, 12)), "1-3,10-12,20")

    def test_pathSeq(self):
        path = ftk.Path("/tmp/render.0001.exr")
        self.assertEqual(
            [ (s.range.min, s.range.max, s.inc) for s in path.frameSeq ],
            [ (1, 1, 1) ])
        self.assertEqual(path.seqSize, 1)
        self.assertFalse(path.isPartialSeq)

        # The frameSeq property and the seq() predicate are separate members
        # that both have to stay reachable.
        self.assertTrue(path.seq(ftk.Path("/tmp/render.0002.exr")))
        self.assertFalse(path.seq(ftk.Path("/tmp/other.0002.exr")))

        path.frameSeq = ftk.toFrameSeq([ 1, 2, 3, 5 ])
        self.assertEqual(path.seqSize, 4)
        self.assertTrue(path.isSeq)
        self.assertTrue(path.isPartialSeq)
        self.assertEqual(path.getFrameRange(), "0001-0005")
        self.assertEqual(path.frames.min, 1)
        self.assertEqual(path.frames.max, 5)

