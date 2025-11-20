// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Range.h>
#include <ftk/Core/Util.h>

#include <filesystem>
#include <iostream>
#include <vector>

namespace ftk
{
    //! \name File Paths
    ///@{

    //! Does the file name start with a dot?
    bool isDotFile(const std::string&);

    //! Split a path.
    std::vector<std::string> split(std::filesystem::path);

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

    //! Frame range.
    typedef Range<int64_t> FrameRange;

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
    class Path
    {
    public:
        Path() = default;
        explicit Path(const std::string&);

        //! \name Path Components
        ///@{

        const std::string& get() const;

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

        ///@}

        //! \name File Sequences
        ///@{

        const FrameRange& getFrames() const;
        void setFrames(const FrameRange&);

        //! Get a path with a frame number.
        std::string getPath(int64_t frame) const;

        //! Get a file name with a frame number.
        std::string getFileName(int64_t frame) const;

        //! Get whether a path is part of this sequence.
        bool seq(const Path&) const;

        //! Add a path to this sequence.
        bool addSeq(const Path&);

        ///@}

        //! \name Constants
        ///@{

        static const std::string numbers;
        static const std::string pathSeparators;

        ///@}

        bool operator == (const Path&) const;
        bool operator != (const Path&) const;

    private:
        std::string _path;
        std::pair<size_t, size_t> _protocol = { std::string::npos, std::string::npos };
        std::pair<size_t, size_t> _dir = { std::string::npos, std::string::npos };
        std::pair<size_t, size_t> _base = { std::string::npos, std::string::npos };
        std::pair<size_t, size_t> _num = { std::string::npos, std::string::npos };
        int _pad = 0;
        std::pair<size_t, size_t> _ext = { std::string::npos, std::string::npos };
        std::pair<size_t, size_t> _request = { std::string::npos, std::string::npos };
        FrameRange _frames;
    };

    ///@}
}

#include <ftk/Core/PathInline.h>