// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/ImageTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Image.h>

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
                FTK_ASSERT(!info.isValid());
                FTK_ASSERT(0 == info.getByteCount());
            }
            {
                const ImageInfo info(Size2I(1920, 1080), ImageType::RGB_U8);
                FTK_ASSERT(info.isValid());
                FTK_ASSERT(1920 * 1080 * 3 == info.getByteCount());
            }
            {
                const ImageInfo info(1920, 1080, ImageType::RGB_U8);
                FTK_ASSERT(info.isValid());
                FTK_ASSERT(1920 * 1080 * 3 == info.getByteCount());
            }
            {
                const ImageInfo a;
                ImageInfo b;
                FTK_ASSERT(a == b);
                b.size.w = 1920;
                b.size.h = 1080;
                FTK_ASSERT(a != b);                
            }
        }
        
        void ImageTest::_members()
        {
            {
                auto image = Image::create(ImageInfo());
                FTK_ASSERT(!image->isValid());
            }
            {
                auto image = Image::create(Size2I(1920, 1080), ImageType::RGB_U8);
                FTK_ASSERT(image->isValid());                
            }
            {
                auto image = Image::create(1920, 1080, ImageType::RGB_U8);
                FTK_ASSERT(image->isValid());
            }
            {
                const ImageInfo info(Size2I(1920, 1080), ImageType::RGB_U8);
                auto image = Image::create(info);
                image->zero();
                FTK_ASSERT(info == image->getInfo());
                FTK_ASSERT(info.size == image->getSize());
                FTK_ASSERT(info.size.w == image->getWidth());
                FTK_ASSERT(info.size.h == image->getHeight());
                const float aspect = image->getAspect();
                FTK_ASSERT(info.type == image->getType());
                FTK_ASSERT(image->isValid());
                ImageTags tags;
                tags["Layer"] = "1";
                image->setTags(tags);
                FTK_ASSERT(tags == image->getTags());
                FTK_ASSERT(1920 * 1080 * 3 == image->getByteCount());
                FTK_ASSERT(image->getData());
                const std::shared_ptr<const Image> image2 = image;
                FTK_ASSERT(image2->getData());
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
    }
}

