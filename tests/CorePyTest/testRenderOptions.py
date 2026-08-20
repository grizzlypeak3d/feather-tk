# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class RenderOptionsTest(unittest.TestCase):

    def test_renderOptions(self):
        o = ftk.RenderOptions()
        self.assertTrue(o.clear)
        self.assertEqual(o.clearColor, ftk.Color4F(0, 0, 0, 0))
        self.assertTrue(o.texturePoolByteCount > 0)
        self.assertTrue(o.textureCacheByteCount > 0)
        self.assertTrue(o.glyphAtlasSize > 0)
        self.assertTrue(o.log)
        self.assertEqual(o, ftk.RenderOptions())
        o.clear = False
        self.assertNotEqual(o, ftk.RenderOptions())

    def test_lineOptions(self):
        o = ftk.LineOptions()
        self.assertAlmostEqual(o.width, 1.0)
        self.assertEqual(o, ftk.LineOptions())
        o.width = 2.0
        self.assertNotEqual(o, ftk.LineOptions())

    def test_imageFilters(self):
        o = ftk.ImageFilters()
        self.assertEqual(o.minify, ftk.ImageFilter.Linear)
        self.assertEqual(o.magnify, ftk.ImageFilter.Linear)
        self.assertEqual(o, ftk.ImageFilters())
        o.magnify = ftk.ImageFilter.Nearest
        self.assertNotEqual(o, ftk.ImageFilters())

    def test_imageOptions(self):
        o = ftk.ImageOptions()
        self.assertEqual(o.channelDisplay, ftk.ChannelDisplay.Color)
        self.assertEqual(o.videoLevels, ftk.InputVideoLevels.FromFile)
        self.assertEqual(o.alphaBlend, ftk.AlphaBlend.Straight)
        self.assertEqual(o.imageFilters, ftk.ImageFilters())
        self.assertTrue(o.cache)
        self.assertEqual(o, ftk.ImageOptions())
        o.alphaBlend = ftk.AlphaBlend.Premultiplied
        self.assertNotEqual(o, ftk.ImageOptions())

    def test_enums(self):
        self.assertEqual(
            [i.name for i in ftk.ChannelDisplay.__members__.values()],
            ["Color", "Red", "Green", "Blue", "Alpha"])
        self.assertEqual(
            [i.name for i in ftk.InputVideoLevels.__members__.values()],
            ["FromFile", "FullRange", "LegalRange"])
        self.assertEqual(
            [i.name for i in ftk.AlphaBlend.__members__.values()],
            ["_None", "Straight", "Premultiplied"])
        self.assertEqual(
            [i.name for i in ftk.ImageFilter.__members__.values()],
            ["Nearest", "Linear", "HighQuality"])

if __name__ == '__main__':
    unittest.main()
