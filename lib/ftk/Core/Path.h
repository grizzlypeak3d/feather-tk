// SPDX-License-Identifier: BSD-3-Clause
// Copyright Contributors to the feather-tk project.

#pragma once

#include <ftk/Core/Export.h>
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
    FTK_CORE_API bool isDotFile(const std::string&);

    //! Split a path.
    FTK_CORE_API std::vector<std::string> split(std::filesystem::path);

    //! Append a path separator.
    FTK_CORE_API std::string appendSeparator(const std::string&);

    //! Get the list of file system drives.
    FTK_CORE_API std::vector<std::filesystem::path> getDrives();

    //! User paths.
    enum class FTK_CORE_API_TYPE UserPath
    {
        Home,
        Desktop,
        Documents,
        Downloads,

        Count,
        First = Home
    };
    FTK_ENUM(FTK_CORE_API, UserPath);

    //! Get a user path.
    FTK_CORE_API std::filesystem::path getUserPath(UserPath);
    
    //! Create a unique temporary directory.
    FTK_CORE_API std::filesystem::path createTmpDir();

    //! Create a temporary directory that will automatically be removed when
    //! finished.
    class FTK_CORE_API_TYPE TmpDir
    {
        FTK_NON_COPYABLE(TmpDir);

    public:
        FTK_CORE_API TmpDir();

        FTK_CORE_API ~TmpDir();
        
        //! Get the path to the temporary directory.
        FTK_CORE_API const std::filesystem::path& getPath() const;

        //! Remove the temporary directory (this will also be called
        //! automatically in the destructor).
        FTK_CORE_API void remove();

    private:
        std::filesystem::path _path;
    };

    //! Convert a frame number to a string.
    FTK_CORE_API std::string toString(int64_t frame, int pad = 0);

    //! File path options.
    struct FTK_CORE_API_TYPE PathOptions
    {
        bool   seqNegative  = true;
        size_t seqMaxDigits = 9;

        FTK_CORE_API bool operator == (const PathOptions&) const;
        FTK_CORE_API bool operator != (const PathOptions&) const;
    };

    //! Frame sequence.
    struct FTK_CORE_API_TYPE FrameSeq
    {
        FrameSeq() = default;
        FTK_CORE_API explicit FrameSeq(const RangeI64&, int inc = 1);
        FTK_CORE_API FrameSeq(int64_t, int64_t, int inc = 1);
        FTK_CORE_API explicit FrameSeq(int64_t);

        RangeI64 range;
        int      inc = 1;

        FTK_CORE_API bool operator == (const FrameSeq&) const;
        FTK_CORE_API bool operator != (const FrameSeq&) const;
    };

    //! Convert frames to frame sequences.
    FTK_CORE_API std::vector<FrameSeq> toFrameSeq(const std::vector<int64_t>&);

    //! Convert a frame sequence to frames.
    FTK_CORE_API std::vector<int64_t> toFrames(const FrameSeq&);

    //! Convert frame sequences to frames.
    FTK_CORE_API std::vector<int64_t> toFrames(const std::vector<FrameSeq>&);

    //! Add a frame to a list of frame sequences, which is kept sorted and
    //! merged. Frames already in the list are ignored.
    //!
    //! Adding frames in order gives the same result as toFrameSeq(); frames
    //! added out of order may be split into a different set of sequences that
    //! covers the same frames.
    FTK_CORE_API void addFrame(std::vector<FrameSeq>&, int64_t);

    //! Get the number of frames in a frame sequence.
    FTK_CORE_API size_t getFrameCount(const FrameSeq&);

    //! Get the number of frames in a list of frame sequences.
    FTK_CORE_API size_t getFrameCount(const std::vector<FrameSeq>&);

    //! Get the range spanned by a list of frame sequences.
    FTK_CORE_API std::optional<RangeI64> getRange(const std::vector<FrameSeq>&);

    //! Convert a frame sequence to a label.
    FTK_CORE_API std::string getLabel(const FrameSeq&, int pad = 0);

    //! Convert frame sequences to a label.
    FTK_CORE_API std::string getLabel(const std::vector<FrameSeq>&, int pad = 0);

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
    class FTK_CORE_API_TYPE Path
    {
    public:
        Path() = default;
        FTK_CORE_API explicit Path(
            const std::string&,
            const PathOptions& = PathOptions());
        FTK_CORE_API Path(
            const std::string& dir,
            const std::string& fileName,
            const PathOptions& = PathOptions());

        //! \name Path Options
        ///@{

        const PathOptions& getOptions() const;
        FTK_CORE_API void setOptions(const PathOptions&);

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

        FTK_CORE_API void setProtocol(const std::string&);
        FTK_CORE_API void setDir(const std::string&);
        FTK_CORE_API void setBase(const std::string&);
        FTK_CORE_API void setNum(const std::string&);
        FTK_CORE_API void setPad(int);
        FTK_CORE_API void setExt(const std::string&);
        FTK_CORE_API void setRequest(const std::string&);
        FTK_CORE_API void setFileName(const std::string&);

        ///@}

        //! \name File Sequences
        ///@{

        //! Get the range spanned by the sequence. This is the range from the
        //! first to the last frame, which for a partial sequence includes
        //! frames that are missing; use getSeq() for the frames themselves.
        const std::optional<RangeI64>& getFrames() const;

        //! Set the sequence to a contiguous range of frames.
        FTK_CORE_API void setFrames(const RangeI64&);

        //! Get the frames in the sequence.
        const std::vector<FrameSeq>& getSeq() const;

        //! Set the frames in the sequence. The list should be sorted and
        //! merged, as returned by toFrameSeq().
        FTK_CORE_API void setSeq(const std::vector<FrameSeq>&);

        //! Get the number of frames in the sequence.
        size_t getSeqSize() const;

        //! Get whether this is a sequence.
        bool isSeq() const;

        //! Get whether this sequence is missing frames within its range.
        bool isPartialSeq() const;

        //! Get whether this has a sequence wildcard ('#').
        bool hasSeqWildcard() const;

        //! Get a file name with the given frame number.
        std::string getFrame(int64_t frame, bool dir = false) const;

        //! Get the frame range string.
        std::string getFrameRange() const;

        //! Get whether a path is part of this sequence.
        bool seq(const Path&) const;

        //! Add a path to this sequence.
        FTK_CORE_API bool addSeq(const Path&);

        //! Group the sequence the way toFrameSeq() would. Paths built up with
        //! addSeq() hold the right frames whatever order they arrive in, but
        //! only reach the canonical grouping if they arrive in order.
        FTK_CORE_API void normalizeSeq();

        ///@}

        //! \name Utility
        ///@{

        //! Get whether the path is absolute.
        FTK_CORE_API bool isAbs() const;

        //! Test whether this extension matches one in the given list.
        FTK_CORE_API bool testExt(const std::vector<std::string>&) const;

        ///@}

        //! \name Constants
        ///@{

        FTK_CORE_API static std::string getNumbers();
        FTK_CORE_API static std::string getPathSeparators();

        ///@}

        FTK_CORE_API bool operator == (const Path&) const;
        FTK_CORE_API bool operator != (const Path&) const;

    private:
        void _parse(const PathOptions&);
        void _setSeq(const std::vector<FrameSeq>&);

        std::string _path;
        PathOptions _options;
        // constexpr, so it needs no definition of its own and no export.
        // It is data, and data is where a single FTK_CORE_API across several
        // libraries shows: ftkUI compiles ftkCore's headers with FTK_EXPORTS
        // set for its own API, so it sees this as dllexport where it wants
        // dllimport. A function survives that -- the linker takes it from the
        // import library -- and a variable does not:
        //
        //     error LNK2001: unresolved external symbol
        //     private: static struct std::pair<...> const ftk::Path::_invalid
        static constexpr std::pair<size_t, size_t> _invalid{
            std::string::npos, std::string::npos };
        std::pair<size_t, size_t> _protocol = _invalid;
        std::pair<size_t, size_t> _dir = _invalid;
        std::pair<size_t, size_t> _base = _invalid;
        std::pair<size_t, size_t> _num = _invalid;
        int _pad = 0;
        std::pair<size_t, size_t> _ext = _invalid;
        std::pair<size_t, size_t> _request = _invalid;
        std::vector<FrameSeq> _seq;
        // The range spanned by _seq, cached so getFrames() can return a
        // reference.
        std::optional<RangeI64> _frames;
    };

    //! Directory list sorting.
    enum class FTK_CORE_API_TYPE DirListSort
    {
        Name,
        Extension,
        Size,
        Time,

        Count,
        First = Name
    };
    FTK_ENUM(FTK_CORE_API, DirListSort);

    //! Directory list options.
    struct FTK_CORE_API_TYPE DirListOptions
    {
        DirListSort              sort         = DirListSort::Name;
        bool                     sortReverse  = false;

        //! Case-insensitive match against the file name: a substring, or a
        //! wildcard pattern when it contains "*" or "?". A directory that
        //! is entered is exempt from the filter -- it is the route to the
        //! files, not what is being looked for -- while one at the last
        //! level is subject to it like anything else.
        std::string              filter;

        bool                     filterFiles  = false;
        std::vector<std::string> filterExt;
        bool                     seq          = true;
        std::vector<std::string> seqExts;
        bool                     seqNegative  = true;
        size_t                   seqMaxDigits = 9;
        bool                     hidden       = false;

        //! How many directory levels to list: 1 is the directory alone.
        //! Deeper levels follow their directory's entry, each listed with
        //! these same options, so the order is deterministic. Directory
        //! links are listed but never entered, which also keeps a cycle of
        //! links from listing forever.
        int                      depth        = 1;

        FTK_CORE_API bool operator == (const DirListOptions&) const;
        FTK_CORE_API bool operator != (const DirListOptions&) const;
    };

    //! Directory list entry.
    struct FTK_CORE_API_TYPE DirEntry
    {
        Path                            path;
        bool                            isDir = false;
        // What std::filesystem::file_size() returns: size_t would narrow
        // it on a 32-bit target, WebAssembly included.
        std::uintmax_t                  size = 0;
        std::filesystem::file_time_type time;

        FTK_CORE_API bool operator == (const DirEntry&) const;
        FTK_CORE_API bool operator != (const DirEntry&) const;
    };

    //! List directory contents.
    FTK_CORE_API std::vector<DirEntry> dirList(
        const std::filesystem::path&,
        const DirListOptions& = DirListOptions());

    //! List directory contents.
    //!
    //! The name is UTF-8, which is what the paths ftk hands out hold. Without
    //! this overload the conversion is left to the implementation, and on
    //! Windows that reads a narrow string as the active code page.
    FTK_CORE_API std::vector<DirEntry> dirList(
        const std::string&,
        const DirListOptions& = DirListOptions());

    //! Find the frames of a sequence on disk.
    //!
    //! expandSeq() searches only when the path has no range yet, so it cannot
    //! find the frames of a sequence whose range was stated rather than
    //! discovered. This always searches.
    FTK_CORE_API std::vector<FrameSeq> findSeq(
        const Path&,
        const PathOptions& = PathOptions());

    //! Expand a file sequence. This function will search the directory for
    //! other frames that match the given file name.
    FTK_CORE_API Path expandSeq(
        const Path&,
        const PathOptions& = PathOptions());

    //! Serialize a path.
    //!
    //! The frames a path covers are its own state rather than part of its
    //! name, so a path written as its name alone comes back naming one file.
    //! This writes them beside it, as data: a sequence has a run for each
    //! hole in it, with a step as well as a range, and a name made to carry
    //! all of that would have to be parsed back out again.
    //!
    //! A path whose name already says what it covers -- a single file, or one
    //! frame -- is written as that name, so the ordinary case reads and
    //! writes as it always did.
    FTK_CORE_API void to_json(nlohmann::json&, const Path&);

    FTK_CORE_API void to_json(nlohmann::json&, const PathOptions&);
    FTK_CORE_API void to_json(nlohmann::json&, const DirListOptions&);

    FTK_CORE_API void from_json(const nlohmann::json&, Path&);
    FTK_CORE_API void from_json(const nlohmann::json&, PathOptions&);
    FTK_CORE_API void from_json(const nlohmann::json&, DirListOptions&);

    ///@}
}

#include <ftk/Core/PathInline.h>
