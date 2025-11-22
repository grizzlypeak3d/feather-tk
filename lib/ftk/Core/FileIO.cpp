// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/FileIO.h>

#include <ftk/Core/Assert.h>
#include <ftk/Core/Error.h>

#include <algorithm>
#include <array>
#include <iostream>
#include <sstream>

namespace ftk
{
    FTK_ENUM_IMPL(
        FileMode,
        "Read",
        "Write",
        "Read Write",
        "Append");

    FTK_ENUM_IMPL(
        FileRead,
        "Normal",
        "Memory Mapped");

    std::shared_ptr<FileIO> FileIO::create(
        const std::filesystem::path& path,
        FileMode mode,
        FileRead readType)
    {
        auto out = std::shared_ptr<FileIO>(new FileIO);
        out->_open(path, mode, readType);
        return out;
    }

    std::shared_ptr<FileIO> FileIO::create(
        const std::string& path,
        FileMode mode,
        FileRead readType)
    {
        return create(std::filesystem::u8path(path), mode, readType);
    }

    std::shared_ptr<FileIO> FileIO::create(
        const std::string& path,
        const MemFile& memFile)
    {
        return create(std::filesystem::u8path(path), memFile);
    }

    void FileIO::read8(int8_t* value, size_t size)
    {
        return read(value, size, 1);
    }

    void FileIO::readU8(uint8_t* value, size_t size)
    {
        return read(value, size, 1);
    }

    void FileIO::read16(int16_t* value, size_t size)
    {
        return read(value, size, 2);
    }

    void FileIO::readU16(uint16_t* value, size_t size)
    {
        return read(value, size, 2);
    }

    void FileIO::read32(int32_t* value, size_t size)
    {
        return read(value, size, 4);
    }

    void FileIO::readU32(uint32_t* value, size_t size)
    {
        return read(value, size, 4);
    }

    void FileIO::readF32(float* value, size_t size)
    {
        return read(value, size, 4);
    }

    void FileIO::write8(const int8_t* value, size_t size)
    {
        write(value, size, 1);
    }

    void FileIO::writeU8(const uint8_t* value, size_t size)
    {
        write(value, size, 1);
    }

    void FileIO::write16(const int16_t* value, size_t size)
    {
        write(value, size, 2);
    }

    void FileIO::writeU16(const uint16_t* value, size_t size)
    {
        write(value, size, 2);
    }

    void FileIO::write32(const int32_t* value, size_t size)
    {
        return write(value, size, 4);
    }

    void FileIO::writeU32(const uint32_t* value, size_t size)
    {
        return write(value, size, 4);
    }

    void FileIO::writeF32(const float* value, size_t size)
    {
        write(value, size, 4);
    }

    void FileIO::write8(int8_t value)
    {
        write8(&value, 1);
    }

    void FileIO::writeU8(uint8_t value)
    {
        writeU8(&value, 1);
    }

    void FileIO::write16(int16_t value)
    {
        write16(&value, 1);
    }

    void FileIO::writeU16(uint16_t value)
    {
        writeU16(&value, 1);
    }

    void FileIO::write32(int32_t value)
    {
        write32(&value, 1);
    }

    void FileIO::writeU32(uint32_t value)
    {
        writeU32(&value, 1);
    }

    void FileIO::writeF32(float value)
    {
        writeF32(&value, 1);
    }

    void FileIO::write(const std::string& value)
    {
        write8(reinterpret_cast<const int8_t*>(value.c_str()), value.size());
    }

    std::string read(const std::shared_ptr<FileIO>& io)
    {
        std::string out;
        if (const uint8_t* p = io->getMemP())
        {
            const uint8_t* end = io->getMemEnd();
            out = std::string(reinterpret_cast<const char*>(p), end - p);
        }
        else
        {
            const size_t fileSize = io->getSize();
            out.resize(fileSize);
            io->read(reinterpret_cast<void*>(&out[0]), fileSize);
        }
        return out;
    }

    void readWord(const std::shared_ptr<FileIO>& io, char* out, size_t maxLen)
    {
        FTK_ASSERT(maxLen);
        out[0] = 0;
        enum class Parse
        {
            End,
            Word,
            Comment
        };
        Parse parse = Parse::Word;
        size_t i = 0;
        while (parse != Parse::End && !io->isEOF())
        {
            // Get the next character.
            uint8_t c = 0;
            io->read(&c, 1);

            switch (c)
            {
            case '#':
                // Start of a comment.
                parse = Parse::Comment;
                break;
            case '\0':
            case '\n':
            case '\r':
                // End of a comment or word.
                parse = Parse::Word;
            case ' ':
            case '\t':
                if (out[0])
                {
                    parse = Parse::End;
                }
                break;
            default:
                // Add the character to the word.
                if (Parse::Word == parse && i < (maxLen - 1))
                {
                    out[i++] = c;
                }
                break;
            }
        }
        out[i] = 0;
    }

    std::string readLine(const std::shared_ptr<FileIO>& io)
    {
        std::string out;
        while (!io->isEOF())
        {
            char c = 0;
            io->read(&c, 1);
            if ('\r' == c || '\n' == c)
            {
                if ('\r' == c && !io->isEOF())
                {
                    io->read(&c, 1);
                    if (c != '\n')
                    {
                        io->seek(1, SeekMode::Reverse);
                    }
                }
                break;
            }
            out.push_back(c);
        }
        return out;
    }

    std::vector<std::string> readLines(const std::filesystem::path& path)
    {
        std::vector<std::string> out;
        auto io = FileIO::create(path, FileMode::Read);
        while (!io->isEOF())
        {
            out.push_back(readLine(io));
        }
        return out;
    }

    std::vector<std::string> readLines(const std::string& path)
    {
        return readLines(std::filesystem::u8path(path));
    }

    void writeLines(const std::filesystem::path& path, const std::vector<std::string>& lines)
    {
        auto io = FileIO::create(path, FileMode::Write);
        for (const auto& line : lines)
        {
            io->write(line);
            io->write8('\n');
        }
    }

    void writeLines(const std::string& path, const std::vector<std::string>& lines)
    {
        writeLines(std::filesystem::u8path(path), lines);
    }

    void truncateFile(const std::string& path, size_t size)
    {
        truncateFile(std::filesystem::u8path(path), size);
    }
}
