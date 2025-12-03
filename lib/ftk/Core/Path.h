// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Range.h>
#include <ftk/Core/Util.h>

#include <filesystem>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace ftk
{
    //! \name File Paths
    ///@{

    //! Does the file name start with a dot?
    bool isDotFile(const std::string&);

    //! Split a path.
    std::vector<std::string> split(std::filesystem::path);

    //! Append a path separator.
    std::string appendSeparator(const std::string&);

    //! Get the list of file system drives.
    std::vector<std::filesystem::path> getDrives();

    //! User paths.
    enum class UserPath
    {
        Home,
        Desktop,
        Documents,
        Downloads,

        Count,
        First = Home
    };
    FTK_ENUM(UserPath);

    //! Get a user path.
    std::filesystem::path getUserPath(UserPath);

    //! Convert a frame number to a string.
    std::string toString(int64_t frame, int pad = 0);

    //! File path options.
    struct PathOptions
    {
        bool   seqNegative  = true;
        size_t seqMaxDigits = 9;

        bool operator == (const PathOptions&) const;
        bool operator != (const PathOptions&) const;
    };
    
    //! File path.
    //! 
    //! Example: file:///tmp/render.0001.exr?user=foo;password=bar
    //! * protocol: file://
    //! * dir: /tmp/
    //! * base: render.
    //! * number: 0001
    //! * padding: 4
    //! * extension: .exr
    //! * request: ?user=foo;password=bar
    //! * file name: render.0001.exr
    class Path
    {
    public:
        Path() = default;
        explicit Path(
            const std::string&,
            const PathOptions& = PathOptions());
        Path(
            const std::string& dir,
            const std::string& fileName,
            const PathOptions& = PathOptions());

        //! \name Path Options
        ///@{

        const PathOptions& getOptions();
        void setOptions(const PathOptions&);

        ///@}

        //! \name Path Components
        ///@{

        const std::string& get() const;
        bool isEmpty() const;

        bool hasProtocol() const;
        bool hasDir() const;
        bool hasBase() const;
        bool hasNum() const;
        bool hasExt() const;
        bool hasRequest() const;

        std::string getProtocol() const;
        std::string getDir() const;
        std::string getBase() const;
        std::string getNum() const;
        int getPad() const;
        std::string getExt() const;
        std::string getRequest() const;
        std::string getFileName(bool dir = false) const;

        void setProtocol(const std::string&);
        void setDir(const std::string&);
        void setBase(const std::string&);
        void setNum(const std::string&);
        void setPad(int);
        void setExt(const std::string&);
        void setRequest(const std::string&);
        void setFileName(const std::string&);

        ///@}

        //! \name File Sequences
        ///@{

        const std::optional<RangeI64>& getFrames() const;
        void setFrames(const RangeI64&);

        //! Get whether this is a sequence.
        bool isSeq() const;

        //! Get whether this has a sequence wildcard ('#').
        bool hasSeqWildcard() const;

        //! Get a file name with the given frame number.
        std::string getFrame(int64_t frame, bool dir = false) const;

        //! Get the frame range string.
        std::string getFrameRange() const;

        //! Get whether a path is part of this sequence.
        bool seq(const Path&) const;

        //! Add a path to this sequence.
        bool addSeq(const Path&);

        ///@}

        //! \name Utility
        ///@{

        //! Get whether the path is absolute.
        bool isAbs() const;

        //! Test whether this extension matches one in the given set.
        bool testExt(const std::vector<std::string>&) const;

        ///@}

        //! \name Constants
        ///@{

        static const std::string numbers;
        static const std::string pathSeparators;

        ///@}

        bool operator == (const Path&) const;
        bool operator != (const Path&) const;

    private:
        void _parse(const PathOptions&);

        std::string _path;
        PathOptions _options;
        static const std::pair<size_t, size_t> _invalid;
        std::pair<size_t, size_t> _protocol = _invalid;
        std::pair<size_t, size_t> _dir = _invalid;
        std::pair<size_t, size_t> _base = _invalid;
        std::pair<size_t, size_t> _num = _invalid;
        int _pad = 0;
        std::pair<size_t, size_t> _ext = _invalid;
        std::pair<size_t, size_t> _request = _invalid;
        std::optional<RangeI64> _frames;
    };

    //! Directory list sorting.
    enum class DirListSort
    {
        Name,
        Extension,
        Size,
        Time,

        Count,
        First = Name
    };
    FTK_ENUM(DirListSort);

    //! Directory list options.
    struct DirListOptions
    {
        DirListSort              sort         = DirListSort::Name;
        bool                     sortReverse  = false;
        std::string              filter;
        bool                     filterFiles  = false;
        std::vector<std::string> filterExt;
        bool                     seq          = true;
        std::vector<std::string> seqExts;
        bool                     seqNegative  = true;
        size_t                   seqMaxDigits = 9;
        bool                     hidden       = false;

        bool operator == (const DirListOptions&) const;
        bool operator != (const DirListOptions&) const;
    };

    //! Directory list entry.
    struct DirEntry
    {
        Path                            path;
        bool                            isDir = false;
        size_t                          size = 0;
        std::filesystem::file_time_type time;

        bool operator == (const DirEntry&) const;
        bool operator != (const DirEntry&) const;
    };

    //! List directory contents.
    std::vector<DirEntry> dirList(
        const std::filesystem::path&,
        const DirListOptions& = DirListOptions());

    //! Expand a file sequence. This function will search the directory for
    //! other frames that match the given frame.
    bool expandSeq(
        const std::filesystem::path&,
        Path&,
        const PathOptions& = PathOptions());

    void to_json(nlohmann::json&, const PathOptions&);
    void to_json(nlohmann::json&, const DirListOptions&);

    void from_json(const nlohmann::json&, PathOptions&);
    void from_json(const nlohmann::json&, DirListOptions&);

    ///@}
}

#include <ftk/Core/PathInline.h>
