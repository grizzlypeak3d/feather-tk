// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/ImageTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Image.h>

#include <cmath>

namespace ftk
{
    namespace core_test
    {
        ImageTest::ImageTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::ImageTest")
        {}

        ImageTest::~ImageTest()
        {}

        std::shared_ptr<ImageTest> ImageTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<ImageTest>(new ImageTest(context));
        }
        
        void ImageTest::run()
        {
            _enums();
            _info();
            _members();
            _functions();
            _bufferPool();
        }
        
        void ImageTest::_enums()
        {
            FTK_TEST_ENUM(ImageType);
            FTK_TEST_ENUM(VideoLevels);
            FTK_TEST_ENUM(YUVCoefficients);
        }
        
        void ImageTest::_info()
        {
            {
                const ImageInfo info;
                FTK_CHECK(!info.isValid());
                FTK_CHECK(0 == info.getByteCount());
            }
            {
                const ImageInfo info(Size2I(1920, 1080), ImageType::RGB_U8);
                FTK_CHECK(info.isValid());
                FTK_CHECK(1920 * 1080 * 3 == info.getByteCount());
            }
            {
                const ImageInfo info(1920, 1080, ImageType::RGB_U8);
                FTK_CHECK(info.isValid());
                FTK_CHECK(1920 * 1080 * 3 == info.getByteCount());
            }
            {
                const ImageInfo a;
                ImageInfo b;
                FTK_CHECK(a == b);
                b.size.w = 1920;
                b.size.h = 1080;
                FTK_CHECK(a != b);                
            }
            {
                // 12-bit video in 16-bit words scales by 65535 / 4095; the
                // depth means nothing for a type that holds its own.
                ImageInfo info(Size2I(1920, 1080), ImageType::YUV_420P_U16);
                FTK_CHECK(1.F == getSampleScale(info));
                info.depth = 12;
                FTK_CHECK(std::abs(getSampleScale(info) - 65535.F / 4095.F) < .0001F);
                info.depth = 10;
                FTK_CHECK(std::abs(getSampleScale(info) - 65535.F / 1023.F) < .0001F);
                info.type = ImageType::RGB_U16;
                FTK_CHECK(1.F == getSampleScale(info));
                ImageInfo other(Size2I(1920, 1080), ImageType::YUV_420P_U16);
                FTK_CHECK(info != other);
            }
        }
        
        void ImageTest::_members()
        {
            {
                auto image = Image::create(ImageInfo());
                FTK_CHECK(!image->isValid());
            }
            {
                auto image = Image::create(Size2I(1920, 1080), ImageType::RGB_U8);
                FTK_CHECK(image->isValid());                
            }
            {
                auto image = Image::create(1920, 1080, ImageType::RGB_U8);
                FTK_CHECK(image->isValid());
            }
            {
                const ImageInfo info(Size2I(1920, 1080), ImageType::RGB_U8);
                auto image = Image::create(info);
                image->zero();
                FTK_CHECK(info == image->getInfo());
                FTK_CHECK(info.size == image->getSize());
                FTK_CHECK(info.size.w == image->getWidth());
                FTK_CHECK(info.size.h == image->getHeight());
                FTK_CHECK(info.type == image->getType());
                FTK_CHECK(image->isValid());
                ImageTags tags;
                tags["Layer"] = "1";
                image->setTags(tags);
                FTK_CHECK(tags == image->getTags());
                FTK_CHECK(1920 * 1080 * 3 == image->getByteCount());
                FTK_CHECK(image->getData());
                const std::shared_ptr<const Image> image2 = image;
                FTK_CHECK(image2->getData());
            }
            {
                // Pixels on the GPU: no data, the nominal byte count, and
                // the release runs once, when the image goes.
                const ImageInfo info(Size2I(1920, 1080), ImageType::RGBA_U8);
                const size_t totalByteCount = Image::getTotalByteCount();
                uintptr_t released = 0;
                auto image = Image::create(
                    info,
                    42,
                    [&released](uintptr_t handle) { released = handle; });
                FTK_CHECK(image->isValid());
                FTK_CHECK(42 == image->getTextureHandle());
                FTK_CHECK(!image->getData());
                FTK_CHECK(1920 * 1080 * 4 == image->getByteCount());
                FTK_CHECK(totalByteCount == Image::getTotalByteCount());
                image->zero();
                FTK_CHECK(0 == released);
                image.reset();
                FTK_CHECK(42 == released);
            }
            {
                auto image = Image::create(1920, 1080, ImageType::RGB_U8);
                FTK_CHECK(0 == image->getTextureHandle());
            }
        }
        
