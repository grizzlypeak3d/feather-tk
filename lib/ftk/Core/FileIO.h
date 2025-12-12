// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/String.h>
#include <ftk/Core/Util.h>

#include <filesystem>
#include <memory>

namespace ftk
{
    //! \name File I/O
    ///@{
        
    //! File I/O modes.
    enum class FTK_API_TYPE FileMode
    {
        Read,
        Write,
        ReadWrite,
        Append,

        Count,
        First = Read
    };
    FTK_ENUM(FileMode);

    //! File read type.
    enum class FTK_API_TYPE FileRead
    {
        Normal,
        MMap,

        Count,
        First = Normal
    };
    FTK_ENUM(FileRead);

    //! In-memory file.
    struct FTK_API_TYPE MemFile
    {
        MemFile() = default;
        MemFile(const uint8_t*, size_t size);

        const uint8_t* p = nullptr;
        size_t size = 0;

        bool operator == (const MemFile&) const;
        bool operator != (const MemFile&) const;
    };

    //! Seek modes.
    enum class FTK_API_TYPE SeekMode
    {
        Set,
        Forward,
        Reverse,

        Count,
        First = Set
    };
    FTK_ENUM(SeekMode);

    //! File I/O.
    class FTK_API_TYPE FileIO : public std::enable_shared_from_this<FileIO>
    {
        FTK_NON_COPYABLE(FileIO);

    protected:
        FileIO();

    public:
        FTK_API ~FileIO();

        //! Create a new file I/O object.
        FTK_API static std::shared_ptr<FileIO> create(
            const std::filesystem::path&,
            FileMode,
            FileRead = FileRead::MMap);

        //! Create a new file I/O object.
        FTK_API static std::shared_ptr<FileIO> create(
            const std::string&,
            FileMode,
            FileRead = FileRead::MMap);

        //! Create a read-only file I/O object from memory.
        FTK_API static std::shared_ptr<FileIO> create(
            const std::filesystem::path&,
            const MemFile&);

        //! Create a read-only file I/O object from memory.
        FTK_API static std::shared_ptr<FileIO> create(
            const std::string&,
            const MemFile&);

        //! Get whether the file is open.
        FTK_API bool isOpen() const;

        //! \name Information
        ///@{

        //! Get the file path.
        FTK_API const std::filesystem::path& getPath() const;

        //! Get the file size.
        FTK_API size_t getSize() const;

        ///@}

        //! \name Position
        ///@{

        //! Get the current file position.
        FTK_API size_t getPos() const;

        //! Change the current file position.
        FTK_API void seek(size_t, SeekMode);

        //! Get whether the file position is at the end of the file.
        FTK_API bool isEOF() const;

        ///@}

        //! \name Read
        ///@{

        FTK_API void read(void*, size_t, size_t wordSize = 1);

        FTK_API void read8(int8_t*, size_t = 1);
        FTK_API void readU8(uint8_t*, size_t = 1);
        FTK_API void read16(int16_t*, size_t = 1);
        FTK_API void readU16(uint16_t*, size_t = 1);
        FTK_API void read32(int32_t*, size_t = 1);
        FTK_API void readU32(uint32_t*, size_t = 1);
        FTK_API void readF32(float*, size_t = 1);

        ///@}

        //! \name Write
        ///@{

        FTK_API void write(const void*, size_t, size_t wordSize = 1);

        FTK_API void write8(const int8_t*, size_t);
        FTK_API void writeU8(const uint8_t*, size_t);
        FTK_API void write16(const int16_t*, size_t);
        FTK_API void writeU16(const uint16_t*, size_t);
        FTK_API void write32(const int32_t*, size_t);
        FTK_API void writeU32(const uint32_t*, size_t);
        FTK_API void writeF32(const float*, size_t);

        FTK_API void write8(int8_t);
        FTK_API void writeU8(uint8_t);
        FTK_API void write16(int16_t);
        FTK_API void writeU16(uint16_t);
        FTK_API void write32(int32_t);
        FTK_API void writeU32(uint32_t);
        FTK_API void writeF32(float);

        FTK_API void write(const std::string&);

        ///@}

        //! \name Memory Mapping
        ///@{

        //! Get a pointer to the start of the file memory.
        FTK_API const uint8_t* getMemStart() const;

        //! Get a pointer to the end of the file memory.
        FTK_API const uint8_t* getMemEnd() const;

        //! Get the current file memory position.
        FTK_API const uint8_t* getMemP() const;

        ///@}

        //! \name Endian
        ///@{

        //! Get whether automatic endian conversion is performed.
        FTK_API bool hasEndianConversion() const;

        //! Set whether automatic endian conversion is performed.
        FTK_API void setEndianConversion(bool);

        ///@}

    private:
        void _open(const std::filesystem::path&, FileMode, FileRead);
        bool _close(std::string* error = nullptr);

        FTK_PRIVATE();
    };

    //! Read the contents from a file.
    FTK_API std::string read(const std::shared_ptr<FileIO>&);

    //! Read a word from a file.
    FTK_API void readWord(const std::shared_ptr<FileIO>&, char*, size_t maxLen = cStringSize);

    //! Read a line from a file.
    FTK_API std::string readLine(const std::shared_ptr<FileIO>&);

    //! Read all the lines from a file.
    FTK_API std::vector<std::string> readLines(const std::filesystem::path&);

    //! Read all the lines from a file.
    FTK_API std::vector<std::string> readLines(const std::string&);

    //! Write lines to a file.
    FTK_API void writeLines(const std::filesystem::path&, const std::vector<std::string>&);

    //! Write lines to a file.
    FTK_API void writeLines(const std::string&, const std::vector<std::string>&);

    //! Truncate a file.
    FTK_API void truncateFile(const std::filesystem::path&, size_t);

    //! Truncate a file.
    FTK_API void truncateFile(const std::string&, size_t);

    ///@}
}

#include <ftk/Core/FileIOInline.h>
