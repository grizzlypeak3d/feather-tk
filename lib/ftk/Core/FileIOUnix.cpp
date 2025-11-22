// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/FileIO.h>

#include <ftk/Core/Format.h>
#include <ftk/Core/Memory.h>
#include <ftk/Core/Path.h>

#include <filesystem>

#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

namespace ftk
{
    namespace
    {
        enum class ErrorType
        {
            Open,
            MMap,
            Close,
            CloseMMap,
            Read,
            ReadMMap,
            Write,
            Seek,
            SeekMMap
        };

        std::string getErrorString()
        {
            std::string out;
            char buf[cStringSize] = "";
#if defined(_GNU_SOURCE)
            out = strerror_r(errno, buf, cStringSize);
#else // _GNU_SOURCE
            strerror_r(errno, buf, cStringSize);
            out = buf;
#endif // _GNU_SOURCE
            return out;
        }
        
        std::string getErrorMessage(
            ErrorType          type,
            const std::string& path,
            const std::string& message  = std::string())
        {
            std::string out;
            switch (type)
            {
            case ErrorType::Open:
                out = Format("Cannot open file \"{0}\"").arg(path);
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
        int                   f = -1;
        void*                 mMap = reinterpret_cast<void*>(-1);
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
        const InMemFile& mem)
    {
        auto out = std::shared_ptr<FileIO>(new FileIO);
        out->_p->path = path;
        out->_p->mode = FileMode::Read;
        out->_p->readType = FileRead::Normal;
        out->_p->size = mem.size;
        out->_p->memStart = mem.p;
        out->_p->memEnd = mem.p + mem.size;
        out->_p->memP = mem.p;
        return out;
    }
    
    bool FileIO::isOpen() const
    {
        return _p->f != -1 || _p->memStart;
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
            out |= -1 == p.f;
        }
        out |= p.pos >= p.size;
        return out;
    }
    
    void FileIO::read(void* in, size_t size, size_t wordSize)
    {
        FTK_P();
        
        if (!p.memStart && -1 == p.f)
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
                    memcpy(in, p.memP, size * wordSize);
                }
                p.memP = memP;
            }
            else
            {
                const ssize_t r = ::read(p.f, in, size * wordSize);
                if (-1 == r)
                {
                    throw std::runtime_error(
                        getErrorMessage(ErrorType::Read, p.path.u8string(), getErrorString()));
                }
                else if (r != size * wordSize)
                {
                    throw std::runtime_error(
                        getErrorMessage(ErrorType::Read, p.path.u8string()));
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
            const ssize_t r = ::read(p.f, in, size * wordSize);
            if (-1 == r)
            {
                throw std::runtime_error(
                    getErrorMessage(ErrorType::Read, p.path.u8string(), getErrorString()));
            }
            else if (r != size * wordSize)
            {
                throw std::runtime_error(
                    getErrorMessage(ErrorType::Read, p.path.u8string()));
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
        
        if (-1 == p.f)
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
        if (::write(p.f, inP, size * wordSize) == -1)
        {
            throw std::runtime_error(
                getErrorMessage(ErrorType::Write, p.path.u8string(), getErrorString()));
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
        int openFlags = 0;
        int openMode  = 0;
        switch (mode)
        {
        case FileMode::Read:
            openFlags = O_RDONLY;
            break;
        case FileMode::Write:
            openFlags = O_WRONLY | O_CREAT | O_TRUNC;
            openMode  = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
            break;
        case FileMode::ReadWrite:
            openFlags = O_RDWR | O_CREAT;
            openMode  = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
            break;
        case FileMode::Append:
            openFlags = O_WRONLY | O_CREAT | O_APPEND;
            openMode  = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
            break;
        default: break;
        }
        p.f = ::open(path.u8string().c_str(), openFlags, openMode);
        if (-1 == p.f)
        {
            throw std::runtime_error(
                getErrorMessage(ErrorType::Open, path.u8string(), getErrorString()));
        }

        // File information.
        p.path     = path;
        p.mode     = mode;
        p.readType = readType;
        p.pos      = 0;
        p.size     = std::filesystem::file_size(path);
        
        // Memory mapping.
        if (FileRead::MMap == p.readType &&
            FileMode::Read == p.mode &&
            p.size > 0)
        {
            p.mMap = mmap(0, p.size, PROT_READ, MAP_SHARED, p.f, 0);
            madvise(p.mMap, p.size, MADV_SEQUENTIAL | MADV_SEQUENTIAL);
            if (p.mMap == (void*)-1)
            {
                throw std::runtime_error(
                    getErrorMessage(ErrorType::MMap, path.u8string(), getErrorString()));
            }
            p.memStart = reinterpret_cast<const uint8_t*>(p.mMap);
            p.memEnd   = p.memStart + p.size;
            p.memP     = p.memStart;
        }
    }

    bool FileIO::_close(std::string* error)
    {
        FTK_P();
        
        bool out = true;
        
        if (p.mMap != (void*)-1)
        {
            int r = munmap(p.mMap, p.size);
            if (-1 == r)
            {
                out = false;
                if (error)
                {
                    *error = getErrorMessage(ErrorType::CloseMMap, p.path.u8string(), getErrorString());
                }
            }
            p.mMap = (void*)-1;
        }
        p.memStart = nullptr;
        p.memEnd   = nullptr;

        if (p.f != -1)
        {
            int r = ::close(p.f);
            if (-1 == r)
            {
                out = false;
                if (error)
                {
                    *error = getErrorMessage(ErrorType::Close, p.path.u8string(), getErrorString());
                }
            }
            p.f = -1;
        }

        p.path = std::filesystem::path();
        p.mode = FileMode::First;
        p.pos  = 0;
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
            off_t offset = value;
            int whence = SEEK_SET;
            switch (seekMode)
            {
            case SeekMode::Forward:
                whence = SEEK_CUR;
                break;
            case SeekMode::Reverse:
                offset = -offset;
                whence = SEEK_CUR;
                break;
            default: break;
            }
            if (::lseek(f, offset, whence) == (off_t)-1)
            {
                throw std::runtime_error(
                    getErrorMessage(ErrorType::Seek, path.u8string(), getErrorString()));
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
        if (::truncate(path.u8string().c_str(), size) != 0)
        {
            throw std::runtime_error(
                getErrorMessage(ErrorType::Write, path.u8string(), getErrorString()));
        }                
    }
}
