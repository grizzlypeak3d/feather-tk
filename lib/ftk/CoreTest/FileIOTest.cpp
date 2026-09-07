// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/CoreTest/FileIOTest.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/FileIO.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Path.h>

#include <algorithm>
#include <atomic>
#include <limits>
#include <thread>
#include <utility>

namespace ftk
{
    namespace core_test
    {
        FileIOTest::FileIOTest(const std::shared_ptr<Context>& context) :
            ITest(context, "ftk::core_test::FileIOTest")
        {}

        FileIOTest::~FileIOTest()
        {}

        std::shared_ptr<FileIOTest> FileIOTest::create(
            const std::shared_ptr<Context>& context)
        {
            return std::shared_ptr<FileIOTest>(new FileIOTest(context));
        }
        
        void FileIOTest::run()
        {
            _enums();
            _members();
            _readAt();
            _functions();
            _operators();
        }
        
        void FileIOTest::_enums()
        {
            FTK_TEST_ENUM(FileMode);
            FTK_TEST_ENUM(FileRead);
        }
        
        void FileIOTest::_members()
        {
            for (auto fileRead : getFileReadEnums())
            {
                {
                    const std::filesystem::path path = _getTempDir() / "FileIOTest";
                    auto fileIO = FileIO::create(path, FileMode::Write);
                    FTK_CHECK(fileIO->isOpen());
                    FTK_CHECK(path == fileIO->getPath());
                    FTK_CHECK(0 == fileIO->getPos());
                    fileIO->write("Hello");
                    fileIO->seek(0, SeekMode::Set);
                    FTK_CHECK(0 == fileIO->getPos());
                    fileIO->seek(1, SeekMode::Forward);
                    fileIO->seek(1, SeekMode::Forward);
                    FTK_CHECK(2 == fileIO->getPos());
                    FTK_CHECK(!fileIO->isEOF());
                    fileIO->seek(5, SeekMode::Set);
                    FTK_CHECK(fileIO->isEOF());
                    fileIO.reset();
                    
                    fileIO = FileIO::create(path, FileMode::Append);
                    fileIO->write(" world");
                    fileIO.reset();

                    fileIO = FileIO::create(path, FileMode::Read, fileRead);
                    const size_t size = fileIO->getSize();
                    FTK_CHECK(11 == size);
                    if (FileRead::MMap == fileRead)
                    {
                        FTK_CHECK(fileIO->getMemStart());
                        FTK_CHECK(fileIO->getMemEnd());
                        FTK_CHECK(fileIO->getMemP());
                    }
                    std::string contents;
                    while (!fileIO->isEOF())
                    {
                        char c = 0;
                        fileIO->read(&c, 1);
                        contents.push_back(c);
                    }
                    FTK_CHECK(contents == "Hello world");
                    fileIO->seek(0, SeekMode::Set);
                    FTK_CHECK(0 == fileIO->getPos());
                    fileIO->seek(1, SeekMode::Forward);
                    fileIO->seek(1, SeekMode::Forward);
                    FTK_CHECK(2 == fileIO->getPos());
                    FTK_CHECK(!fileIO->isEOF());
                    fileIO->seek(fileIO->getSize(), SeekMode::Set);
                    FTK_CHECK(fileIO->isEOF());
                    fileIO.reset();

                    MemFile memFile(nullptr, (uint8_t*)contents.data(), contents.size());
                    fileIO = FileIO::create(path, memFile);
                    std::string contents2;
                    while (!fileIO->isEOF())
                    {
                        char c = 0;
                        fileIO->read(&c, 1);
                        contents2.push_back(c);
                    }
                    FTK_CHECK(contents2 == "Hello world");
                }
                {
                    const std::filesystem::path path = _getTempDir() / "FileIOTest2";
                    auto fileIO = FileIO::create(path, FileMode::Write);
                    int8_t i8 = 1;
                    uint8_t u8 = 2;
                    int16_t i16 = 3;
                    uint16_t u16 = 4;
                    int32_t i32 = 5;
                    uint32_t u32 = 6;
                    float f = 7.F;
                    fileIO->write8(i8);
                    fileIO->writeU8(u8);
                    fileIO->write16(i16);
                    fileIO->writeU16(u16);
                    fileIO->write32(i32);
                    fileIO->writeU32(u32);
                    fileIO->writeF32(f);
                    fileIO.reset();

                    fileIO = FileIO::create(path, FileMode::Read, fileRead);
                    int8_t i8b = 1;
                    uint8_t u8b = 2;
                    int16_t i16b = 3;
                    uint16_t u16b = 4;
                    int32_t i32b = 5;
                    uint32_t u32b = 6;
                    float fb = 7.F;
                    fileIO->read8(&i8b, 1);
                    fileIO->readU8(&u8b, 1);
                    fileIO->read16(&i16b, 1);
                    fileIO->readU16(&u16b, 1);
                    fileIO->read32(&i32b, 1);
                    fileIO->readU32(&u32b, 1);
                    fileIO->readF32(&fb, 1);
                    FTK_CHECK(i8 == i8b);
                    FTK_CHECK(u8 == u8b);
                    FTK_CHECK(i16 == i16b);
                    FTK_CHECK(u16 == u16b);
                    FTK_CHECK(i32 == i32b);
                    FTK_CHECK(u32 == u32b);
                    FTK_CHECK(f == fb);
                }
                {
                    const std::filesystem::path path = _getTempDir() / "FileIOTest3";
                    auto fileIO = FileIO::create(path, FileMode::Write);
                    uint32_t u32 = 1;
                    FTK_CHECK(!fileIO->hasEndianConversion());
                    fileIO->setEndianConversion(true);
                    FTK_CHECK(fileIO->hasEndianConversion());
                    fileIO->writeU32(u32);
                    fileIO.reset();

                    fileIO = FileIO::create(path, FileMode::Read, fileRead);
                    uint32_t u32b = 0;
                    fileIO->readU32(&u32b, 1);
                    FTK_CHECK(u32 != u32b);
                    fileIO.reset();

                    fileIO = FileIO::create(path, FileMode::Read, fileRead);
                    fileIO->setEndianConversion(true);
                    u32b = 0;
                    fileIO->readU32(&u32b, 1);
                    FTK_CHECK(u32 == u32b);
                    fileIO.reset();

                    fileIO = FileIO::create(path, FileMode::ReadWrite, fileRead);
                    fileIO->setEndianConversion(true);
                    u32b = 0;
                    fileIO->readU32(&u32b, 1);
                    FTK_CHECK(u32 == u32b);
                }
                try
                {
                    const std::filesystem::path path = _getTempDir() / "FileIOTest4";
                    auto fileIO = FileIO::create(path, FileMode::Read, fileRead);
                    FTK_CHECK(false);
                }
                catch (const std::exception&)
                {}
                try
                {
                    const std::filesystem::path path = _getTempDir() / "FileIOTest5";
                    auto fileIO = FileIO::create(path, FileMode::Write);
                    fileIO.reset();

                    fileIO = FileIO::create(path, FileMode::Read, fileRead);
                    uint8_t u8 = 0;
                    fileIO->readU8(&u8, 1);
                    FTK_CHECK(false);
                }
                catch (const std::exception&)
                {}
                try
                {
                    const std::filesystem::path path = _getTempDir() / "FileIOTest6";
                    auto fileIO = FileIO::create(path, FileMode::Write);
                    fileIO.reset();

                    fileIO = FileIO::create(path, FileMode::ReadWrite, fileRead);
                    uint8_t u8 = 0;
                    fileIO->readU8(&u8, 1);
                    FTK_CHECK(false);
                }
                catch (const std::exception&)
                {}
                try
                {
                    const std::filesystem::path path = _getTempDir() / "FileIOTest7";
                    auto fileIO = FileIO::create(path, FileMode::Read, fileRead);
                    fileIO->writeU8(1);
                    FTK_CHECK(false);
                }
                catch (const std::exception&)
                {}
                {
                    const std::filesystem::path path = _getTempDir() / toFileSystem("大平原");
                    auto fileIO = FileIO::create(path, FileMode::Write);
                    fileIO->writeU8(1);
                    fileIO.reset();

                    fileIO = FileIO::create(path, FileMode::Read, fileRead);
                    uint8_t c = 0;
                    fileIO->readU8(&c);
                    FTK_CHECK(1 == c);
                }
            }
        }
        
