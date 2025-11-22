// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/FileIO.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/Format.h>
#include <ftk/Core/Memory.h>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN
#ifndef NOMINMAX
#define NOMINMAX
#endif // NOMINMAX
#include <windows.h>

namespace ftk
{
    namespace
    {
        enum class ErrorType
        {
            Open,
            OpenTemp,
            MMap,
            Close,
            CloseMMap,
            Read,
            ReadMMap,
            Write,
            Seek,
            SeekMMap
        };

        std::string getErrorMessage(
            ErrorType          type,
            const std::string& path,
            const std::string& message = std::string())
        {
            std::string out;
            switch (type)
            {
            case ErrorType::Open:
                out = Format("Cannot open file \"{0}\"").arg(path);
                break;
            case ErrorType::OpenTemp:
                out = Format("Cannot open temporary file");
                break;
            case ErrorType::MMap:
                out = Format("Cannot memory map file \"{0}\"").arg(path);
                break;
            case ErrorType::Close:
                out = Format("Cannot close file \"{0}\"").arg(path);
                break;
            case ErrorType::CloseMMap:
                out = Format("Cannot unmap file \"{0}\"").arg(path);
                break;
            case ErrorType::Read:
                out = Format("Cannot read file \"{0}\"").arg(path);
                break;
            case ErrorType::ReadMMap:
                out = Format("Cannot read memory mapped file \"{0}\"").arg(path);
                break;
            case ErrorType::Write:
                out = Format("Cannot write file \"{0}\"").arg(path);
                break;
            case ErrorType::Seek:
                out = Format("Cannot seek file \"{0}\"").arg(path);
                break;
            case ErrorType::SeekMMap:
                out = Format("Cannot seek memory mapped file \"{0}\"").arg(path);
                break;
            default: break;
            }
            if (!message.empty())
            {
                out = Format("{0}: {1}").arg(out).arg(message);
            }
            return out;
        }

    } // namespace

    struct FileIO::Private
    {
        void seek(size_t, SeekMode);

        std::filesystem::path path;
        FileMode              mode = FileMode::First;
        FileRead              readType = FileRead::First;
        size_t                pos = 0;
        size_t                size = 0;
        bool                  endianConversion = false;
        HANDLE                f = INVALID_HANDLE_VALUE;
        void*                 mMap = nullptr;
        const uint8_t*        memStart = nullptr;
        const uint8_t*        memEnd = nullptr;
        const uint8_t*        memP = nullptr;
    };

    FileIO::FileIO() :
        _p(new Private)
    {}

    FileIO::~FileIO()
    {
        _close();
    }

    std::shared_ptr<FileIO> FileIO::create(
        const std::filesystem::path& path,
        const MemFile& memFile)
    {
        auto out = std::shared_ptr<FileIO>(new FileIO);
        out->_p->path = path;
        out->_p->mode = FileMode::Read;
        out->_p->readType = FileRead::Normal;
        out->_p->size = memFile.size;
        out->_p->memStart = memFile.p;
        out->_p->memEnd = memFile.p + memFile.size;
        out->_p->memP = memFile.p;
        return out;
    }

    bool FileIO::isOpen() const
    {
        return _p->f != INVALID_HANDLE_VALUE || _p->memStart;
    }

    const std::filesystem::path& FileIO::getPath() const
    {
        return _p->path;
    }

    size_t FileIO::getSize() const
    {
        return _p->size;
    }

    size_t FileIO::getPos() const
    {
        return _p->pos;
    }

    void FileIO::seek(size_t in, SeekMode mode)
    {
        _p->seek(in, mode);
    }

    const uint8_t* FileIO::getMemStart() const
    {
        return _p->memStart;
    }

    const uint8_t* FileIO::getMemEnd() const
    {
        return _p->memEnd;
    }

    const uint8_t* FileIO::getMemP() const
    {
        return _p->memP;
    }

    bool FileIO::hasEndianConversion() const
    {
        return _p->endianConversion;
    }

    void FileIO::setEndianConversion(bool in)
    {
        _p->endianConversion = in;
    }

    bool FileIO::isEOF() const
    {
        FTK_P();
        bool out = false;
        if (!p.memStart)
        {
            out |= p.f == INVALID_HANDLE_VALUE;
        }
        out |= p.pos >= p.size;
        return out;
    }

