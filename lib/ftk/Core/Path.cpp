// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#include <ftk/Core/Path.h>

#include <ftk/Core/Error.h>
#include <ftk/Core/String.h>

#include <algorithm>
#include <array>
#include <list>
#include <sstream>

namespace ftk
{
    std::vector<std::string> split(std::filesystem::path path)
    {
        std::list<std::string> out;
        const std::filesystem::path root = path.root_path();
        while (!path.empty() && path != root)
        {
            if (!path.filename().empty())
            {
                out.push_front(path.filename().u8string());
            }
            path = path.parent_path();
        }
        if (!path.empty())
        {
            out.push_front(path.u8string());
        }
        return std::vector<std::string>(out.begin(), out.end());
    }

    FTK_ENUM_IMPL(
        UserPath,
        "Home",
        "Desktop",
        "Documents",
        "Downloads");

    std::string toString(int64_t frame, int pad)
    {
        std::stringstream ss;
        if (pad > 0)
        {
            ss << std::setfill('0');
            ss << std::setw(pad);
        }
        ss << frame;
        return ss.str();
    }

    void Path::_init(const std::string& value)
    {
        _path = value;

        // Find the request.
        size_t size = _path.size();
        size_t requestPos = std::string::npos;
        if (size > 0)
        {
            for (int i = 0; i < size; ++i)
            {
                if ('?' == _path[i])
                {
                    requestPos = i;
                    break;
                }
            }
        }
        if (requestPos != std::string::npos)
        {
            const size_t sizeTmp = size - requestPos;
            _request = std::pair<size_t, size_t>(requestPos, sizeTmp);
            size -= sizeTmp;
        }

        // Find the protocol.
        size_t protocolEnd = std::string::npos;
        size_t protocolSize = 0;
        if (size > 2)
        {
            for (int i = 0; i < size - 3; ++i)
            {
                if (':' == _path[i] &&
                    '/' == _path[i + 1] &&
                    '/' == _path[i + 2])
                {
                    protocolEnd = i + 2;
                    protocolSize = protocolEnd + 1;
                    break;
                }
            }
        }
        if (protocolEnd != std::string::npos)
        {
            _protocol = std::pair<size_t, size_t>(0, protocolSize);
        }

        // Find the directory.
        size_t dirEnd = std::string::npos;
        size_t dirSize = 0;
        if (size > 0)
        {
            for (int i = size - 1; i >= static_cast<int>(protocolSize); --i)
            {
                if (pathSeparators.find(_path[i]) != std::string::npos)
                {
                    dirEnd = i;
                    dirSize = dirEnd + 1 - protocolSize;
                    break;
                }
            }
        }
        if (std::string::npos == dirEnd &&
            size > 1 &&
            _path[0] >= 'A' && _path[0] <= 'Z' &&
            ':' == _path[1])
        {
            dirEnd = 1;
            dirSize = 2;
            _dir = std::pair<size_t, size_t>(0, dirSize);
        }
        else if (dirEnd != std::string::npos)
        {
            _dir = std::pair<size_t, size_t>(protocolSize, dirSize);
        }
        const size_t protocolDirSize = protocolSize + dirSize;

        // Find the extension.
        size_t extPos = std::string::npos;
        if (size > 0)
        {
            for (int i = size - 1; i >= static_cast<int>(protocolDirSize); --i)
            {
                if ('.' == _path[i])
                {
                    extPos = i;
                    break;
                }
            }
        }
        if (extPos != std::string::npos &&
            extPos > protocolDirSize &&
            extPos < size - 1)
        {
            const size_t sizeTmp = size - extPos;
            _ext = std::pair<size_t, size_t>(extPos, sizeTmp);
            size -= sizeTmp;
        }

        // Find the number.
        size_t numPos = std::string::npos;
        if (size > 0)
        {
            for (int i = size - 1; i >= static_cast<int>(protocolDirSize); --i)
            {
                if (numbers.find(_path[i]) != std::string::npos)
                {
                    numPos = i;
                }
                else
                {
                    break;
                }
            }
        }
        if (numPos != std::string::npos)
        {
            const size_t sizeTmp = size - numPos;
            _num = std::pair<size_t, size_t>(numPos, sizeTmp);
            if ('0' == _path[numPos] || '#' == _path[numPos])
            {
                _pad = sizeTmp;
            }
            const int64_t frame = std::atoi(getNum().c_str());
            _frames = FrameRange(frame, frame);
            size -= sizeTmp;
        }

        // Find the base name.
        if (size - protocolDirSize > 0)
        {
            _base = std::pair<size_t, size_t>(
                protocolDirSize,
                size - protocolDirSize);
        }
    }

    Path::Path(const std::string& value)
    {
        _init(value);
    }

    void Path::setFrames(const FrameRange& value)
    {
        _frames = value;
    }

    bool Path::addSeq(const Path& other)
    {
        bool out = seq(other);
        if (out)
        {
            _frames = expand(_frames, other._frames);
            _pad = std::max(_pad, other._pad);
        }
        return out;
    }

    const std::string Path::numbers = "0123456789#";
    const std::string Path::pathSeparators = "/\\";
}