        namespace
        {
            // At file scope so the thread below needs no capture for it.
            // Whether a constant like this has to be captured is a point the
            // compilers disagree on: clang warns if it is captured, MSVC
            // fails to build if it is not.
            constexpr size_t readAtFileSize = 64 * 1024;
        }

        void FileIOTest::_readAt()
        {
            const size_t fileSize = readAtFileSize;
            std::vector<uint8_t> data(fileSize);
            for (size_t i = 0; i < fileSize; ++i)
            {
                data[i] = static_cast<uint8_t>((i * 7 + 11) & 0xFF);
            }
            const std::filesystem::path path = _getTempDir() / "FileIOTestReadAt";
            {
                auto fileIO = FileIO::create(path, FileMode::Write);
                fileIO->write(data.data(), data.size());
            }

            for (auto fileRead : getFileReadEnums())
            {
                auto fileIO = FileIO::create(path, FileMode::Read, fileRead);

                const std::vector<std::pair<size_t, size_t> > ranges =
                {
                    { 0, 1 },
                    { 0, fileSize },
                    { 1, 4095 },
                    { 4096, 8192 },
                    { fileSize - 1, 1 },
                    { fileSize, 0 }
                };
                for (const auto& range : ranges)
                {
                    std::vector<uint8_t> buf(range.second + 1);
                    fileIO->readAt(buf.data(), range.first, range.second);
                    FTK_CHECK(std::equal(
                        buf.begin(),
                        buf.begin() + range.second,
                        data.begin() + range.first));
                }

                // The current position is not disturbed.
                fileIO->seek(100, SeekMode::Set);
                uint8_t u8 = 0;
                fileIO->read(&u8, 1);
                std::vector<uint8_t> buf(1000);
                fileIO->readAt(buf.data(), 5000, buf.size());
                FTK_CHECK(101 == fileIO->getPos());
                fileIO->read(&u8, 1);
                FTK_CHECK(data[101] == u8);
                FTK_CHECK(102 == fileIO->getPos());

                // Several threads read at once.
                std::vector<std::thread> threads;
                std::atomic<bool> ok(true);
                for (size_t i = 0; i < 8; ++i)
                {
                    threads.push_back(std::thread(
                        [fileIO, &data, &ok, i]
                        {
                            std::vector<uint8_t> buf(1024);
                            for (size_t j = 0; j < 100; ++j)
                            {
                                const size_t pos =
                                    ((i * 100 + j) * 617) %
                                    (readAtFileSize - buf.size());
                                fileIO->readAt(buf.data(), pos, buf.size());
                                if (!std::equal(
                                    buf.begin(),
                                    buf.end(),
                                    data.begin() + pos))
                                {
                                    ok = false;
                                }
                            }
                        }));
                }
                for (auto& thread : threads)
                {
                    thread.join();
                }
                FTK_CHECK(ok);

                try
                {
                    fileIO->readAt(&u8, fileSize, 1);
                    FTK_CHECK(false);
                }
                catch (const std::exception&)
                {}
                try
                {
                    fileIO->readAt(&u8, fileSize + 1, 0);
                    FTK_CHECK(false);
                }
                catch (const std::exception&)
                {}
            }

            // Endian conversion and word size match read().
            {
                auto a = FileIO::create(path, FileMode::Read);
                a->setEndianConversion(true);
                a->seek(64, SeekMode::Set);
                std::vector<uint32_t> viaRead(16);
                a->readU32(viaRead.data(), viaRead.size());

                auto b = FileIO::create(path, FileMode::Read);
                b->setEndianConversion(true);
                std::vector<uint32_t> viaReadAt(16);
                b->readAt(viaReadAt.data(), 64, viaReadAt.size(), 4);
                FTK_CHECK(viaRead == viaReadAt);
            }

            {
                MemFile memFile(nullptr, data.data(), data.size());
                auto fileIO = FileIO::create(path, memFile);
                std::vector<uint8_t> buf(256);
                fileIO->readAt(buf.data(), 1234, buf.size());
                FTK_CHECK(std::equal(buf.begin(), buf.end(), data.begin() + 1234));
            }

            try
            {
                const std::filesystem::path path2 = _getTempDir() / "FileIOTestReadAt2";
                auto fileIO = FileIO::create(path2, FileMode::Write);
                uint8_t u8 = 0;
                fileIO->readAt(&u8, 0, 1);
                FTK_CHECK(false);
            }
            catch (const std::exception&)
            {}
        }

