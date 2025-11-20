// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

namespace ftk
{
    inline bool isDotFile(const std::string& fileName)
    {
        return !fileName.empty() && '.' == fileName[0];
    }

    inline const std::string& Path::get() const
    {
        return _path;
    }

    inline bool Path::hasProtocol() const
    {
        return _protocol.first != std::string::npos && _protocol.second != std::string::npos;
    }

    inline bool Path::hasDir() const
    {
        return _dir.first != std::string::npos && _dir.second != std::string::npos;
    }

    inline bool Path::hasBase() const
    {
        return _base.first != std::string::npos && _base.second != std::string::npos;
    }

    inline bool Path::hasNum() const
    {
        return _num.first != std::string::npos && _num.second != std::string::npos;
    }

    inline bool Path::hasExt() const
    {
        return _ext.first != std::string::npos && _ext.second != std::string::npos;
    }

    inline bool Path::hasRequest() const
    {
        return _request.first != std::string::npos && _request.second != std::string::npos;
    }

    inline std::string Path::getProtocol() const
    {
        return _protocol.first != std::string::npos && _protocol.second != std::string::npos ?
            _path.substr(_protocol.first, _protocol.second) :
            std::string();
    }

    inline std::string Path::getDir() const
    {
        return _dir.first != std::string::npos && _dir.second != std::string::npos ?
            _path.substr(_dir.first, _dir.second) :
            std::string();
    }

    inline std::string Path::getBase() const
    {
        return _base.first != std::string::npos && _base.second != std::string::npos ?
            _path.substr(_base.first, _base.second) :
            std::string();
    }

    inline std::string Path::getNum() const
    {
        return _num.first != std::string::npos && _num.second != std::string::npos ?
            _path.substr(_num.first, _num.second) :
            std::string();
    }

    inline int Path::getPad() const
    {
        return _pad;
    }

    inline std::string Path::getExt() const
    {
        return _ext.first != std::string::npos && _ext.second != std::string::npos ?
            _path.substr(_ext.first, _ext.second) :
            std::string();
    }

    inline std::string Path::getRequest() const
    {
        return _request.first != std::string::npos && _request.second != std::string::npos ?
            _path.substr(_request.first, _request.second) :
            std::string();
    }

    inline const FrameRange& Path::getFrames() const
    {
        return _frames;
    }

    inline std::string Path::getPath(int64_t frame) const
    {
        return _num.first != std::string::npos && _num.second != std::string::npos ?
            (getDir() + getBase() + toString(frame, _pad) + getExt()) :
            (getDir() + getBase() + getExt());
    }

    inline std::string Path::getFileName(int64_t frame) const
    {
        return _num.first != std::string::npos && _num.second != std::string::npos ?
            (getBase() + toString(frame, _pad) + getExt()) :
            (getBase() + getExt());
    }

    inline bool Path::seq(const Path& other) const
    {
        return
            hasNum() &&
            other.hasNum() &&
            _dir == other._dir &&
            _base == other._base &&
            getExt() == other.getExt();
    }

    inline bool Path::operator == (const Path& other) const
    {
        return _path == other._path && _frames == other._frames;
    }

    inline bool Path::operator != (const Path& other) const
    {
        return !(*this == other);
    }
}
