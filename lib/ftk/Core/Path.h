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

    //! Convert a UTF-8 string to a file system path.
    //!
    //! ftk strings are UTF-8, and the standard's way of saying so changed
    //! underfoot: C++20 deprecated u8path() and made u8string() return a
    //! char8_t string. These two cross the boundary the same way under
    //! either standard, and they are the only place the difference lives.
    FTK_CORE_API std::filesystem::path toFileSystem(const std::string&);

    //! Convert a file system path to a UTF-8 string.
    FTK_CORE_API std::string fromFileSystem(const std::filesystem::path&);

    //! Convert a file system path to a UTF-8 string with generic
    //! (forward slash) separators.
    FTK_CORE_API std::string fromFileSystemGeneric(const std::filesystem::path&);

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

        //! Where an application keeps its own files -- settings, logs,
        //! recovery state: "Application Support" on macOS, the roaming
        //! AppData directory on Windows, XDG_CONFIG_HOME or ~/.config
        //! elsewhere. Not Documents, which cloud drives sync and macOS gates
        //! behind a permission prompt; a log rewritten on every launch trips
        //! over both.
        Config,

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
        //! Whether a minus in front of a frame number makes it negative. A minus
        //! in front of a number padded with zeros is part of the base name.
        bool   seqNegative  = true;
        //! The most digits a frame number can have. A longer run of digits is
        //! not taken for a frame number.
        size_t seqMaxDigits = 9;

        bool operator == (const PathOptions&) const = default;
    };

    //! Frame sequence.
    struct FTK_CORE_API_TYPE FrameSeq
    {
        FrameSeq() = default;
        FTK_CORE_API explicit FrameSeq(const RangeI64&, int inc = 1);
        FTK_CORE_API FrameSeq(int64_t, int64_t, int inc = 1);
        FTK_CORE_API explicit FrameSeq(int64_t);

        //! The first and last frames.
        RangeI64 range;
        //! The step between frames: 1 for every frame, 2 for every other one.
        int      inc = 1;

        bool operator == (const FrameSeq&) const = default;
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

    //! Get the step of a list of frame sequences: the smallest spacing
    //! between its frames, which is one for a sequence of every frame and
    //! two for one rendered on twos. One for a single frame or none.
    FTK_CORE_API int64_t getStep(const std::vector<FrameSeq>&);

    //! Get how many frames are missing from a list of frame sequences: the
    //! frames at its step, between its first and last, that it does not
    //! have. Frames 1, 3, 7 and 9 are missing one, frame 5.
    FTK_CORE_API size_t getMissingCount(const std::vector<FrameSeq>&);

    //! Get the frames missing from a list of frame sequences, as
    //! getMissingCount() counts them: for frames 1, 3, 7 and 9, frame 5.
    FTK_CORE_API std::vector<FrameSeq> getMissing(const std::vector<FrameSeq>&);

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

        //! Get the path as a string.
        const std::string& get() const;
        //! Get whether the path is empty.
        bool isEmpty() const;

        //! Get whether the path has a protocol.
        bool hasProtocol() const;
        //! Get whether the path has a directory.
        bool hasDir() const;
        //! Get whether the path has a base name.
        bool hasBase() const;
        //! Get whether the path has a frame number, or a "#" wildcard.
        bool hasNum() const;
        //! Get whether the path has an extension.
        bool hasExt() const;
        //! Get whether the path has a request.
        bool hasRequest() const;

        //! Get the protocol, such as "file://".
        std::string getProtocol() const;
        //! Get the directory, with its trailing separator, such as "/tmp/".
        std::string getDir() const;
        //! Get the base name: what comes before the frame number and the
        //! extension, such as "render.".
        std::string getBase() const;
        //! Get the frame number as it is written, such as "0001", or the "#"
        //! wildcard.
        std::string getNum() const;
        //! Get the frame number padding: the number of digits when the number
        //! starts with a zero or is a wildcard, and zero otherwise.
        int getPad() const;
        //! Get the extension, with its dot, such as ".exr".
        std::string getExt() const;
        //! Get the request, with its question mark, such as "?user=foo".
        std::string getRequest() const;
        //! Get the file name: the base name, frame number and extension, after
        //! the directory when dir is true. The protocol and the request are
        //! left out.
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

        //! Get the frame range string. When count is true a sequence that
        //! is missing frames says how many, e.g. "0001-0020 (8 missing)",
        //! and one on a step with none missing says the step, e.g.
        //! "0001-0019 (step 2)" -- what a file browser shows, so that a
        //! broken render is seen before it is opened.
        std::string getFrameRange(bool count = false) const;

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

        //! Get the characters a frame number is made of: the digits and "#".
        FTK_CORE_API static std::string getNumbers();
        //! Get the characters that separate directories: forward slash and
        //! backslash.
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
        //! How the entries are sorted. Directories come first whatever the sort.
        DirListSort              sort         = DirListSort::Name;
        //! Whether the sort is reversed.
        bool                     sortReverse  = false;

        //! Case-insensitive match against the file name: a substring, or a
        //! wildcard pattern when it contains "*" or "?". A directory that
        //! is entered is exempt from the filter -- it is the route to the
        //! files, not what is being looked for -- while one at the last
        //! level is subject to it like anything else.
        std::string              filter;

        //! Whether to leave the files out, listing only directories.
        bool                     filterFiles  = false;
        //! The extensions of the files to list, lower case with the dot, such as
        //! ".exr". Empty lists every file. Directories are not filtered.
        std::vector<std::string> filterExt;
        //! Whether numbered files are grouped into sequences, each listed as one
        //! entry.
        bool                     seq          = true;
        //! The extensions of the files that are grouped into sequences, lower
        //! case with the dot. Empty groups any file.
        std::vector<std::string> seqExts;
        //! Whether a minus in front of a frame number makes it negative; see
        //! PathOptions::seqNegative.
        bool                     seqNegative  = true;
        //! The most digits a frame number can have; see
        //! PathOptions::seqMaxDigits.
        size_t                   seqMaxDigits = 9;
        //! Whether to list the files and directories whose names start with a
        //! dot.
        bool                     hidden       = false;

        //! How many directory levels to list: 1 is the directory alone.
        //! Deeper levels follow their directory's entry, each listed with
        //! these same options, so the order is deterministic. Directory
        //! links are listed but never entered, which also keeps a cycle of
        //! links from listing forever.
        int                      depth        = 1;

        bool operator == (const DirListOptions&) const = default;
    };

    //! Directory list entry.
    struct FTK_CORE_API_TYPE DirEntry
    {
        //! The path. For a sequence it names the first frame and holds the
        //! frames found.
        Path                            path;
        //! Whether the entry is a directory.
        bool                            isDir = false;
        // What std::filesystem::file_size() returns: size_t would narrow
        // it on a 32-bit target, WebAssembly included.
        //! The size, in bytes: for a sequence the total of its files, and zero
        //! for a directory.
        std::uintmax_t                  size = 0;
        //! When the entry was last written. For a sequence, the latest of its
        //! files.
        std::filesystem::file_time_type time;

        bool operator == (const DirEntry&) const = default;
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
#include <ftk/Core/Path.h>