        void FileIOTest::_functions()
        {
            {
                const std::filesystem::path path = _getTempDir() / "FileIOTest8";
                auto fileIO = FileIO::create(path, FileMode::Write);
                fileIO->write("Hello world");
                fileIO.reset();

                fileIO = FileIO::create(path, FileMode::Read);
                std::string contents = read(fileIO);
                FTK_CHECK(contents == "Hello world");
            }
            {
                const std::filesystem::path path = _getTempDir() / "FileIOTest9";
                writeLines(
                    path,
                    {
                        "# This is a comment",
                        "Hello world"
                    });
                for (auto readType : getFileReadEnums())
                {
                    auto io = FileIO::create(path, FileMode::Read, readType);
                    char buf[cStringSize];
                    readWord(io, buf);
                    FTK_CHECK(std::string("Hello") == std::string(buf));
                    readWord(io, buf);
                    FTK_CHECK(std::string("world") == std::string(buf));
                }
            }
            {
                const std::filesystem::path path = _getTempDir() / "FileIOTest10";
                auto fileIO = FileIO::create(path, FileMode::Write);
                fileIO->write("Hello world");
                fileIO.reset();

                fileIO = FileIO::create(path, FileMode::Read);
                std::string line = readLine(fileIO);
                FTK_CHECK(line == "Hello world");
            }
            {
                const std::filesystem::path path = _getTempDir() / "FileIOTest11";
                std::vector<std::string> contents;
                contents.push_back("Hello");
                contents.push_back("World");
                writeLines(path, contents);
                FTK_CHECK(contents == readLines(path));
            }
            {
                const std::filesystem::path path = _getTempDir() / "FileIOTest12";
                auto fileIO = FileIO::create(path, FileMode::Write);
                fileIO->write("Hello world");
                fileIO.reset();

                truncateFile(path, 5);
                fileIO = FileIO::create(path, FileMode::ReadWrite);
                std::string contents = read(fileIO);
                FTK_CHECK(contents == "Hello");
            }
        }
        
        void FileIOTest::_operators()
        {
            {
                std::string contents = "Hello world";
                MemFile a(nullptr, (uint8_t*)contents.data(), contents.size());
                MemFile b(nullptr, (uint8_t*)contents.data(), contents.size());
                FTK_CHECK(a == b);
                b = MemFile();
                FTK_CHECK(a != b);
            }
        }
    }
}