        void ImageTest::_functions()
        {
            for (auto i : getImageTypeEnums())
            {
                _print(Format("{0}: channels={1}, bitDepth={2}").
                    arg(i).
                    arg(getChannelCount(i)).
                    arg(getBitDepth(i)));
            }
            for (auto i : getYUVCoefficientsEnums())
            {
                _print(Format("{0}: {1}").
                    arg(i).
                    arg(getYUVCoefficients(i)));
            }
        }

        void ImageTest::_bufferPool()
        {
            // Other images may be alive, from what ran before: the maximum is
            // set from what is alive at the start.
            const size_t maxPrev = Image::getBufferPoolMax();
            Image::clearBufferPool();
            const size_t live = Image::getTotalByteCount();

            // 4 MB, big enough to be kept; what is kept is the data and its
            // padding, a little more than the image.
            const ImageInfo big(1024, 1024, ImageType::RGBA_U8);
            const ImageInfo other(1024, 512, ImageType::RGBA_U8);
            const ImageInfo small(64, 64, ImageType::RGBA_U8);
            const size_t bigBytes = big.getByteCount();
            const size_t slack = 1024;

            // Nothing is kept until there is a maximum.
            Image::setBufferPoolMax(0);
            {
                auto image = Image::create(big);
            }
            FTK_ASSERT(0 == Image::getBufferPoolByteCount());

            // A buffer freed is kept and handed out again for the same size.
            Image::setBufferPoolMax(live + 2 * bigBytes + slack);
            FTK_ASSERT(live + 2 * bigBytes + slack == Image::getBufferPoolMax());
            const uint8_t* data = nullptr;
            {
                auto image = Image::create(big);
                data = image->getData();
            }
            FTK_ASSERT(Image::getBufferPoolByteCount() > bigBytes);
            {
                auto image = Image::create(big);
                FTK_ASSERT(data == image->getData());
                FTK_ASSERT(0 == Image::getBufferPoolByteCount());
            }

            // Small images are not kept.
            Image::clearBufferPool();
            {
                auto image = Image::create(small);
            }
            FTK_ASSERT(0 == Image::getBufferPoolByteCount());

            // Kept only while the images alive and the buffers kept fit: with
            // room for two, freeing both keeps both, and with room for one
            // only the first freed is kept.
            {
                auto a = Image::create(big);
                auto b = Image::create(big);
            }
            FTK_ASSERT(Image::getBufferPoolByteCount() > 2 * bigBytes);
            Image::clearBufferPool();
            Image::setBufferPoolMax(live + bigBytes + slack);
            {
                auto a = Image::create(big);
                auto b = Image::create(big);
                b.reset();
                // One alive and one kept is more than the room for one.
                FTK_ASSERT(0 == Image::getBufferPoolByteCount());
            }
            FTK_ASSERT(Image::getBufferPoolByteCount() > bigBytes);
            FTK_ASSERT(Image::getBufferPoolByteCount() < 2 * bigBytes);

            // Lowering the maximum frees what no longer fits.
            Image::clearBufferPool();
            Image::setBufferPoolMax(live + 2 * bigBytes + slack);
            {
                auto a = Image::create(big);
                auto b = Image::create(big);
            }
            FTK_ASSERT(Image::getBufferPoolByteCount() > 2 * bigBytes);
            Image::setBufferPoolMax(live + bigBytes + slack);
            FTK_ASSERT(Image::getBufferPoolByteCount() > bigBytes);
            FTK_ASSERT(Image::getBufferPoolByteCount() < 2 * bigBytes);

            // An image of another size makes room for itself: the buffer kept
            // is freed rather than held alongside it.
            {
                auto image = Image::create(other);
                FTK_ASSERT(0 == Image::getBufferPoolByteCount());
            }

            // Clearing frees everything kept.
            {
                auto image = Image::create(big);
            }
            FTK_ASSERT(Image::getBufferPoolByteCount() > 0);
            Image::clearBufferPool();
            FTK_ASSERT(0 == Image::getBufferPoolByteCount());

            Image::setBufferPoolMax(maxPrev);
        }
    }
}

