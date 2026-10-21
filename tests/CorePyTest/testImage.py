# SPDX-License-Identifier: BSD-3-Clause
# Copyright Contributors to the feather-tk project.

import feather_tk as ftk

import unittest

try:
    import numpy
except ImportError:
    numpy = None

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

    def test_image_data(self):
        # Through the buffer protocol: nothing beyond Python needed.
        img = ftk.Image(4, 2, ftk.ImageType.RGBA_U8)
        img.zero()
        view = img.data
        self.assertEqual(view.shape, (2, 4, 4))
        self.assertEqual(view.nbytes, img.byteCount)
        self.assertFalse(view.readonly)
        view[1, 3, 0] = 255
        self.assertEqual(img.data[1, 3, 0], 255)
        self.assertEqual(img.data[0, 0, 0], 0)

    def test_image_data_refused(self):
        # Packed and planar types have no array of this shape.
        for t in (ftk.ImageType.RGB_U10, ftk.ImageType.YUV_420P_U8):
            img = ftk.Image(4, 2, t)
            with self.assertRaises(RuntimeError):
                img.data

    @unittest.skipIf(numpy is None, "numpy is not installed")
    def test_image_numpy(self):
        for t, dtype, channels in (
            (ftk.ImageType.L_U8, numpy.uint8, 1),
            (ftk.ImageType.LA_U16, numpy.uint16, 2),
            (ftk.ImageType.RGB_U32, numpy.uint32, 3),
            (ftk.ImageType.RGBA_F16, numpy.float16, 4),
            (ftk.ImageType.RGBA_F32, numpy.float32, 4)):
            a = numpy.asarray(ftk.Image(ftk.Size2I(4, 2), t))
            self.assertEqual(a.shape, (2, 4, channels))
            self.assertEqual(a.dtype, dtype)

        # The image's own memory: written through one view, read in another,
        # and kept alive by the array.
        img = ftk.Image(4, 2, ftk.ImageType.RGBA_F32)
        a = numpy.asarray(img)
        a[:] = 0.0
        a[1, 3] = (0.25, 0.5, 0.75, 1.0)
        b = numpy.asarray(img)
        self.assertEqual(b[1, 3].tolist(), [0.25, 0.5, 0.75, 1.0])
        self.assertEqual(b[0, 0].tolist(), [0.0, 0.0, 0.0, 0.0])
        del img, b
        self.assertEqual(a[1, 3, 2], 0.75)

        # Padded rows: a three byte pixel in rows aligned to four bytes.
        info = ftk.ImageInfo(3, 2, ftk.ImageType.RGB_U8)
        info.layout = ftk.ImageLayout(ftk.ImageMirror(), 4, ftk.getEndian())
        a = numpy.asarray(ftk.Image(info))
        self.assertEqual(a.shape, (2, 3, 3))
        self.assertEqual(a.strides, (12, 3, 1))

        # A mirrored layout is seen the right way up: the first row of the
        # view is the last in memory.
        info = ftk.ImageInfo(2, 3, ftk.ImageType.L_U8)
        info.layout = ftk.ImageLayout(ftk.ImageMirror(False, True), 1, ftk.getEndian())
        img = ftk.Image(info)
        img.zero()
        a = numpy.asarray(img)
        a[0, 1, 0] = 9
        self.assertEqual(a.strides, (-2, 1, 1))
        self.assertEqual(a[2].ctypes.data + 4, a[0].ctypes.data)
        self.assertEqual(numpy.asarray(img)[0, 1, 0], 9)

        # numpy's dtype and copy arguments.
        img = ftk.Image(2, 2, ftk.ImageType.L_U8)
        img.zero()
        self.assertEqual(numpy.asarray(img, dtype=numpy.float32).dtype, numpy.float32)
        c = numpy.array(img, copy=True)
        c[0, 0] = 7
        self.assertEqual(numpy.asarray(img)[0, 0, 0], 0)
        with self.assertRaises(ValueError):
            numpy.asarray(img, dtype=numpy.float32, copy=False)

if __name__ == '__main__':
    unittest.main()
