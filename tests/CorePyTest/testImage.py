# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import ftkPy as ftk

import unittest

class ImageTest(unittest.TestCase):

    def test_image_type_enum(self):
        self.assertIsNotNone(ftk.ImageType.L_U8)
        self.assertIsNotNone(ftk.ImageType.RGBA_U8)
        self.assertIsNotNone(ftk.ImageType.RGB_F32)
        label = ftk.getLabel(ftk.ImageType.L_U8)
        self.assertIsInstance(label, str)

    def test_channel_count(self):
        self.assertEqual(ftk.getChannelCount(ftk.ImageType.L_U8), 1)
        self.assertEqual(ftk.getChannelCount(ftk.ImageType.LA_U8), 2)
        self.assertEqual(ftk.getChannelCount(ftk.ImageType.RGB_U8), 3)
        self.assertEqual(ftk.getChannelCount(ftk.ImageType.RGBA_U8), 4)

    def test_bit_depth(self):
        self.assertEqual(ftk.getBitDepth(ftk.ImageType.L_U8), 8)
        self.assertEqual(ftk.getBitDepth(ftk.ImageType.L_U16), 16)
        self.assertEqual(ftk.getBitDepth(ftk.ImageType.L_U32), 32)

    def test_image_mirror(self):
        m = ftk.ImageMirror()
        self.assertFalse(m.x)
        self.assertFalse(m.y)

        m = ftk.ImageMirror(True, False)
        self.assertTrue(m.x)
        self.assertFalse(m.y)
        m.y = True
        self.assertTrue(m.y)

        self.assertEqual(ftk.ImageMirror(True, True), ftk.ImageMirror(True, True))
        self.assertNotEqual(ftk.ImageMirror(True, False), ftk.ImageMirror(False, True))

    def test_image_info(self):
        info = ftk.ImageInfo()
        self.assertFalse(info.valid)

        info = ftk.ImageInfo(ftk.Size2I(320, 240), ftk.ImageType.RGBA_U8)
        self.assertTrue(info.valid)
        self.assertEqual(info.size, ftk.Size2I(320, 240))
        self.assertEqual(info.type, ftk.ImageType.RGBA_U8)
        self.assertAlmostEqual(info.aspect, 320 / 240)
        self.assertEqual(info.byteCount, 320 * 240 * 4)

        info2 = ftk.ImageInfo(320, 240, ftk.ImageType.RGBA_U8)
        self.assertEqual(info, info2)

    def test_image_create(self):
        img = ftk.Image(ftk.Size2I(64, 64), ftk.ImageType.RGBA_U8)
        self.assertTrue(img.valid)
        self.assertEqual(img.width, 64)
        self.assertEqual(img.height, 64)
        self.assertAlmostEqual(img.aspect, 1.0)
        self.assertEqual(img.type, ftk.ImageType.RGBA_U8)
        self.assertEqual(img.byteCount, 64 * 64 * 4)

        img2 = ftk.Image(64, 64, ftk.ImageType.RGB_U8)
        self.assertTrue(img2.valid)
        self.assertEqual(img2.type, ftk.ImageType.RGB_U8)

    def test_image_zero(self):
        img = ftk.Image(4, 4, ftk.ImageType.L_U8)
        img.zero()

    def test_image_info_from_create(self):
        info = ftk.ImageInfo(128, 64, ftk.ImageType.RGB_F32)
        img = ftk.Image(info)
        self.assertEqual(img.info, info)
        self.assertEqual(img.size, info.size)

    def test_image_tags(self):
        img = ftk.Image(4, 4, ftk.ImageType.L_U8)
        img.tags = { "foo": "bar" }
        self.assertEqual(img.tags, { "foo": "bar" })

if __name__ == '__main__':
    unittest.main()
