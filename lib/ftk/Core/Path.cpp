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

    bool PathOptions::operator == (const PathOptions& other) const
    {
        return
            seqNegative == other.seqNegative &&
            seqMaxDigits == other.seqMaxDigits;
    }

    bool PathOptions::operator != (const PathOptions& other) const
    {
        return !(*this == other);
    }

    Path::Path(const std::string& value, const PathOptions& options) :
        _path(value),
        _options(options)
    {
        _parse(options);
    }

    const PathOptions& Path::getOptions()
    {
        return _options;
    }

    void Path::setOptions(const PathOptions& value)
    {
        _options = value;
        _parse(_options);
    }

    void Path::setProtocol(const std::string& value)
    {
        _path = value + getDir() + getBase() + getNum() + getExt() + getRequest();
        const RangeI64 tmp = _frames;
        _parse(_options);
        _frames = tmp;
    }

    void Path::setDir(const std::string& value)
    {
        _path = getProtocol() + value + getBase() + getNum() + getExt() + getRequest();
        const RangeI64 tmp = _frames;
        _parse(_options);
        _frames = tmp;
    }

    void Path::setBase(const std::string& value)
    {
        _path = getProtocol() + getDir() + value + getNum() + getExt() + getRequest();
        const RangeI64 tmp = _frames;
        _parse(_options);
        _frames = tmp;
    }

    void Path::setNum(const std::string& value)
    {
        _path = getProtocol() + getDir() + getBase() + value + getExt() + getRequest();
        const RangeI64 tmp = _frames;
        _parse(_options);
        _frames = tmp;
    }

    void Path::setPad(int value)
    {
        _pad = value;
        std::string num = getNum();
        if (!num.empty())
        {
            num = toString(std::atoi(num.c_str()), _pad);
        }
        _path = getProtocol() + getDir() + getBase() + num + getExt() + getRequest();
        const RangeI64 tmp = _frames;
        _parse(_options);
        _frames = tmp;
    }

    void Path::setExt(const std::string& value)
    {
        _path = getProtocol() + getDir() + getBase() + getNum() + value + getRequest();
        const RangeI64 tmp = _frames;
        _parse(_options);
        _frames = tmp;
    }

    void Path::setRequest(const std::string& value)
    {
        _path = getProtocol() + getDir() + getBase() + getNum() + getExt() + value;
        const RangeI64 tmp = _frames;
        _parse(_options);
        _frames = tmp;
    }

    void Path::setFileName(const std::string& value)
    {
        _path = getProtocol() + getDir() + value + getRequest();
        const RangeI64 tmp = _frames;
        _parse(_options);
        _frames = tmp;
    }

    void Path::setFrames(const RangeI64& value)
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

    void Path::_parse(const PathOptions& options)
    {
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
            if (numPos != std::string::npos &&
                size - numPos > options.seqMaxDigits)
            {
                numPos = std::string::npos;
            }
            if (options.seqNegative &&
                numPos != std::string::npos &&
                numPos > protocolDirSize &&
                '-' == _path[numPos - 1])
            {
                --numPos;
            }
        }
        if (numPos != std::string::npos)
        {
            const size_t sizeTmp = size - numPos;
            _num = std::pair<size_t, size_t>(numPos, sizeTmp);
            if ('0' == _path[numPos])
            {
                _pad = sizeTmp;
            }
            else if ('#' == _path[numPos])
            {
                _pad = sizeTmp;
            }
            if (options.seqNegative &&
                '-' == _path[numPos] &&
                numPos < size - 1 &&
                '0' == _path[numPos + 1])
            {
                _pad = sizeTmp - 1;
            }
            const int64_t frame = std::atoi(getNum().c_str());
            _frames = RangeI64(frame, frame);
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

    FTK_ENUM_IMPL(
        DirListSort,
        "Name",
        "Extension",
        "Size",
        "Time");

    bool DirListOptions::operator == (const DirListOptions& other) const
    {
        return
            sort == other.sort &&
            sortReverse == other.sortReverse &&
            filter == other.filter &&
            filterFiles == other.filterFiles &&
            filterExt == other.filterExt &&
            seq == other.seq &&
            seqExts == other.seqExts &&
            seqNegative == other.seqNegative &&
            seqMaxDigits == other.seqMaxDigits &&
            hidden == other.hidden;
    }

    bool DirListOptions::operator != (const DirListOptions& other) const
    {
        return !(*this == other);
    }

    bool DirEntry::operator == (const DirEntry& other) const
    {
        return
            path == other.path &&
            isDir == other.isDir &&
            size == other.size &&
            time == other.time;
    }

    bool DirEntry::operator != (const DirEntry& other) const
    {
        return !(*this == other);
    }

    std::vector<DirEntry> dirList(
        const std::filesystem::path& path,
        const DirListOptions& options)
    {
        std::vector<DirEntry> out;
        PathOptions pathOptions;
        pathOptions.seqNegative = options.seqNegative;
        pathOptions.seqMaxDigits = options.seqMaxDigits;
        try
        {
            for (const auto& i : std::filesystem::directory_iterator(path))
            {
                const Path path(i.path().u8string(), pathOptions);
                const std::string fileName = i.path().filename().u8string();

                // Apply filters.
                bool keep = true;
                if (keep && !options.hidden && isDotFile(fileName))
                {
                    keep = false;
                }
                const bool isDir = std::filesystem::is_directory(i.path());
                if (keep && !isDir && !options.filterExt.empty())
                {
                    keep = compare(
                        options.filterExt,
                        path.getExt(),
                        CaseCompare::Insensitive);
                }
                if (keep && !options.filter.empty())
                {
                    keep = contains(
                        fileName,
                        options.filter,
                        CaseCompare::Insensitive);
                }
                if (keep && options.filterFiles && !isDir)
                {
                    keep = false;
                }

                if (keep)
                {
                    // Check for sequences.
                    bool seq = false;
                    bool seqExt = true;
                    if (!options.seqExts.empty())
                    {
                        seqExt = options.seqExts.find(toLower(path.getExt())) != options.seqExts.end();
                    }
                    if (options.seq && seqExt && !isDir)
                    {
                        for (auto& j : out)
                        {
                            if (j.path.addSeq(path))
                            {
                                seq = true;
                                j.size += std::filesystem::file_size(i.path());
                                j.time = std::max(
                                    j.time,
                                    std::filesystem::last_write_time(i.path()));
                                break;
                            }
                        }
                    }

                    if (!seq)
                    {
                        // Add the entry.
                        out.push_back({
                            path,
                            isDir,
                            isDir ? 0 : std::filesystem::file_size(i.path()),
                            std::filesystem::last_write_time(i.path()) });
                    }
                }
            }
        }
        catch (const std::exception&)
        {}

        // Seq sequence numbers to their first frame.
        for (auto& i : out)
        {
            const auto& frames = i.path.getFrames();
            if (!frames.equal())
            {
                i.path.setNum(toString(frames.min(), i.path.getPad()));
            }
        }

        // Sort the entries.
        std::function<int(const DirEntry& a, const DirEntry& b)> sort;
        switch (options.sort)
        {
        case DirListSort::Name:
            sort = [](const DirEntry& a, const DirEntry& b)
                {
                    return a.path.getFileName() < b.path.getFileName();
                };
            break;
        case DirListSort::Extension:
            sort = [](const DirEntry& a, const DirEntry& b)
                {
                    return a.path.getExt() < b.path.getExt();
                };
            break;
        case DirListSort::Size:
            sort = [](const DirEntry& a, const DirEntry& b)
                {
                    return a.size < b.size;
                };
            break;
        case DirListSort::Time:
            sort = [](const DirEntry& a, const DirEntry& b)
                {
                    return a.time < b.time;
                };
            break;
        default: break;
        }
        if (sort)
        {
            if (options.sortReverse)
            {
                std::sort(out.rbegin(), out.rend(), sort);
            }
            else
            {
                std::sort(out.begin(), out.end(), sort);
            }
        }

        // Sort the directories.
        std::stable_sort(
            out.begin(),
            out.end(),
            [](const DirEntry& a, const DirEntry& b)
            {
                return a.isDir > b.isDir;
            });

        return out;
    }

    bool expandSeq(
        const std::filesystem::path& stdpath,
        Path& path,
        size_t seqMaxDigits)
    {
        bool out = false;

        // Find matching sequence files.
        bool init = true;
        const Path tmp(stdpath.u8string());
        for (const auto& i : std::filesystem::directory_iterator(stdpath.parent_path()))
        {
            const Path entry(i.path().u8string());
            if (init)
            {
                out = entry.getNum().size() < seqMaxDigits && tmp.seq(entry);
                if (out)
                {
                    init = false;
                    path = entry;
                }
            }
            if (!init)
            {
                path.addSeq(entry);
            }
        }

        // Set the sequence number to the first frame.
        const auto& frames = path.getFrames();
        if (out && !frames.equal())
        {
            path.setNum(toString(frames.min(), path.getPad()));
        }

        return out;
    }

    void to_json(nlohmann::json& json, const PathOptions& value)
    {
        json["SeqNegative"] = value.seqNegative;
        json["SeqMaxDigits"] = value.seqMaxDigits;
    }

    void to_json(nlohmann::json& json, const DirListOptions& value)
    {
        json["Sort"] = to_string(value.sort);
        json["SortReverse"] = value.sortReverse;
        json["Filter"] = value.filter;
        json["FilterFiles"] = value.filterFiles;
        json["FilterExt"] = value.filterExt;
        json["Seq"] = value.seq;
        json["SeqExts"] = value.seqExts;
        json["SeqNegative"] = value.seqNegative;
        json["SeqMaxDigits"] = value.seqMaxDigits;
        json["Hidden"] = value.hidden;
    }

    void from_json(const nlohmann::json& json, PathOptions& value)
    {
        json.at("SeqNegative").get_to(value.seqNegative);
        json.at("SeqMaxDigits").get_to(value.seqMaxDigits);
    }

    void from_json(const nlohmann::json& json, DirListOptions& value)
    {
        from_string(json.at("Sort").get<std::string>(), value.sort);
        json.at("SortReverse").get_to(value.sortReverse);
        json.at("Filter").get_to(value.filter);
        json.at("FilterFiles").get_to(value.filterFiles);
        json.at("FilterExt").get_to(value.filterExt);
        json.at("Seq").get_to(value.seq);
        json.at("SeqExts").get_to(value.seqExts);
        json.at("SeqNegative").get_to(value.seqNegative);
        json.at("SeqMaxDigits").get_to(value.seqMaxDigits);
        json.at("Hidden").get_to(value.hidden);
    }
}