    void FileIO::read(void* in, size_t size, size_t wordSize)
    {
        FTK_P();

        if (!p.memStart && !p.f)
        {
            throw std::runtime_error(
                getErrorMessage(ErrorType::Read, p.path.u8string()));
        }

        switch (p.mode)
        {
        case FileMode::Read:
        {
            if (p.memStart)
            {
                const uint8_t* memP = p.memP + size * wordSize;
                if (memP > p.memEnd)
                {
                    throw std::runtime_error(
                        getErrorMessage(ErrorType::ReadMMap, p.path.u8string()));
                }
                if (p.endianConversion && wordSize > 1)
                {
                    endian(p.memP, in, size, wordSize);
                }
                else
                {
                    std::memcpy(in, p.memP, size * wordSize);
                }
                p.memP = memP;
            }
            else
            {
                DWORD n;
                if (!::ReadFile(p.f, in, static_cast<DWORD>(size * wordSize), &n, 0))
                {
                    throw std::runtime_error(
                        getErrorMessage(ErrorType::Read, p.path.u8string(), getLastError()));
                }
                if (p.endianConversion && wordSize > 1)
                {
                    endian(in, size, wordSize);
                }
            }
            break;
        }
        case FileMode::ReadWrite:
        {
            DWORD n;
            if (!::ReadFile(p.f, in, static_cast<DWORD>(size * wordSize), &n, 0))
            {
                throw std::runtime_error(
                    getErrorMessage(ErrorType::Read, p.path.u8string(), getLastError()));
            }
            if (p.endianConversion && wordSize > 1)
            {
                endian(in, size, wordSize);
            }
            break;
        }
        default: break;
        }
        p.pos += size * wordSize;
    }

    void FileIO::write(const void* in, size_t size, size_t wordSize)
    {
        FTK_P();

        if (!p.f)
        {
            throw std::runtime_error(
                getErrorMessage(ErrorType::Write, p.path.u8string()));
        }

        const uint8_t* inP = reinterpret_cast<const uint8_t*>(in);
        std::vector<uint8_t> tmp;
        if (p.endianConversion && wordSize > 1)
        {
            tmp.resize(size * wordSize);
            endian(in, tmp.data(), size, wordSize);
            inP = tmp.data();
        }

        DWORD n = 0;
        if (!::WriteFile(p.f, inP, static_cast<DWORD>(size * wordSize), &n, 0))
        {
            throw std::runtime_error(
                getErrorMessage(ErrorType::Write, p.path.u8string(), getLastError()));
        }
        p.pos += size * wordSize;
        p.size = std::max(p.pos, p.size);
    }

    void FileIO::_open(
        const std::filesystem::path& path,
        FileMode mode,
        FileRead readType)
    {
        FTK_P();

        _close();

        // Open the file.
        DWORD desiredAccess = 0;
        DWORD shareMode = 0;
        DWORD disposition = 0;
        DWORD flags =
            //FILE_ATTRIBUTE_NORMAL;
            FILE_FLAG_SEQUENTIAL_SCAN;
        switch (mode)
        {
        case FileMode::Read:
            desiredAccess = GENERIC_READ;
            shareMode = FILE_SHARE_READ;
            disposition = OPEN_EXISTING;
            break;
        case FileMode::Write:
            desiredAccess = GENERIC_WRITE;
            disposition = CREATE_ALWAYS;
            break;
        case FileMode::ReadWrite:
            desiredAccess = GENERIC_READ | GENERIC_WRITE;
            shareMode = FILE_SHARE_READ;
            disposition = OPEN_EXISTING;
            break;
        case FileMode::Append:
            desiredAccess = GENERIC_WRITE;
            disposition = OPEN_EXISTING;
            break;
        default: break;
        }
        try
        {
            p.f = CreateFileW(
                path.wstring().c_str(),
                desiredAccess,
                shareMode,
                0,
                disposition,
                flags,
                0);
        }
        catch (const std::exception&)
        {
            p.f = INVALID_HANDLE_VALUE;
        }
        if (INVALID_HANDLE_VALUE == p.f)
        {
            throw std::runtime_error(
                getErrorMessage(ErrorType::Open, path.u8string(), getLastError()));
        }
        p.path = path;
        p.mode = mode;
        p.readType = readType;
        p.pos = 0;
        p.size = std::filesystem::file_size(path);

        // Memory mapping.
        if (FileRead::MMap == p.readType &&
            FileMode::Read == p.mode &&
            p.size > 0)
        {
            p.mMap = CreateFileMapping(p.f, 0, PAGE_READONLY, 0, 0, 0);
            if (!p.mMap)
            {
                throw std::runtime_error(
                    getErrorMessage(ErrorType::MMap, path.u8string(), getLastError()));
            }

            p.memStart = reinterpret_cast<const uint8_t*>(MapViewOfFile(p.mMap, FILE_MAP_READ, 0, 0, 0));
            if (!p.memStart)
            {
                throw std::runtime_error(
                    getErrorMessage(ErrorType::MMap, path.u8string()));
            }

            p.memEnd = p.memStart + p.size;
            p.memP = p.memStart;
        }

        // Seek to the end when appending.
        if (FileMode::Append == mode)
        {
            seek(p.size, SeekMode::Forward);
        }
    }

