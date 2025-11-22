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

    inline bool Path::isEmpty() const
    {
        return _path.empty();
    }

    inline bool Path::hasProtocol() const
    {
        return _protocol != _invalid;
    }

    inline bool Path::hasDir() const
    {
        return _dir != _invalid;
    }

    inline bool Path::hasBase() const
    {
        return _base != _invalid;
    }

    inline bool Path::hasNum() const
    {
        return _num != _invalid;
    }

    inline bool Path::hasExt() const
    {
        return _ext != _invalid;
    }

    inline bool Path::hasRequest() const
    {
        return _request != _invalid;
    }

    inline std::string Path::getProtocol() const
    {
        return _protocol != _invalid ?
            _path.substr(_protocol.first, _protocol.second) :
            std::string();
    }

    inline std::string Path::getDir() const
    {
        return _dir != _invalid ?
            _path.substr(_dir.first, _dir.second) :
            std::string();
    }

    inline std::string Path::getBase() const
    {
        return _base != _invalid ?
            _path.substr(_base.first, _base.second) :
            std::string();
    }

    inline std::string Path::getNum() const
    {
        return _num != _invalid ?
            _path.substr(_num.first, _num.second) :
            std::string();
    }

    inline int Path::getPad() const
    {
        return _pad;
    }

    inline std::string Path::getExt() const
    {
        return _ext != _invalid ?
            _path.substr(_ext.first, _ext.second) :
            std::string();
    }

    inline std::string Path::getRequest() const
    {
        return _request != _invalid ?
            _path.substr(_request.first, _request.second) :
            std::string();
    }

    inline std::string Path::getFileName(bool dir) const
    {
        return dir ?
            getDir() + getBase() + getNum() + getExt() :
            getBase() + getNum() + getExt();
    }

    inline const RangeI64& Path::getFrames() const
    {
        return _frames;
    }

    inline std::string Path::getFrame(int64_t frame, bool dir) const
    {
        return _num != _invalid ?
            ((dir ? getDir() : std::string()) + getBase() + toString(frame, _pad) + getExt()) :
            ((dir ? getDir() : std::string()) + getBase() + getExt());
    }

    inline std::string Path::getFrameRange() const
    {
        return hasNum() ?
            _frames.min() != _frames.max() ?
                toString(_frames.min(), _pad) + "-" + toString(_frames.max(), _pad) :
                toString(_frames.min(), _pad) :
            std::string();
    }

    inline bool Path::seq(const Path& other) const
    {
        return
            (hasNum() || "#" == getNum()) &&
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
