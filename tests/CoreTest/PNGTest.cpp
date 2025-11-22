// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <CoreTest/PNGTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Context.h>
#include <ftk/Core/FileIO.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/ImageIO.h>

namespace ftk
{
    namespace core_test
    {
        PNGTest::PNGTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::PNGTest")
        {}

        PNGTest::~PNGTest()
        {}

        std::shared_ptr<PNGTest> PNGTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<PNGTest>(new PNGTest(context));
        }
        
        void PNGTest::run()
        {
            if (auto context = _context.lock())
            {
                auto io = context->getSystem<ImageIO>();
                const std::vector<Size2I> sizes =
                {
                    Size2I(512, 512),
                    Size2I(1, 1),
                    Size2I(0, 0)
                };
                const std::vector<ImageType> imageTypes =
                {
                    ImageType::L_U8,
                    ImageType::L_U16,
                    ImageType::LA_U8,
                    ImageType::LA_U16,
                    ImageType::RGB_U8,
                    ImageType::RGB_U16,
                    ImageType::RGBA_U8,
                    ImageType::RGBA_U16,
                    ImageType::None
                };
                for (const auto& size : sizes)
                {
                    for (auto imageType : imageTypes)
                    {
                        try
                        {
                            const ImageInfo info(size, imageType);
                            auto image = Image::create(info);
                            image->zero();
                            const std::filesystem::path path = Format(
                                "PNGTest_{0}_{1}_{2}.png").
                                arg(size.w).
                                arg(size.h).
                                arg(imageType).str();

                            auto write = io->write(path, info);
                            if (write)
                            {
                                write->write(image);
                                write.reset();
                            }

                            auto read = io->read(path);
                            auto info2 = read->getInfo();
                            FTK_ASSERT(info.size == info2.size);
                            FTK_ASSERT(info.type == info2.type);
                            auto image2 = read->read();
                            FTK_ASSERT(image2);
                            read.reset();
                            
                            auto fileIO = FileIO::create(path, FileMode::Read);
                            const size_t size = fileIO->getSize();
                            InMemFile mem(fileIO->getMemStart(), size);
                            read = io->read(path, mem);
                            info2 = read->getInfo();
                            FTK_ASSERT(info.size == info2.size);
                            FTK_ASSERT(info.type == info2.type);
                            image2 = read->read();
                            FTK_ASSERT(image2);
                            read.reset();
                            
                            truncateFile(path, 0);
                            read = io->read(path);
                            image2 = read->read();
                            FTK_ASSERT(false);
                        }
                        catch (const std::exception& e)
                        {
                            _error(e.what());
                        }
                    }
                }
            }
        }
    }
}