    bool FileIO::_close(std::string* error)
    {
        FTK_P();

        bool out = true;

        if (p.mMap)
        {
            if (p.memStart)
            {
                if (!::UnmapViewOfFile((void*)p.memStart))
                {
                    out = false;
                    if (error)
                    {
                        *error = getErrorMessage(
                            ErrorType::CloseMMap, p.path.u8string(), getLastError());
                    }
                }
                p.memStart = nullptr;
            }

            if (!::CloseHandle(p.mMap))
            {
                out = false;
                if (error)
                {
                    *error = getErrorMessage(
                        ErrorType::Close, p.path.u8string(), getLastError());
                }
            }
            p.mMap = nullptr;
        }
        p.memEnd = nullptr;
        p.memP = nullptr;

        if (p.f != INVALID_HANDLE_VALUE)
        {
            CloseHandle(p.f);
            p.f = INVALID_HANDLE_VALUE;
        }

        p.path = std::filesystem::path();
        p.mode = FileMode::First;
        p.pos = 0;
        p.size = 0;

        return out;
    }

    void FileIO::Private::seek(size_t value, SeekMode seekMode)
    {
        if (FileMode::Read == mode && memStart)
        {
            switch (seekMode)
            {
            case SeekMode::Set:
                memP = reinterpret_cast<const uint8_t*>(memStart) + value;
                if (memP > memEnd)
                {
                    throw std::runtime_error(
                        getErrorMessage(ErrorType::SeekMMap, path.u8string()));
                }
                break;
            case SeekMode::Forward:
                memP += value;
                if (memP > memEnd)
                {
                    throw std::runtime_error(
                        getErrorMessage(ErrorType::SeekMMap, path.u8string()));
                }
                break;
            case SeekMode::Reverse:
                memP -= value;
                if (memP < memStart)
                {
                    throw std::runtime_error(
                        getErrorMessage(ErrorType::SeekMMap, path.u8string()));
                }
                break;
            default: break;
            }
        }
        else
        {
            LARGE_INTEGER v;
            v.QuadPart = value;
            DWORD move = FILE_BEGIN;
            switch (seekMode)
            {
            case SeekMode::Forward:
                move = FILE_CURRENT;
                break;
            case SeekMode::Reverse:
                v.QuadPart = -v.QuadPart;
                move = FILE_CURRENT;
                break;
            default: break;
            }
            if (!::SetFilePointerEx(f, v, 0, move))
            {
                throw std::runtime_error(
                    getErrorMessage(ErrorType::Seek, path.u8string(), getLastError()));
            }
        }
        switch (seekMode)
        {
        case SeekMode::Set: pos = value; break;
        case SeekMode::Forward: pos += value; break;
        case SeekMode::Reverse: pos -= value; break;
        default: break;
        }
    }

    void truncateFile(const std::filesystem::path& path, size_t size)
    {
        HANDLE h = INVALID_HANDLE_VALUE;
        try
        {
            h = CreateFileW(
                path.wstring().c_str(),
                GENERIC_WRITE,
                0,
                0,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                0);
        }
        catch (const std::exception&)
        {
            h = INVALID_HANDLE_VALUE;
        }
        if (INVALID_HANDLE_VALUE == h)
        {
            throw std::runtime_error(
                getErrorMessage(ErrorType::Open, path.u8string(), getLastError()));
        }
        LARGE_INTEGER v;
        v.QuadPart = size;
        if (!::SetFilePointerEx(
            h,
            static_cast<LARGE_INTEGER>(v),
            0,
            FILE_BEGIN))
        {
            CloseHandle(h);
            throw std::runtime_error(
                getErrorMessage(ErrorType::Seek, path.u8string(), getLastError()));
        }
        if (!::SetEndOfFile(h))
        {
            CloseHandle(h);
            throw std::runtime_error(
                getErrorMessage(ErrorType::Write, path.u8string(), getLastError()));
        }
        CloseHandle(h);
    }
}
